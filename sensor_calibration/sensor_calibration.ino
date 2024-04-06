int hall_effect_sensor = A0; 
int hall_effect_val;    

void setup() {

  pinMode(A0, INPUT);   // set analog pin as input
  Serial.begin(9600);   // initialize serial interface
   

}

void loop() {
  
  hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
  Serial.println(hall_effect_val);                                // print value to serial

  delay(1000);  

}
