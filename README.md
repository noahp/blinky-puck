# blinky-puck

Yet another blinky light gizmo.

Features:

- one (or more?) ws2812b LEDs
- samd10 (cheapest arm i could find on
  [digikey](https://www.digikey.com/en/products/detail/microchip-technology/ATSAMD10D13A-MUT/5226468))
- single tact button (white flashing, red flashing, white solid, off)
- usb c [recharcheable
  battery](https://www.amazon.com/Rechargeable-Batteries-Lithium-Button-CR2032/dp/B07S51KBYY?th=1)
  (onboard charging IC)

Notes:

- high side cutoff (p-fet) for all components, tied off, when shorted micro
  boots and then holds power on until it's turned off?
- check low power oscillators on the samd10 (see [samd20
  reference](https://blog.thea.codes/understanding-the-sam-d21-clocks/))
- check coin cell capacity...looks like LIR2032's are about 40mAh
- might need some capacitance on Vbat to not brown out the cell... might be
  taking ~1C off it if the ws2812b goes full on

## software

`LED_STATE`:

- white flashing
- red flashing
- white solid
- off (system shut down)

Use SPI DMA, GPIO interrupt, and 2 timers.

### system startup

1. initial button press shorts p-fet to enable power momentarily to micro +
   ws2812 VSS, micro boots and holds p-fet on until `LED_STATE` goes to off.

### button control

1. button press triggers GPIO interrupt (interrupt triggers on high + low)
2. GPIO ISR, sample GPIO level, if button=pressed, start timer
   peripheral for debounce, otherwise clear and disable timer IRQ
3. if debounce timer IRQ trips, advance `LED_STATE`. next timer ISR will select
   the correct mode (TODO or update immediately? need to stop interrupts and
   wait for SPI to finish and then update everything)

### ws2812b control

1. on system init set up SPI for DMA
2. patterns should be defined in an arrary of structs defining ON/OFF periods
   and rgb values to write
3. write ON rgb value and set timer for timeout on the ON period
4. when timer ISR fires, write OFF rgb value and set timer for timeout on the
   OFF period
5. when timer ISR fires, write ON rgb value and set timer for timeout on the ON
   period. repeat from 3.
