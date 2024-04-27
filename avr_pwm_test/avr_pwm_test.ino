#define _PWM_LOGLEVEL_ 0
#define MOTOR_IN2 11
#include "AVR_PWM.h"




  // Pins tested OK on Nano / UNO
  //#define pinToUse      9            // Timer1A on UNO, Nano, etc
  #define pinToUse     10            // Timer1B on UNO, Nano, etc
  //#define pinToUse      5               // Timer0B on UNO, Nano, e
  //#define pinToUse       3            // Timer2B on UNO, Nano, etc


//creates pwm instance
AVR_PWM* PWM_Instance;

float frequency;
float dutyCycle;
float real_DutyCycle;
float new_level;

int hall_effect_sensor = A0; 
int hall_effect_val; 

void setup()
{
  Serial.begin(9600);
  
  pinMode(MOTOR_IN2, OUTPUT);
  digitalWrite(MOTOR_IN2, LOW);

  //assigns PWM frequency of 20 KHz and a duty cycle of 0%
  PWM_Instance = new AVR_PWM(pinToUse, 1000, 0);

  // calibration loop
  // for (float i = 0.0; i <= 99.0; i++) {
  //   frequency = 1000;
  //   PWM_Instance->setPWM(pinToUse, frequency, i);
  //   hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
  //   Serial.println(hall_effect_val);
  //   delay(500);

  // }
}

void loop()
{

  frequency = 5000;
  PWM_Instance->setPWM(pinToUse, frequency, 20);
  hall_effect_val = analogRead(hall_effect_sensor);   // read sensor value
  Serial.println(hall_effect_val);


  // frequency = 1000;
  // dutyCycle = 25;

  // Serial.println("Duty Cycle: 25%");
  // PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);

  
}
