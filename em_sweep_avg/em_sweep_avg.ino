#define MOTOR_IN1 6
#define MOTOR_IN2 5

float array[105];
int mag_strength = 255;             // new for this version

int hall_effect_sensor = A0; 
int hall_effect_val; 

void setup() {
  // setup serial 
  Serial.begin(9600);

  // setting pinmodes for sensor + electromagent
  pinMode(A0, INPUT);   
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  
  // starting magnet at zero strength
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, 0);

  for (int i = 150; i <= 255; i++) {
    float avg = 0;
    for (int j = 0; j<=4; j++){
      analogWrite(MOTOR_IN1, i);
      hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
      avg = avg + hall_effect_val;
    }
    float tmp = avg;
    array[i-150] = avg/5.0;
    delay(250);
    
    
  }

  for(int i = 0; i < 105; i++){
    Serial.println(array[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
