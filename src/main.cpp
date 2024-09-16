#include "LEDController.h"
#include "TouchController.h"
#include "SleepController.h"

// Constants for pins and thresholds
#define TOUCH_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_18
#define RMT_CHANNEL RMT_CHANNEL_0
#define THRESHOLD 20000
#define RELEASETHRESHOLD 7

// Use RTC memory to store the LED state across deep sleep cycles
RTC_DATA_ATTR bool ledState = false;  // Tracks whether the LED is on or off
RTC_DATA_ATTR bool isTouchHeld = false;

// Instantiate SleepController and TouchController globally
SleepController sleepController(TOUCH_PIN, 1);  // Wake-up pin 4, HIGH level
TouchController touchController(TOUCH_PIN, THRESHOLD, RELEASETHRESHOLD);

// Instantiate LEDController globally
LEDController ledController(LED_PIN, RMT_CHANNEL);

void setup() {
    Serial.begin(115200);
    
    // Initialize the LED controller (RMT setup)
    ledController.setup();  

    // Handle waking up from deep sleep
    if (sleepController.isWakeUpFromDeepSleep()) {
        Serial.println("Woke up from deep sleep");
        isTouchHeld = false;  // Reset to avoid sleep immediately
        
        // Reapply the saved LED state (on or off) after waking up
        Serial.println(ledState ? "LED ON" : "LED OFF");
        ledController.applyState(ledState);
    } else {
        // On initial boot, turn off the LED
        Serial.println("Initial boot: LED OFF");
        ledController.applyState(false);  // Set LED off on initial boot
    }
}

void loop() {
    // Continuously check for touch events
    if (touchController.detectTouch()) {
        if (!isTouchHeld) {  // Only handle the first touch event
            Serial.println("Touch detected!");

            // Toggle the global LED state
            ledState = !ledState;
            ledController.applyState(ledState);  // Apply the new LED state
            isTouchHeld = true;
        }
    } else if (isTouchHeld) {
        // If touch was previously held but now released
        Serial.println("Touch released, entering deep sleep...");
        isTouchHeld = false;

        // Save the LED state and enter deep sleep
        Serial.println("Saving state and entering deep sleep...");
        sleepController.goToDeepSleep();  // Enter deep sleep after release
    }
}
