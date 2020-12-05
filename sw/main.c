#include <stdint.h>
#include <stdio.h>

#include "lib/samd10/include/samd10d13am.h"

extern void initialise_monitor_handles(void);

static enum LED_STATE {
  LED_STATE_WHITE_FLASHING,
  LED_STATE_RED_FLASHING,
  LED_STATE_WHITE_SOLID,
  LED_STATE_OFF,
} current_led_state = LED_STATE_WHITE_FLASHING;

__attribute__((packed)) struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct period {
  struct rgb rgb_val;
  uint32_t period_val;
};

static struct pattern {
  struct period periods[2]; //< on then off
} patterns[] = {
    [LED_STATE_WHITE_FLASHING] =
        {
            .periods =
                {
                    {
                        .rgb_val =
                            {
                                0xff,
                                0xff,
                                0xff,
                            },
                        .period_val = 1,
                    },
                    {
                        .rgb_val =
                            {
                                0,
                                0,
                                0,
                            },
                        .period_val = 20,
                    },
                },
        },
    [LED_STATE_RED_FLASHING] =
        {
            .periods =
                {
                    {
                        .rgb_val =
                            {
                                0xff,
                                0,
                                0,
                            },
                        .period_val = 1,
                    },
                    {
                        .rgb_val =
                            {
                                0,
                                0,
                                0,
                            },
                        .period_val = 20,
                    },
                },
        },
    [LED_STATE_WHITE_SOLID] =
        {
            .periods =
                {
                    {
                        .rgb_val =
                            {
                                0xff,
                                0xff,
                                0xff,
                            },
                        .period_val = 10,
                    },
                    {
                        .rgb_val =
                            {
                                0xff,
                                0xff,
                                0xff,
                            },
                        .period_val = 10,
                    },
                },
        },
};

static int read_button_gpio(void) {
  // TODO implement
  return 0;
}

static void set_rgb_pattern(struct rgb rgb_val) {
  (void)rgb_val;
  // TODO kick off SPI DMA
}

static void set_TC2_timeout_and_start(uint32_t timeout) {
  (void)timeout;
  // TODO implement
}

void EIC_Handler(void) {
  // sample GPIO
  if (!read_button_gpio()) {
    // TODO set up TC1 for button debounce
  } else {
    // TODO clear and reset TC1, we failed to debounce 😔
  }
}

void TC1_Handler(void) {
  current_led_state++;

  if (current_led_state == LED_STATE_OFF) {
    // we done here, disable GPIO for p-fet
    // TODO
    while (1)
      ;
  }

  // TODO disable TC1
}

#define ARRAY_LEN(x) sizeof(x) / sizeof(*x)
void TC2_Handler(void) {
  static size_t current_period_index = 0;

  // apply current period settings
  struct period *current_period =
      &patterns[current_led_state].periods[current_period_index];

  set_rgb_pattern(current_period->rgb_val);
  set_TC2_timeout_and_start(current_period->period_val);

  // move to next period
  current_period_index = (current_period_index + 1u) %
                         ARRAY_LEN(patterns[LED_STATE_WHITE_FLASHING].periods);
}

static void setup(void) {
  // TODO GPIO hold p-fet on (this may need to be moved to Reset_Handler to be
  // faster)

  // TODO set up SPI peripheral and DMA for SPI

  // TODO initialize GPIO + EIC interrupt for button

  // TODO initialize TC1 for button debounce

  // TODO initialize TC2 for LED pattern timeouts
}

int main(void) {
#if ENABLE_SEMIHOSTING
  initialise_monitor_handles();

  // don't buffer on stdout
  setvbuf(stdout, NULL, _IOLBF, 0);
  printf("🦄 Hello there!\n");
#endif

  setup();

  // everything happens in the interrupts, stall the cpu
  while (1) {
    __WFI();
  };

  return 0;
}
