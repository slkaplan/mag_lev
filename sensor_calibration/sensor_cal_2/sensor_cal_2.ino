#define MOTOR_IN2 5
#define MOTOR_IN1 6
#define SENSOR_PIN A0
#define WINDOW_SIZE 5

//init moving average vars
int index = 0;
int sensor_val_raw;
int sensor_sum = 0;
int sensor_readings[WINDOW_SIZE];
int filtered_reading;
float analog_voltage; 
int array[106];

void setup() {
  // setting up serial
  Serial.begin(115200);
  
  //setting pinmodes
  pinMode(SENSOR_PIN, INPUT); 
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);

  // digitalWrite(MOTOR_IN2, LOW);
  // for (int i = 150; i <= 255; i++) {
  //   analogWrite(MOTOR_IN1, i);
  //   for (int j = 0; j<=9; j++){
  //     sensor_sum = sensor_sum - sensor_readings[index];
  //     sensor_val_raw = analogRead(A0);
  //     sensor_readings[index] = sensor_val_raw;
  //     sensor_sum = sensor_sum + sensor_val_raw;
  //     index = (index + 1) % WINDOW_SIZE;
  //   }
  //   array[i-150] = sensor_sum / WINDOW_SIZE;
  //   delay(100);
  // }

  // for(int i = 0; i < 106; i++){
  //   Serial.println(array[i]);
  // }

  // analogWrite(MOTOR_IN1, 0);
}

void loop(){
  sensor_sum = sensor_sum - sensor_readings[index];
  sensor_val_raw = analogRead(A0);
  sensor_readings[index] = sensor_val_raw;
  sensor_sum = sensor_sum + sensor_val_raw;
  index = (index + 1) % WINDOW_SIZE;

  filtered_reading = sensor_sum / WINDOW_SIZE;
  Serial.println(filtered_reading);
  analog_voltage = 0;
  analogWrite(MOTOR_IN1, analog_voltage);

}