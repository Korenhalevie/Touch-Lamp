#include <Arduino.h>
#include "WS2812.h"

#define LED_PIN GPIO_NUM_18
#define TOUCH_PIN GPIO_NUM_4
#define RMT_CHANNEL RMT_CHANNEL_0


int touchValue;
int ledState = LOW; // start with LED off
bool isTouched = false; // flag to check if the touch sensor is touched


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
    touchValue = touchRead(TOUCH_PIN);


    if (touchValue < 40) { // Touch sensor is activated
        if (!isTouched) { 
            ledState = !ledState; // Toggle the LED state
            
            if (ledState) { // If LED is on
                uint8_t currentRed = random(0, 256);
                uint8_t currentGreen = random(0, 256);
                uint8_t currentBlue = random(0, 256);
            } else { // If LED is off
                led.setColor(0, 0, 0);
            }
            led.lightPixels(); 
            isTouched = true; 
    } else {
        isTouched = false;
    }
}
}