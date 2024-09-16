#include "TouchController.h"

TouchController::TouchController(uint8_t pin, unsigned long touchThreshold, int releaseThreshold)
  : touchPin(pin), threshold(touchThreshold), releaseThreshold(releaseThreshold), 
    currentState(IDLE), touchActive(false), releaseCounter(0), startTime(0) {}

bool TouchController::detectTouch() {
  unsigned long currentTime = micros();  // Capture the current time

  switch (currentState) {
    case IDLE:
      pinMode(touchPin, OUTPUT);
      digitalWrite(touchPin, LOW);
      startTime = currentTime;
      currentState = DISCHARGING;
      break;

    case CHARGING:
      pinMode(touchPin, INPUT); 
      startTime = currentTime;
      currentState = MEASURING;
      break;

    case MEASURING:
      currentTime = micros(); 

      if (digitalRead(touchPin) == HIGH) {
        unsigned long duration = currentTime - startTime;
        if (duration < threshold) {
          if (!touchActive) {
            Serial.println("Touch detected");
            touchActive = true;
          }
          releaseCounter = 0;  // Reset release counter when touch is detected
        }
      } else if (currentTime - startTime > threshold) {
        // If no touch is detected (duration exceeded threshold), start counting for release
        releaseCounter++;
        if (releaseCounter >= releaseThreshold) {
          if (touchActive) {
            Serial.println("Touch released");
            touchActive = false;  // Mark touch as released after threshold timeout
            releaseCounter = 0;
          }
        }
        currentState = IDLE;  // Return to IDLE if no touch detected after threshold time
      }
      break;

    case DISCHARGING:
      if (currentTime - startTime >= 5) {  // Wait 5 microseconds to discharge the pin
        startTime = currentTime;
        currentState = CHARGING;
      }
      break;
  }
  return touchActive;  // Return the current touch state
}

void TouchController::reset() {
  currentState = IDLE;
  touchActive = false;
  releaseCounter = 0;
  startTime = micros();
  Serial.println("TouchController reset.");
}
