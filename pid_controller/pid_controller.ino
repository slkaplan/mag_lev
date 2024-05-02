#define MOTOR_IN2 5
#define MOTOR_IN1 6
#define SENSOR_PIN A0
#define WINDOW_SIZE 5


bool DEBUG = false;

//init moving average vars
int index = 0;
int sensor_sum = 0;
int sensor_readings[WINDOW_SIZE];
int filtered_reading;

//init pid variables
float input, output;
float setpoint = 13;
float e_total, e_prev;
float e = 0;

// PID tuning parameters
float Kp=10, Ki=0, Kd = 0.1;

// setting setpoint in mm from bottom of electromagnet

// init other variables
float sensor_val_raw;
float offset_sensor_val;
float distance;
float equilibrium_voltage; 
float mass = 0.012488; 
float gravity = 9.81; 
float Beta = 4.379 * pow(10, -8);
float analog_voltage; 

float time_step = 0.0001;

unsigned long previousMillis = 0;

void setup() {
  // setting up serial
  Serial.begin(115200);
  
  //setting pinmodes
  pinMode(SENSOR_PIN, INPUT); 
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);


  input = analogRead(A0);

  equilibrium_voltage = (mass * gravity * pow(setpoint/1000, 3)) / Beta;
  analog_voltage = ((equilibrium_voltage - 1.74) / 0.1) + 150;

}

void loop(){
  sensor_sum = sensor_sum - sensor_readings[index];
  sensor_val_raw = analogRead(A0);
  sensor_readings[index] = sensor_val_raw;
  sensor_sum = sensor_sum + sensor_val_raw;
  index = (index + 1) % WINDOW_SIZE;

  filtered_reading = sensor_sum / WINDOW_SIZE;

  offset_sensor_val = filtered_reading - sensor_offset(analog_voltage);

  input = sensor_to_distance(offset_sensor_val) - setpoint;

  output = equilibrium_voltage + controller(time_step, 0, input);

  output = constrain(output, 1.74, 12);
  output = int((output - 1.74) / 0.1 + 150);
  analog_voltage = output;

  // Serial.println(analog_voltage);

  analog_voltage = 0;
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, analog_voltage);

  if(DEBUG){
    debug();
  }
  
  // unsigned long currentMillis = millis();
  // time_step = currentMillis - previousMillis;
  // previousMillis = currentMillis;
}

void debug(){
  String input_str = "Input " + String(input);
  // Serial.println(input_str);

  String output_str = "Output " + String(output);
  // Serial.println(output_str);
}

float sensor_offset(float analog_voltage){
  return (0.693 * analog_voltage) - 94;
}

float sensor_to_distance(float offset_sensor_val){
  //return 294 + (-1.26*offset_sensor_val) + (0.00208*pow(offset_sensor_val,2)) - (0.00000155*pow(offset_sensor_val,3)) + (0.000000000431*pow(offset_sensor_val,4));
  return 651 + -3.09*offset_sensor_val + 0.00555*pow(offset_sensor_val, 2) + -0.00000443*pow(offset_sensor_val, 3) + 0.00000000132*pow(offset_sensor_val, 4);
} 

float controller(float del_t, float target_x, float measured_x){
  e = measured_x - target_x;
  float P = Kp * e;
  
  float I = Ki * e_total * del_t;

  float D = Kd * ((e - e_prev)/del_t);
  
  float v = (P + I + D);

  e_total = e_total + e;
  e_prev = e;

  return v;

}




