#include <stdint.h>
#include <stdio.h>

extern void initialise_monitor_handles(void);

enum LED_STATE {
  LED_STATE_WHITE_FLASHING,
  LED_STATE_RED_FLASHING,
  LED_STATE_WHITE_SOLID,
  LED_STATE_OFF,
};

__attribute__((packed)) struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};
struct period {
  struct rgb rgb_val;
  uint32_t period_val;
};
struct pattern {
  struct period on;
  struct period off;
} patterns[] = {
    [LED_STATE_WHITE_FLASHING] =
        {
            .on =
                {
                    .rgb_val =
                        {
                            0xff,
                            0xff,
                            0xff,
                        },
                    .period_val = 1,
                },
            .off =
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
    [LED_STATE_RED_FLASHING] =
        {
            .on =
                {
                    .rgb_val =
                        {
                            0xff,
                            0,
                            0,
                        },
                    .period_val = 1,
                },
            .off =
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
    [LED_STATE_WHITE_SOLID] =
        {
            .on =
                {
                    .rgb_val =
                        {
                            0xff,
                            0xff,
                            0xff,
                        },
                    .period_val = 10,
                },
            .off =
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
};

void EIC_Handler(void) {}
void TC1_Handler(void) {}

static void setup(void) {
  // GPIO hold p-fet on (this may need to be moved to Reset_Handler to be
  // faster)

  // set up SPI peripheral and DMA for SPI

  // initialize GPIO + EIC interrupt for button

  // initialize TC1 for button debounce

  // initialize TC2 for LED pattern timeouts
}

int main(void) {
#if ENABLE_SEMIHOSTING
  initialise_monitor_handles();

  // don't buffer on stdout
  setvbuf(stdout, NULL, _IOLBF, 0);
  printf("🦄 Hello there!\n");
#endif

  setup();

  while (1) {
  };

  return 0;
}
