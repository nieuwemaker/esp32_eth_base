# Base code for the WROOM ESP32
This is where I work from now.

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