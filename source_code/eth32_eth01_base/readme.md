# WT32-ETH01 learnings

## Programmer pins FTDI programmer
- 3.3V --> 3.3V
- GND  --> GND
- DTR  --> IO0
- RTS  --> EN
- RX   --> TX
- TX   --> RX

## Pins I can use for WLED safely
Pins: 2, 4, 5, 12, 17 --> So yes, only these 5!! With a maximum of 2 16x16 screens per pin, this amounts to enough pins for the:
- NM Pixel Mini with 1 screen
- NM Pixel Medium with 4 screens

## Power pins
### Barrel jack

| Pin       |     |
| --------- | --- | 
| short pin | +5v |
| long pin  | GND |

### GX16 power

| Pin       |     |
| --------- | --- | 
| 1 | +5v |
| 2  | GND |

# ESP32-WROOM Learnings

## How to connect wired network to ESP module

| W5500 Pin | ESP32-WROOM PIN | ESP32 ID |
| --------- | --------------- | -------- |
| MISO | SCK | GPIO14 |
| SCLK | MISO | GPIO12 |
| MOSI | MOSI | GPIO13 |
| CS   | SS | GPIO15 |
| INT (optional) | SCL | GPIO22 |
| RST (optional) | JTAG | GPIO23 |
| 3v3 | 3v3 | 3.3v |
| GND | GND | GND |

## Safe pins to use for LED outputs
Very safe Pins:
- GPIO16
- GPIO17
- GPIO18
- GPIO19
- GPIO21
- GPIO25
- GPIO26
- GPIO27
- GPIO32
- GPIO33

If you need more, consider these:
- GPIO2 - connected to on-board LED, must be left floating or LOW to enter flashing mode
- GPIO4 - Has weak pull up, and should work fine
- GPIO5 - Weak pull up and outputs PWM signal at boot ... weird!