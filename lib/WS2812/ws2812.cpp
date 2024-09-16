#include "WS2812.h"

uint32_t WS2812::t0h_ticks = 0;
uint32_t WS2812::t1h_ticks = 0;
uint32_t WS2812::t0l_ticks = 0;
uint32_t WS2812::t1l_ticks = 0;

WS2812::WS2812(gpio_num_t pin, rmt_channel_t channel, uint8_t red, uint8_t green, uint8_t blue) 
    : ledPin(pin), rmtChannel(channel), red(red), green(green), blue(blue) {}

void WS2812::setupRMT() {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(ledPin, rmtChannel);
    config.clk_div = 2;
    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    uint32_t counter_clk_hz = 0;
    rmt_get_counter_clock(rmtChannel, &counter_clk_hz);
    float ratio = (float)counter_clk_hz / 1e9;

    t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS);
    t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS);
    t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS);
    t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS);

    rmt_translator_init(rmtChannel, ws2812_rmt_adapter);
}

void WS2812::lightPixels() {
    uint8_t rgbPixels[3] = {green, red, blue};
    rmt_write_sample(rmtChannel, rgbPixels, 3, true);
    rmt_wait_tx_done(rmtChannel, pdMS_TO_TICKS(100));
}

void WS2812::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

void WS2812::getColor(uint8_t &red, uint8_t &green, uint8_t &blue) {
    red = this->red;
    green = this->green;
    blue = this->blue;
}

void WS2812::ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num) {
    rmt_item32_t bit0 = {{{ t0h_ticks, 1, t0l_ticks, 0 }}};
    rmt_item32_t bit1 = {{{ t1h_ticks, 1, t1l_ticks, 0 }}};
    size_t size = 0;
    size_t num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;

    while (size < src_size && num < wanted_num) {
        for (int i = 0; i < 8; i++) {
            if (*psrc & (1 << (7 - i))) {
                pdest->val = bit1.val;
            } else {
                pdest->val = bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }

    *translated_size = size;
    *item_num = num;
}
