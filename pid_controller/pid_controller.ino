#include "ArduPID.h"
// #include "AVR_PWM.h"

// #define _PWM_LOGLEVEL_ 0
#define MOTOR_IN2 5
#define MOTOR_IN1 6
// #define timer1B     10

// stuff for controller
ArduPID myController;
double setpoint = 700;
double K = 1200;
double input;
double output;
double p = 100 * K;
double i = 0 * K;
double d = 1 * K;


// stuff for PWM signal
// AVR_PWM* PWM_Instance;
float frequency;
float dutyCycle;

void setup() {
  // setting up Serial and controller
  Serial.begin(115200);
  myController.begin(&input, &output, &setpoint, p, i, d);
  
  // setting hall effect sensor to input mode
  pinMode(A0, INPUT);   

  // setting electromagnet as output
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);

  // initializing pin2 to low for correct EM orientation
  digitalWrite(MOTOR_IN2, LOW);

  //assigns PWM frequency of 20 KHz and a duty cycle of 0%
  // PWM_Instance = new AVR_PWM(timer1B, 5000, 0);

}

void loop()
{
  input = analogRead(A0); // Replace with sensor feedback

  myController.compute();
  myController.debug(&Serial, "myController", PRINT_INPUT    | // Can include or comment out any of these terms to print
                                              PRINT_OUTPUT   | // in the Serial plotter
                                              PRINT_SETPOINT |
                                              PRINT_BIAS     |
                                              PRINT_P        |
                                              PRINT_I        |
                                              PRINT_D);
  
  analogWrite(MOTOR_IN1, output); // Replace with plant control signal

  // PWM_Instance = new AVR_PWM(timer1B, 5000, output);
}

float sensor_to_distance(float sensor_val){

  return 277 + (-1.16 * sensor_val) + (1.86E-03 * pow(sensor_val,2)) + (-1.33E-06 * pow(sensor_val,3)) + (3.53E-10 * pow(sensor_val,4));
}
