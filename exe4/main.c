#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

volatile int btn_r_pressed;
volatile int btn_g_unpressed;
const int BTN_R = 28;
const int BTN_G = 26;
const int LED_R = 4;
const int LED_G = 6;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4 && gpio == BTN_R) {
    btn_r_pressed = 1;
  }
  if (events == 0x8 && gpio == BTN_G) {
    btn_g_unpressed = 1;
  }
}

int main() {
  stdio_init_all();
  
  gpio_init(BTN_R);
  gpio_init(BTN_G);
  gpio_init(LED_R);
  gpio_init(LED_G);

  gpio_set_dir(BTN_R, GPIO_IN);
  gpio_set_dir(BTN_G, GPIO_IN);
  gpio_set_dir(LED_R, GPIO_OUT);
  gpio_set_dir(LED_G, GPIO_OUT);
  
  gpio_pull_up(BTN_R);
  gpio_pull_up(BTN_G);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_R, GPIO_IRQ_EDGE_FALL, true,
    &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_G, GPIO_IRQ_EDGE_RISE, true);


  while (true) {
    if (btn_r_pressed) {
      gpio_put(LED_R, !gpio_get(LED_R));
      btn_r_pressed = 0;
    }
    if (btn_g_unpressed) {
      gpio_put(LED_G, !gpio_get(LED_G));
      btn_g_unpressed = 0;
    }
  }
}
