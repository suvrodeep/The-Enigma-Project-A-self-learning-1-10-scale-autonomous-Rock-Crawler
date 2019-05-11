#include <NewPing.h>
#include <Servo.h>

const int steering_neutral = 85;
const int throttle_neutral = 90;

int distance[] = {0, 0, 0, 0, 0, 0};
int serial_input = 0;
int throttle_pos = throttle_neutral;    // Variable to store current throttle position. Initialize to neutral
int steering_pos = steering_neutral;    // Variable to store current steering position. Initialize to neutral
int counter = 0;         // Global counter
int index = 0;           // Global index
int steering_in = steering_neutral;            // Variable to store steering command 
int throttle_in = throttle_neutral;            // Variable to store throttle command
char skip_char = ",";
Servo throttle;               // Initiate throttle as a servo object
Servo steering;               // Initiate steering as a servo object


NewPing sensorArray[] = {NewPing(13, 8, 300), 
  NewPing(13, 7, 300), 
  NewPing(13, 9, 300),
  NewPing(13, 11, 300), 
  NewPing(13, 10, 300),
  NewPing(13, 12, 300)
};


void get_distance() {             // Define get_distance function. Pings sensors and returns the distance
  for (index = 0; index < 6; index++){
    distance[index] = sensorArray[index].ping_cm();
    delay(50);
  }

  for (index = 0; index < 6; index++){
    Serial.print(distance[index]);
    if (index < 5) {
     Serial.print(","); 
    }
  }
  Serial.println();
}


int smooth_accel (int throttle_input) {     // Define smooth acceleration function
  if (throttle_input > throttle_pos) {
    if (throttle_pos < 90 && throttle_input > 90) {   // If commanded to move forward when moving in reverse
      throttle.write(90); // Release throttle
      delay(1000);        // Allow time to stop
      for (counter = 90; counter <= throttle_input; counter++){   // Accelerate in forward direction
      throttle.write(counter);
      delay(20);
      }
    }
    else {
      for (counter = throttle_pos; counter <= throttle_input; counter++){   // Accelerate smmothly in forward direction
      throttle.write(counter);
      delay(20);
      }
    }
  }
  
  else if (throttle_input < throttle_pos) {
    if (throttle_pos > 90 && throttle_input < 90) {    //If commanded to reverse when moving forward
      throttle.write(90); // Release throttle
      delay(1000);        // Allow time to stop
      for (counter = 90; counter >= throttle_input; counter--){   // Accelerate in reverse
      throttle.write(counter);
      delay(20);
      }
    }
    else {
      for (counter = throttle_pos; counter >= throttle_input; counter--){   // Accelerate smoothly in reverse
      throttle.write(counter);
      delay(20);
      }
    }
  }
  else {
    throttle.write(throttle_input);
    counter = throttle_input;
  }
  return(counter);
}


int set_steering(int steering_input) {    // Function to set steering
  steering.write(steering_input);
  return(steering_input);
}


int recv_serial() {             // Read serial data
  int input = 0;
  if (Serial.available() > 0){
   input = Serial.parseInt();
  }
  return(input);
}


void setup() {
  throttle.attach(6);                 // Attaches the throttle on pin 5
  throttle.write(throttle_neutral);   // Arms the ESC with zero throttle
  steering.attach(5);                 // Attaches the steering on pin 3
  steering.write(steering_neutral);   // Makes front wheels straight
  
  pinMode(4, OUTPUT);                 // Pin to control relay
  digitalWrite(4, HIGH);              // Deactivate relay to diable connection with receiver
  delay(5000);
  
  Serial.begin(9600);
}


void loop() {
  get_distance();        // Ping sensors and get distance

  Serial.println(-1)    //Getting ready to receive command through serial port
  while (recv_serial() != -1)
  serial_input = recv_serial();
  //Serial.println(serial_input);

  if (serial_input >= 1000 && serial_input <=2000){
    steering_in = serial_input - 1000;
  }
  else if (serial_input >= 2000 && serial_input <=3000){
    throttle_in = serial_input - 2000;
  }
  else{
    steering_in = steering_pos;
    throttle_in = throttle_pos;
  }
  
  steering_pos= set_steering(steering_in);
  throttle_pos = smooth_accel(throttle_in);

  //Serial.print(steering_pos);
  //Serial.print(",");
  //Serial.println(throttle_pos);
}


