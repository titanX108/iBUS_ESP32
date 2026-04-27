# iBUS_ESP32

A lightweight Arduino library for reading FlySky iBus RC receiver data on ESP32. No external components required.

## Tested Hardware
- ESP32
- FlySky FSSR-8 receiver with iBus enabled

## Wiring

| Receiver | ESP32 |
|----------|-------|
| iBus signal | GPIO 23 (or any RX-capable pin) |
| GND | GND |
| 5V | 5V |

## Installation

1. Download this repository as a ZIP
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded ZIP

## Usage

```cpp
#include <iBUS_ESP32.h>

iBUS_ESP32 ibus(Serial1, 23, -1, false); // serial, rxPin, txPin, invert

void setup() {
    Serial.begin(115200);
    ibus.begin();
}

void loop() {
    if (ibus.read()) {
        for (int i = 0; i < 12; i++) {
            Serial.print(ibus.getChannel(i)); // returns 1000-2000
            Serial.print(" ");
        }
        Serial.println();
    }
}
```

## API

| Method | Description |
|--------|-------------|
| `begin()` | Initialize serial at 115200 baud |
| `read()` | Call in loop(). Returns true on valid packet |
| `getChannel(n)` | Get channel n (0-indexed), returns 1000–2000 |
| `getPacketCount()` | Total valid packets received |
| `getErrorCount()` | Total checksum failures |

## Channel Layout (FSSR-8)

| Index | Channel | Default |
|-------|---------|---------|
| 0 | Right stick H | 1500 |
| 1 | Right stick V | 1500 |
| 2 | Left stick V (throttle) | 1000 |
| 3 | Left stick H | 1500 |
| 4–11 | Aux / switches | varies |

## License

MIT License — see [LICENSE](LICENSE)
