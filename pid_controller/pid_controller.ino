#define MOTOR_IN2 5
#define MOTOR_IN1 6
#define SENSOR_PIN A0
#define WINDOW_SIZE 5

// Sensor to distance lookup table (index 0 is sensor reading 550)
float sensor_to_distance[176] = {10.24956875,10.04957363,9.853511231,9.661329288,9.472975862,9.28839936,9.107548531,\
                                8.930372467,8.756820602,8.58684271,8.42038891,8.257409662,8.097855767,7.941678371,7.788828959,
                                7.63925936,7.492921745, 7.349768626,7.209752859,7.072827641,6.93894651,6.808063349,6.68013238,\
                                6.55510817,6.432945627,6.3136,6.197026881,6.083182206,5.972022249,5.86350363,5.75758331,5.654218591,\
                                5.553367118,5.454986879,5.359036202,5.26547376,5.174258565,5.085349974,4.998707684,4.914291735,\
                                4.83206251,4.751980732,4.674007469,4.598104128,4.524232461,4.45235456,4.38243286,4.314430139,4.248309516,\
                                4.184034451,4.12156875,4.060876557,4.00192236,3.94467099,3.889087619,3.83513776,3.782787271,3.732002349,\
                                3.682749536,3.634995715,3.58870811,3.543854289,3.500402161,3.458319977,3.417576332,3.37814016,3.33998074,\
                                3.303067692,3.267370977,3.232860901,3.19950811,3.167283592,3.136158678,3.106105041,3.077094696,3.0491,\
                                3.022093653,2.996048695,2.970938512,2.946736827,2.92341771,2.90095557,2.87932516,2.858501573,2.838460247,\
                                2.81917696,2.800627832,2.782789328,2.76563825,2.749151748,2.73330731,2.718082768,2.703456295,2.689406407,\
                                2.675911962,2.66295216,2.650506544,2.638554997,2.627077746,2.61605536,2.60546875,2.595299168,2.58552821,\
                                2.576137813,2.567110256,2.55842816,2.55007449,2.542032551,2.534285991,2.5268188,2.51961531,2.512660196,\
                                2.505938474,2.499435504,2.493136984,2.48702896,2.481097815,2.475330277,2.469713416,2.464234642,2.45888171,\
                                2.453642715,2.448506096,2.443460631,2.438495445,2.4336,2.428764104,2.423977905,2.419231894,2.414516904,\
                                2.40982411,2.405145029,2.400471521,2.395795788,2.391110372,2.38640816,2.38168238,2.376926601,2.372134737,\
                                2.367301041,2.36242011,2.357486883,2.35249664,2.347445005,2.342327943,2.33714176,2.331883107,2.326548974,\
                                2.321136696,2.315643949,2.31006875,2.304409459,2.29866478,2.292833755,2.286915772,2.28091056,2.274818189,\
                                2.268639072,2.262373965,2.256023964,2.24959051,2.243075383,2.236480708,2.22980895,2.223062917,2.21624576,\
                                2.20936097,2.202412383,2.195404174,2.188340863,2.18122731,2.174068718,2.166870633,2.159638942,2.152379873,2.1451};

//init moving average vars
int index = 0;
int sensor_sum = 0;
int sensor_readings[WINDOW_SIZE];
int filtered_reading;

//init pid variables
float input;
int output;
float setpoint = 6;
float e_total, e_prev;
float e = 0;

// PID tuning parameters
int Kp=70, Ki=0, Kd = 0.000005;

// setting setpoint in mm from bottom of electromagnet

// init other variables
int sensor_val_raw;
int offset_sensor_val;
float distance;
int equilibrium_voltage;
int analog_voltage; 

float time_step = 0;

unsigned long previousMicros = 0;

void setup() {
  // setting up serial
  Serial.begin(115200);
  
  //setting pinmodes
  pinMode(SENSOR_PIN, INPUT); 
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);


  input = analogRead(A0);

  analog_voltage = 200;

}

void loop(){
  // Get sensor data & pass through moving average filter
  sensor_sum = sensor_sum - sensor_readings[index];
  sensor_val_raw = analogRead(A0);
  sensor_readings[index] = sensor_val_raw;
  sensor_sum = sensor_sum + sensor_val_raw;
  index = (index + 1) % WINDOW_SIZE;

  // Get filtered reading & correct for EM offset
  int filt_offset_reading = int(sensor_sum / WINDOW_SIZE - ((0.71 * analog_voltage) - 95.5));

  int lookup_reading = filt_offset_reading - 550;
  int distance_idx = constrain(lookup_reading - (lookup_reading % 2), 0, 176);
  input = sensor_to_distance[distance_idx] - setpoint;

  output = equilibrium_voltage + controller(time_step, 0, input);

  analog_voltage = constrain(output, 150, 255);

  //analog_voltage = 0;
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_IN1, analog_voltage);
  
  unsigned long currentMicros = micros();
  time_step = (currentMicros - previousMicros) * 0.000001;
  previousMicros = currentMicros;
}

float controller(float del_t, float target_x, float measured_x){
  e = measured_x - target_x;

  float P = Kp * e;
  
  float I = Ki * e_total * del_t;

  float D = Kd * ((e - e_prev)/del_t);
  
  float pwm = (P + I + D);

  e_total = e_total + e;
  e_prev = e;

  return pwm;

}




