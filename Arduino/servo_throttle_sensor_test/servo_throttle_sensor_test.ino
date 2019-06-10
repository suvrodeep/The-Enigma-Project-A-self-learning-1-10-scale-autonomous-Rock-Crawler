#include <NewPing.h>
#include <Servo.h>

#define ESCDetect A3  // Define pin for analog read of ESC voltage
#define statusPin 13  // Monitor status of sensors

const int steering_right_max = 35;    // Maximum angle of steering in right turns
const int steering_left_max = 131;    // Maximum angle of steering in left turns
const int steering_straight = 85;     // Value for steering to hold straight
const int maximumRange = 300;         // Maximum range needed
const int minimumRange = 50;          // Minimum range needed

int throttle_pos = 90;   // Variale to store current throttle position. Initialize to neutral
int inst_dist = 0;
int distance[] = {0, 0, 0};
int* check_distance;
int counter = 0;         // Global counter
int index = 0;

bool esc_detect = 0;     // ESC detection flag
bool sensor_test = 0;    // Sensor test flag

NewPing sensorArray[] = {NewPing(6, 9, 700), NewPing(8, 7, 700), NewPing(4, 2, 700)};

Servo throttle;               // Initiate throttle as a servo object
Servo steering;               // Initiate steering as a servo object


int smooth_accel (int throttle_input) {     // Define smooth acceleration function
  if (throttle_input > throttle_pos) {
    if (throttle_pos < 90 && throttle_input > 90) {   // If commanded to move forward when moving in reverse
      throttle.write(90); // Release throttle
      delay(1000);        // Allow time to stop
      for (counter = 90; counter <= throttle_input; counter++){   // Accelerate in forward direction
      throttle.write(counter);
      delay(30);
      }
    }
    else {
      for (counter = throttle_pos; counter <= throttle_input; counter++){   // Accelerate smmothly in forward direction
      throttle.write(counter);
      delay(30);
      }
    }
  }
  
  else if (throttle_input < throttle_pos) {
    if (throttle_pos > 90 && throttle_input < 90) {    //If commanded to reverse when moving forward
      throttle.write(90); // Release throttle
      delay(1000);        // Allow time to stop
      for (counter = 90; counter >= throttle_input; counter--){   // Accelerate in reverse
      throttle.write(counter);
      delay(30);
      }
    }
    else {
      for (counter = throttle_pos; counter >= throttle_input; counter--){   // Accelerate smoothly in reverse
      throttle.write(counter);
      delay(30);
      }
    }
  }
  else {
    throttle.write(throttle_input);
    counter = throttle_input;
  }
  return(counter);
}


int* get_distance () {             // Define get_distance function. Pings sensors and returns the distance
  for (index = 0; index < 3; index++) {
    distance[index] = 0;
  }
  
  for (counter = 0; counter < 5; counter++) {
    for (index = 0; index < 3; index++) {
      inst_dist = sensorArray[index].ping_cm();
      if (inst_dist == 0) {
        distance[index] = distance[index] + (distance[index] / (counter + 1));
      }
      else {
        distance[index] = distance[index] + inst_dist;
      }
      if (counter == 4) {
        if (distance[index] / 5 > 10){
          distance[index] = distance[index] / 5;
        }
        else {
          distance[index] = 0;
        }
      }
    }
    delay(40);
  }
  for (index = 0; index < 3; index++) {
    Serial.println(distance[index]);
  }
  Serial.println();
  return(distance);
}



void stop_immediate() {
  throttle.write(90);
  steering.write(steering_straight);
}


void setup() {                    // Code to run once during initial setup goes here 
  Serial.begin(9600);
  pinMode(statusPin, OUTPUT);
  check_distance = get_distance();
    
  if ((analogRead(ESCDetect) > 470) && (check_distance[1] > 0)) {
    throttle.attach(5);   // Attaches the throttle on pin 5
    throttle.write(90);   // Arms the ESC with zero throttle
    steering.attach(3);   // Attaches the steering on pin 3
    steering.write(85);
    esc_detect = 1;
    sensor_test = 1;
    delay(5000);
  }
  else if ((analogRead(ESCDetect) > 470) && (check_distance[1] == 0)) {
    esc_detect = 1;
    sensor_test = 0;
    digitalWrite(statusPin, HIGH);
    delay(200);
    digitalWrite(statusPin, LOW);
  }
  else if ((analogRead(ESCDetect) <= 470) && (check_distance[1] > 0)) {
    esc_detect = 0;
    sensor_test = 1;
    digitalWrite(statusPin, HIGH);
    delay(50);
    digitalWrite(statusPin, LOW);
  }
  else {
    esc_detect = 0;
    sensor_test = 0;
    digitalWrite(statusPin, HIGH);
    delay(500);
    digitalWrite(statusPin, LOW);
    delay(300);
  }
}

void loop() {                   // Code to run over and over in a loop goes here
  if (esc_detect && sensor_test) {
    digitalWrite(statusPin, HIGH);
    check_distance = get_distance();        // Ping sensors and get distance
   
    if (check_distance[1] >= maximumRange) {
      steering.write(steering_straight);
      throttle_pos = smooth_accel(115);  // Increase speed if there is no obstacle within 200 cm
    }
    else if (check_distance[1] > minimumRange && check_distance[1] < maximumRange) {
      steering.write(steering_straight);
      throttle_pos = smooth_accel(108);  // Maintain moderate speed if obstacle is within 200 cm
    }
    else {
      do {
        throttle_pos = smooth_accel(78);   // Stop the car and initiate reverse if there is an obstacle within 10 cm
        steering.write(40);                // Turn 40 degrees and continue backing up until front is clear
        check_distance = get_distance();
      }
      while (check_distance[1] < 70 && check_distance[1] > 0);
    }
  
    if ((analogRead(ESCDetect) > 470) && (get_distance()[1] > 0)) {   // Verifying if ESC is still powered on and sensors are working
      esc_detect = 1;
      sensor_test = 1;
    }
    else {
      esc_detect = 0;
      sensor_test = 0;
      stop_immediate();
    }
  }
  
  else{
    setup();
  }
}
