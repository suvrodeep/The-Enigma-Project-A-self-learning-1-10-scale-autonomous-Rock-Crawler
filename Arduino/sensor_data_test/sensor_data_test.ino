#include <NewPing.h>

int distance[] = {0, 0, 0, 0, 0, 0};
int* check_distance;
int counter = 0;         // Global counter
int index = 0;           // Global index
int steering;            // Variable to store steering reading 
int throttle;            // Variable to store throttle reading

NewPing sensorArray[] = {NewPing(13, 8, 300), 
  NewPing(13, 7, 300), 
  NewPing(13, 9, 300),
  NewPing(13, 11, 300), 
  NewPing(13, 10, 300),
  NewPing(13, 12, 300)
};

int* get_distance() {             // Define get_distance function. Pings sensors and returns the distance
  for (index = 0; index < 6; index++){
    distance[index] = sensorArray[index].ping_cm();
    delay(50);
  }
  return(distance);
}

void setup() {                    // Code to run once during initial setup goes here 
  pinMode(6, INPUT); // Throttle pin
  pinMode(5, INPUT); // Steering pin
  pinMode(4, OUTPUT); // Pin to control relay
  digitalWrite(4, LOW);
  
  Serial.begin(9600);
}

void loop() {                   // Code to run over and over in a loop goes here
    steering = pulseIn(5, HIGH, 25000); // Read the pulse width of 
    throttle = pulseIn(6, HIGH, 25000); // each channel
  
    check_distance = get_distance();        // Ping sensors and get distance
    for (index = 0; index < 6; index++)
    {
      Serial.print("Sensor ");
      Serial.print(index + 1);
      Serial.print(": ");
      Serial.println(check_distance[index]);
    }
    Serial.print("Steering:"); // Print the value of 
    Serial.println(steering);        // each channel
    Serial.print("Throttle:");
    Serial.println(throttle);
    Serial.println();
    delay(500);
}




