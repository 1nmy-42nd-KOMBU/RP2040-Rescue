#include <Arduino.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 18,19,20,21);

void setup() {
  myStepper.setSpeed(10);
}

void loop() {
  myStepper.step(stepsPerRevolution);
  delay(1000);
}
