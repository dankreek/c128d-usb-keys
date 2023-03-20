# C128D USB Keys Teensy implementation

This implementation uses the following external parts:
  * [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) or [Teensy 4.0](https://www.pjrc.com/store/teensy40.html) with USB host functionality connected.
  * [Sparkfun Logic Level Converter - Bi-Directional](https://www.sparkfun.com/products/12009)
  * [Shift Register 8-Bit - SN74HC595](https://www.sparkfun.com/products/13699)
  * Mitel MT8812AE
    * The PDIP packaging of this chip is no longer available so I had to get one on e-bay, but surface-mount options are still available

All output pin definitions are defined in [teensy_usb_c128d.hpp](./teensy_usb_c128d.hpp#L22-L35)


## Shift register to MT8812 connection

| SN74HC595 | MT8812AE  |
| --------- | --------- |
| Qa (15)   | AX0 (5)   |
| Qb (1)    | AX1 (22)  |
| Qc (2)    | AX2 (23)  |
| Qd (3)    | AX3 (4)   |
| Qe (4)    | AY0 (24)  |
| Qf (5)    | AY1 (25)  |
| Qg (6)    | AY2 (2)   |
| Qh (7)    | DATA (38) |