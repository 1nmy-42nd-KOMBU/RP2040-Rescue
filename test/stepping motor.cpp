#include <Arduino.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 10, 11, 12, 13);

void setup() {
  myStepper.setSpeed(10);
}

void loop() {
  myStepper.step(stepsPerRevolution);
  delay(1000);
}
