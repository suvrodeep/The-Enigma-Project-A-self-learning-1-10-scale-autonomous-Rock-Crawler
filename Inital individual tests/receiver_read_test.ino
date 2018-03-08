int steering; 
int throttle;

void setup() {

  pinMode(5, INPUT); // Throttle pin
  pinMode(3, INPUT); // Steering pin
  pinMode(9, OUTPUT); // Pin to control relay
  digitalWrite(9, LOW);

  Serial.begin(9600); 

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
