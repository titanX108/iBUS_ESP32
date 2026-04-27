/*
 * iBUS_ESP32 - BasicRead Example
 *
 * Reads 12 iBus channels from a FlySky receiver (e.g. FSSR-8)
 * and prints them to Serial.
 *
 * Wiring:
 *   Receiver iBus pin --> ESP32 GPIO 23
 *   Receiver GND      --> ESP32 GND
 *   Receiver 5V       --> ESP32 5V (or external BEC)
 */

#include <iBUS_ESP32.h>

// iBUS_ESP32(HardwareSerial, rxPin, txPin, invert)
iBUS_ESP32 ibus(Serial1, 23, -1, false);

void setup() {
    Serial.begin(115200);
    ibus.begin();
    Serial.println("iBUS_ESP32 ready");
}

void loop() {
    if (ibus.read()) {
        Serial.print("CH: ");
        for (int i = 0; i < 12; i++) {
            Serial.print(ibus.getChannel(i));
            Serial.print(" ");
        }
        Serial.println();
    }
}
