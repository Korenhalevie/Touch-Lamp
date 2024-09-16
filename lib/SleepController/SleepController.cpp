#include "SleepController.h"

SleepController::SleepController(uint8_t wakeUpPin, int wakeUpLevel)
  : wakeUpPin(wakeUpPin), wakeUpLevel(wakeUpLevel) {}

void SleepController::goToDeepSleep() {
  esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeUpPin, wakeUpLevel);
  Serial.println("Entering deep sleep...");
  esp_deep_sleep_start();
}

bool SleepController::isWakeUpFromDeepSleep() {
  return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0;
}
