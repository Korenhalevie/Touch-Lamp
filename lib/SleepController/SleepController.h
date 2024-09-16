#ifndef SLEEPCONTROLLER_H
#define SLEEPCONTROLLER_H

#include <Arduino.h>
#include <esp_sleep.h>

class SleepController {
  public:
    SleepController(uint8_t wakeUpPin, int wakeUpLevel);
    void goToDeepSleep();
    bool isWakeUpFromDeepSleep();

  private:
    uint8_t wakeUpPin;  
    int wakeUpLevel;   
};

#endif
