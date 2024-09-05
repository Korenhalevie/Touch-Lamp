#include <Arduino.h>
#include "WS2812.h"

#define LED_PIN GPIO_NUM_18
#define RMT_CHANNEL RMT_CHANNEL_0

WS2812 led(LED_PIN, RMT_CHANNEL);
// WS2812 led(LED_PIN, RMT_CHANNEL, 255, 0, 0); // red
// WS2812 led(LED_PIN, RMT_CHANNEL, 0, 255, 0); // green
// WS2812 led(LED_PIN, RMT_CHANNEL, 0, 0, 255); // blue

// uint8_t currentRed, currentGreen, currentBlue; // uncomment this line if you want to keep the same color

void setup() {
    led.setupRMT();
    // led.getColor(currentRed, currentGreen, currentBlue); // get current color, uncomment this line if you want to keep the same color
}

void loop() {

  // to get random color LED, comment out the following 3 lines if you want to keep the same color
    uint8_t currentRed = random(0, 256);
    uint8_t currentGreen = random(0, 256);
    uint8_t currentBlue = random(0, 256);
    
    led.setColor(currentRed, currentGreen, currentBlue);
    led.lightPixels();
    delay(500);


    led.setColor(0, 0, 0);
    led.lightPixels();
    delay(500);
}
