#include "LEDController.h"

LEDController::LEDController(uint8_t pin, rmt_channel_t channel) 
    : ledPin(pin), rmtChannel(channel), led(static_cast<gpio_num_t>(pin), channel) {}

void LEDController::setup() {
    led.setupRMT();  // Only perform RMT setup here, LED is already turned off in the constructor
}

void LEDController::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    led.setColor(red, green, blue);
}

void LEDController::turnOff() {
    setColor(0, 0, 0); 
}

void LEDController::lightUp() {
    led.lightPixels();  // This will display and light the set color
}

void LEDController::applyState(bool state) {
    if (state) {
        setColor(random(0, 256), random(0, 256), random(0, 256));
    } else {
        turnOff();  // Turn off the LED when toggling off
    }

    lightUp();
}
