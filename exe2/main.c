#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN = 28;
const int LED_R = 4;
volatile int btn_fall;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    btn_fall = 1;
  }
}


int main() {
  stdio_init_all();
  gpio_init(BTN);
  gpio_init(LED_R);
  gpio_set_dir(BTN, GPIO_IN);
  gpio_set_dir(LED_R, GPIO_OUT);
  gpio_pull_up(BTN);

  gpio_set_irq_enabled_with_callback(
    BTN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (btn_fall) {
      gpio_put(LED_R, !gpio_get(LED_R));
      btn_fall = 0;
    }
  }
}
