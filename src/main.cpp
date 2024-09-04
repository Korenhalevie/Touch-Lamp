#include <driver/rmt.h>
#include <Arduino.h>

#define LED_PIN GPIO_NUM_18
#define RMT_CHANNEL RMT_CHANNEL_0

int number;
// WS2812 times by the datasheet
#define WS2812_T0H_NS (400)
#define WS2812_T0L_NS (850)
#define WS2812_T1H_NS (800)
#define WS2812_T1L_NS (450)

static uint32_t t0h_ticks = 0; // static to keep the value it gets on the function
static uint32_t t1h_ticks = 0;
static uint32_t t0l_ticks = 0;
static uint32_t t1l_ticks = 0;

void ws2812_rmt_adapter(const void *src, rmt_item32_t *dest, size_t src_size, size_t wanted_num, size_t *translated_size, size_t *item_num) {
    const rmt_item32_t bit0 = {{{ t0h_ticks, 1, t0l_ticks, 0 }}}; // bit 0
    const rmt_item32_t bit1 = {{{ t1h_ticks, 1, t1l_ticks, 0 }}}; // bit 1
    size_t size = 0;
    size_t num = 0;
    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;
    
    while (size < src_size && num < wanted_num) {
        for (int i = 0; i < 8; i++) {
            if (*psrc & (1 << (7 - i))) {
                pdest->val = bit1.val;  // send bit 1
            } else {
                pdest->val = bit0.val;  // send bit 0
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

void setupRMT() {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(LED_PIN, RMT_CHANNEL);
    config.clk_div = 2;  // clocl divide
    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    
    uint32_t counter_clk_hz = 0;
    rmt_get_counter_clock(RMT_CHANNEL, &counter_clk_hz);
    float ratio = (float)counter_clk_hz / 1e9;

    t0h_ticks = (uint32_t)(ratio * WS2812_T0H_NS);
    t0l_ticks = (uint32_t)(ratio * WS2812_T0L_NS);
    t1h_ticks = (uint32_t)(ratio * WS2812_T1H_NS);
    t1l_ticks = (uint32_t)(ratio * WS2812_T1L_NS);

   
    rmt_translator_init(RMT_CHANNEL, ws2812_rmt_adapter);
}

void sendPixels(uint8_t *pixels) {
    uint8_t rgbPixels[3] = {pixels[1], pixels[0], pixels[2]};  // to make it RGB
    rmt_write_sample(RMT_CHANNEL, rgbPixels, 3, true);
    rmt_wait_tx_done(RMT_CHANNEL, pdMS_TO_TICKS(100));
}


void setup() {
    setupRMT();
    randomSeed(analogRead(0));
}

void loop() {
  
    uint8_t pixels[3] = {random(0, 256), random(0, 256), random(0, 256)};
    sendPixels(pixels);      
    delay(500);

    uint8_t pixelsOff[3] = {0, 0, 0}; 
    sendPixels(pixelsOff);
    delay(500);
}
