int steering; 
int throttle;

void setup() {

  pinMode(5, INPUT); // Throttle pin
  pinMode(3, INPUT); // Steering pin
  pinMode(10, OUTPUT); // Pin to control relay
  digitalWrite(10, LOW);

  Serial.begin(19200); 

}

void loop() {

  steering = pulseIn(3, HIGH, 25000); // Read the pulse width of 
  throttle = pulseIn(5, HIGH, 25000); // each channel
  
  Serial.print("Steering:"); // Print the value of 
  Serial.println(steering);        // each channel

  Serial.print("Throttle:");
  Serial.println(throttle);

  Serial.println();

  delay(500); // Delay to see output clearly
}

//throttle max(forward) = 2005; throttle min(backward) = 1005
//steering max(left) = 1890; throttle min(right) = 911
