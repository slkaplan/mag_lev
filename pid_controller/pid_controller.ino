#define MOTOR_IN2 5
#define MOTOR_IN1 6
#define SENSOR_PIN A0


bool DEBUG = false;

//init pid variables
float input, output;
float setpoint = 4;
float e_total, e_prev;
float e = 0;

// PID tuning parameters
float Kp=1.8, Ki=0, Kd = 0.0000005;

// setting setpoint in mm from bottom of electromagnet

// other things
float sensor_val_raw;
float offset_sensor_val;
float distance;
float initial_voltage; 
float mass; 
float gravity; 
float Beta = 3.423 * pow(10, -6);
float analog_voltage; 

float time_step = 6.25 * .0000001;





void setup() {
  // setting up serial
  Serial.begin(9600);
  

  //setting pinmodes
  pinMode(SENSOR_PIN, INPUT); 
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);


  input = analogRead(A0);



  initial_voltage = (mass * gravity * pow(setpoint/1000, 2)) / Beta;
  analog_voltage = (initial_voltage - 150) * .097733;
  

}

void loop(){
  sensor_val_raw = analogRead(A0) - 511.5;
  offset_sensor_val = sensor_val_raw - sensor_offset(analog_voltage);
  // Serial.println(offset_sensor_val);

  input = sensor_to_distance(offset_sensor_val) - setpoint;
  Serial.println(input);
  output = controller(time_step, 0, input);
  // Serial.println(output);
  output = constrain(output, -12, 12);
  output = int(output/12 * 255);
  analog_voltage = output;

  if(output < 0){
    output = abs(output);
    digitalWrite(MOTOR_IN2, LOW);
    analogWrite(MOTOR_IN1, output);
  }
  else{
    output = abs(output);
    digitalWrite(MOTOR_IN1, LOW);
    analogWrite(MOTOR_IN2, abs(output));
  }

  if(DEBUG){
    debug();
  }
  

  
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
  return 294 + (-1.26*offset_sensor_val) + (0.00208*pow(offset_sensor_val,2)) - (0.00000155*pow(offset_sensor_val,3)) + (0.000000000431*pow(offset_sensor_val,4));
} 

float controller(float del_t, float target_x, float measured_x){
  e = measured_x - target_x;
  float P = Kp * e;
  // float I = Ki * e_total * del_t;
  // Serial.println(e);

  float D = Kd * ((e - e_prev)/del_t);

  
  float v = -1 * (P + D);
  if(v < 0){
    v = v - 1.7;
  }
  if(v > 0){
    v = v + 1.7;
  }

  e_total = e_total + e;
  e_prev = e;

  return v;

}




