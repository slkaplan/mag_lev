#define MOTOR_IN1 10
#define MOTOR_IN2 11

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

int dataNumber = 0;             // new for this version

int hall_effect_sensor = A0; 
int hall_effect_val; 
float distance = 0.0;


void setup() {

  pinMode(A0, INPUT);   // set analog pin as input
  Serial.begin(115200);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  
  // set for attraction mode
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, dataNumber);

  // for (int i = 0; i <= 255; i++) {
  //   analogWrite(MOTOR_IN1, i);
  //   hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
  //   Serial.println(hall_effect_val); 
  //   delay(500);
  //   // Serial.println((int)i);
  // }

  
     
   

}

void loop() {
  hall_effect_val = analogRead(hall_effect_sensor);
  distance = sensor_to_distance(hall_effect_val);
  Serial.println(distance);
  // recvWithEndMarker();
  // showNewNumber();

  // for (int i = 0; i <= 255; i++) {
  //   analogWrite(MOTOR_IN2, i);
  //   Serial.println((int)i);
  // }

  // hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
  // // Serial.println(hall_effect_val);    
  // delay(500);                            // print value to serial


}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version

        digitalWrite(MOTOR_IN1, LOW);
        analogWrite(MOTOR_IN2, dataNumber);

        // Serial.print("This just in ... ");
        // Serial.println(receivedChars);
        Serial.print("Data as Number ... ");    // new for this version
        Serial.println(dataNumber);     // new for this version
        newData = false;
    }
}

float sensor_to_distance(float sensor_val){

  return 277 + (-1.16 * sensor_val) + (1.86E-03 * pow(sensor_val,2)) + (-1.33E-06 * pow(sensor_val,3)) + (3.53E-10 * pow(sensor_val,4));
}


