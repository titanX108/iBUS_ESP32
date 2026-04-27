/*
 * MIT License
 * Copyright (c) 2026 [Your Name]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef iBUS_ESP32_H
#define iBUS_ESP32_H

#include <Arduino.h>

#define IBUS_BUFFSIZE    32
#define IBUS_HEADER1     0x20
#define IBUS_HEADER2     0x40
#define IBUS_NUM_CHANNELS 14

class iBUS_ESP32 {
public:
    /**
     * @param serial  HardwareSerial port to use (e.g. Serial1)
     * @param rxPin   RX pin number
     * @param txPin   TX pin number (-1 if unused)
     * @param invert  Invert signal (false for standard iBus TTL)
     */
    iBUS_ESP32(HardwareSerial &serial, int rxPin, int txPin = -1, bool invert = false);

    /** Call once in setup() */
    void begin();

    /**
     * Call in loop(). Returns true when a valid packet is parsed.
     * Channel values are updated only on true return.
     */
    bool read();

    /**
     * Get channel value. ch is 0-indexed (0 = CH1).
     * Returns value in range 1000-2000, or 0 if ch is out of range.
     */
    uint16_t getChannel(uint8_t ch) const;

    /** Returns number of valid packets received (wraps at 65535) */
    uint16_t getPacketCount() const;

    /** Returns number of checksum failures since start */
    uint16_t getErrorCount() const;

private:
    HardwareSerial &_serial;
    int             _rxPin;
    int             _txPin;
    bool            _invert;

    uint8_t  _buf[IBUS_BUFFSIZE];
    uint16_t _channels[IBUS_NUM_CHANNELS];
    uint16_t _packetCount;
    uint16_t _errorCount;
};

#endif // iBUS_ESP32_H
