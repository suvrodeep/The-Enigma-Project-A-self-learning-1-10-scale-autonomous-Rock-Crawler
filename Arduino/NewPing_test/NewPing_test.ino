#include <NewPing.h>

NewPing sonar(8, 7, 500);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(sonar.ping_cm());
  delay(500);

}
