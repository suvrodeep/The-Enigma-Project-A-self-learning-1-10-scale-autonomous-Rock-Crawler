// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(9, HIGH);   
  digitalWrite(10, HIGH);   
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);                       // wait for a second
  digitalWrite(9, LOW);   
  digitalWrite(10, LOW);
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);                       
}