#include <Servo.h>

#define echoPin 7     // Echo Pin
#define trigPin 8     // Trigger Pin
//#define LEDPin 13     // Onboard LED
#define throttlePin 5 // Throttle pin
#define ESCDetect A3  //

int throttle_pos = 90;  // Variale to store current throttle position. Initialize to neutral
int maximumRange = 200; // Maximum range needed
int minimumRange = 20;  // Minimum range needed
float duration;         // Variable to store sensor output in microseconds
float distance;         // Duration used to calculate distance

bool esc_detect = 0;

Servo throttle;  // Initiate throttle as a servo object


void smooth_accel (int throttle_input) {     // Define smooth acceleration function
  int counter = 0;
  
  if (throttle_input > throttle_pos) {
    if (throttle_pos < 90 && throttle_input > 90) {   // If commanded to move forward when moving in reverse
      /*for (counter = throttle_pos; counter <= 90; counter++){   // Stop smoothly if moving backward
      throttle.write(counter);
      delay(15);
      }*/
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
    throttle_pos = counter;
  }
  
  else if (throttle_input < throttle_pos) {
    if (throttle_pos > 90 && throttle_input < 90) {    //If commanded to reverse when moving forward
      /*for (counter = throttle_pos; counter >= 90; counter--){   // Stop smoothly if moving forward
      throttle.write(counter);
      delay(15);
      }*/
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
    throttle_pos = counter;
  }
  else {
    throttle.write(throttle_input);
    throttle_pos = throttle_input;
  }
}

void setup() {                    // Code to run once during initial setup goes here
  if (analogRead(ESCDetect) > 470) {
    pinMode(trigPin, OUTPUT);       // Define pin modes
    pinMode(echoPin, INPUT);
    throttle.attach(throttlePin);   // Attaches the throttle on pin 9 to the throttle object
    throttle.write(90);
    esc_detect = 1;
    delay(3000);
  }
  else {
    esc_detect = 0;
  }
}

void loop() {                   // Code to run over and over in a loop goes here
  if (esc_detect) {
    digitalWrite(trigPin, LOW);  // Send low initial to sensor
    delayMicroseconds(2); 
  
    digitalWrite(trigPin, HIGH); // Provide 10 microsecond high trigger pulse to sensor
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);  // End the trigger pulse
     
    duration = pulseIn(echoPin, HIGH); // Read output from sensor
    distance = duration * 0.017;       // Calculate distance
  
    if (distance >= maximumRange) {
      smooth_accel(112);  // Increase speed if there is no obstacle within 200 cm
    }
    else if (distance > minimumRange && distance < maximumRange) {
      smooth_accel(107);  // Maintain moderate speed if obstacle is within 200 cm
    }
    else {
      smooth_accel(77);   // Stop the car and initiate reverse if there is an obstacle within 10 cm
    }
  
    if (analogRead(ESCDetect) > 470) {   // Verifying if ESC is still powered on
      esc_detect = 1;
    }
    else {
      esc_detect = 0;
    }
  }
  
  else{
    setup();
  }

}