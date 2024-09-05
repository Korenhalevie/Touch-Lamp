#ifndef WS2812_H
#define WS2812_H

#include <driver/rmt.h>
#include <Arduino.h>

#define WS2812_T0H_NS (400)
#define WS2812_T0L_NS (850)
#define WS2812_T1H_NS (800)
#define WS2812_T1L_NS (450)

class WS2812 {
public:
    WS2812(gpio_num_t pin, rmt_channel_t channel, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);
    
    void setupRMT();
    void lightPixels();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void getColor(uint8_t &red, uint8_t &green, uint8_t &blue);
    
private:
    gpio_num_t ledPin;
    rmt_channel_t rmtChannel;
    
    uint8_t red, green, blue;
    
    static uint32_t t0h_ticks;
    static uint32_t t1h_ticks;
    static uint32_t t0l_ticks;
    static uint32_t t1l_ticks;
    
    static void ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num);
};

#endif
