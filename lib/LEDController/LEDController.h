#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "WS2812.h"

class LEDController {
  private:
    uint8_t ledPin;
    rmt_channel_t rmtChannel;
    WS2812 led;

  public:
    LEDController(uint8_t pin, rmt_channel_t channel);
    void setup();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void turnOff();
    void lightUp();
    void applyState(bool state);
};

#endif
