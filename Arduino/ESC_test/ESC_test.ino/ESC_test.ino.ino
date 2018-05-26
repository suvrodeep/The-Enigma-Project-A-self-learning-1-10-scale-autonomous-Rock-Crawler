/* Axial ESC ramp test
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);
  delay(3000);
}

void loop() {
  //myservo.write(90);
  //delay(15);

  for (pos = 90; pos <= 130; pos += 1) { // goes from 90 degrees to 180 degrees
    // in steps of 1 degree
    if(pos == 130){
      myservo.write(90);
    }
    else{
       myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
    }
   }
   delay(2000);
   
  for (pos = 90; pos >= 60; pos -= 1) { // goes from 90 degrees to 0 degrees
    if(pos == 60){
      myservo.write(90);
    }
    else{
       myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(50);                       // waits 15ms for the servo to reach the position
    }
   }
   delay(2000);
}

