# Pinouts diagram to use Bruce

## USING CUSTOM BOARD, with SPI

## Waveshare ESP32-C5 n16r8
## TFT ST7789 170x320 RGB

| Device  | SCK   | MISO  | MOSI  | CS    | GDO0/CE | TFT_DC | TFT_RES | TFT_BL |
| ---     | :---: | :---: | :---: | :---: | :---:   | :---:  | :---:   | :---:  |
| Display | 10    | 9     | -     | 23    | ---     | 24     | C5 RST  | 25     |
| SD Card | 10    | 9     | 8     | 6     | ---     | ---    |  ---    | ---    |
| CC1101  | 10    | 9     | 8     | 3*    | 2*      | ---    |  ---    | ---    |
| NRF24   | 10    | 9     | 8     | 3*    | 2*      | ---    |  ---    | ---    |
| W5500   | 10    | 9     | 8     | 3*    | 2*      | ---    |  ---    | ---    |

(*) CC1101, NRF24, W5500 use the same pinouts, need to add a switch on CS and CE/GDO0 to choose which to use.

Pinouts for 5 buttons
| Buttons | GPIO  |
| ---     | :---: |
| Prev    | 1     |
| Sel     | 28    |
| Next    | 0     |



| Device  | RX    | TX    | GPIO  |
| ---     | :---: | :---: | :---: |
| GPS     | 4     | 5     | ---   |
| IR RX   |  ---  | ---   | 26    |
| IR TX   |  ---  | ---   | 3     |
| LED     |  ---  | ---   | 27    |

ESP32-C5 doesn't support USB-OTG, for BadUSB you need to use a CH9329 module

FM Radio, PN532 on I2C, other I2C devices, CH9329, shared with GPS Serial interface.
I2C SDA: 4
I2C SCL: 5

Serial interface to other devices (Flipper)
Serial Tx: 11
Serial Rx: 12
