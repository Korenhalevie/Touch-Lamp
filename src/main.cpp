#include <Arduino.h>
#include "WS2812.h"
#include "driver/touch_pad.h" // to wake up from touch

#define LED_PIN GPIO_NUM_18
#define TOUCH_PIN T0 // T0 = GPIO 4
#define RMT_CHANNEL RMT_CHANNEL_0
#define TOUCH_THRESHOLD 20

// because we are in deep sleep, ram is off, so we need to use RTC_DATA_ATTR that stores the variable in RTC memory
RTC_DATA_ATTR bool ledState = LOW;
RTC_DATA_ATTR bool isTouchHeld = false;

WS2812 led(LED_PIN, RMT_CHANNEL);

void goToDeepSleep()
{
  touchAttachInterrupt(TOUCH_PIN, [] {}, TOUCH_THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  esp_deep_sleep_start();
}

void setup()
{
  led.setupRMT();

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TOUCHPAD)
  { // check if touchpin held and it wakes up
    if (!isTouchHeld)
    {
      isTouchHeld = true;
      ledState = !ledState;
      if (ledState)
      {
        uint8_t currentRed = random(0, 256);
        uint8_t currentGreen = random(0, 256);
        uint8_t currentBlue = random(0, 256);
        led.setColor(currentRed, currentGreen, currentBlue);
      }
      else
      {
        led.setColor(0, 0, 0);
      }
      led.lightPixels();
    }
  }
  else
  { // the system starts ehre
    led.setColor(0, 0, 0);
    led.lightPixels();
  }

  bool touchReleased = false;
  while (!touchReleased)
  {
    if (touchRead(TOUCH_PIN) > 20)
    {
      touchReleased = true;

      goToDeepSleep();
    }
  }
}

void loop()
{
}
