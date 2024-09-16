#ifndef TOUCH_CONTROLLER_H
#define TOUCH_CONTROLLER_H

#include <Arduino.h>

// Enum for touch states
enum TouchState {
    IDLE,
    DISCHARGING,
    CHARGING,
    MEASURING
};

class TouchController {
  private:
    uint8_t touchPin;          // GPIO pin used for touch input
    unsigned long threshold;   // Threshold for touch detection
    int releaseThreshold;      // Number of measurements needed to detect release
    TouchState currentState;   // Current state of the touch sensor
    unsigned long startTime;   // Time at the start of a touch measurement
    bool touchActive;          // Flag to track if touch is active
    int releaseCounter;        // Counter for consecutive measurements above threshold

  public:
    // Constructor
    TouchController(uint8_t pin, unsigned long touchThreshold, int releaseThreshold);

    // Initializes the touch controller
    void setup();

    // Detects touch events
    bool detectTouch();

    // Resets the state of the touch controller
    void reset();
};

#endif
