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
