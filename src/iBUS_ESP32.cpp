/*
 * MIT License
 * Copyright (c) 2026 [Your Name]
 *
 * See iBUS_ESP32.h for full license text.
 */

#include "iBUS_ESP32.h"

iBUS_ESP32::iBUS_ESP32(HardwareSerial &serial, int rxPin, int txPin, bool invert)
    : _serial(serial),
      _rxPin(rxPin),
      _txPin(txPin),
      _invert(invert),
      _packetCount(0),
      _errorCount(0)
{
    memset(_channels, 0, sizeof(_channels));
}

void iBUS_ESP32::begin() {
    _serial.begin(115200, SERIAL_8N1, _rxPin, _txPin, _invert);
}

bool iBUS_ESP32::read() {
    if (_serial.available() < IBUS_BUFFSIZE) return false;

    // Discard bytes until we find the header
    if (_serial.peek() != IBUS_HEADER1) {
        _serial.read();
        return false;
    }

    _serial.readBytes(_buf, IBUS_BUFFSIZE);

    // Verify header
    if (_buf[0] != IBUS_HEADER1 || _buf[1] != IBUS_HEADER2) {
        _errorCount++;
        return false;
    }

    // Verify checksum
    uint16_t checksum = 0xFFFF;
    for (int i = 0; i < 30; i++) {
        checksum -= _buf[i];
    }
    uint16_t rxChecksum = _buf[30] | (_buf[31] << 8);
    if (checksum != rxChecksum) {
        _errorCount++;
        return false;
    }

    // Parse 14 channels — 2 bytes each, little-endian, 12-bit masked
    for (int i = 0; i < IBUS_NUM_CHANNELS; i++) {
        _channels[i] = (_buf[2 + i * 2] | (_buf[3 + i * 2] << 8)) & 0x0FFF;
    }

    _packetCount++;
    return true;
}

uint16_t iBUS_ESP32::getChannel(uint8_t ch) const {
    if (ch >= IBUS_NUM_CHANNELS) return 0;
    return _channels[ch];
}

uint16_t iBUS_ESP32::getPacketCount() const {
    return _packetCount;
}

uint16_t iBUS_ESP32::getErrorCount() const {
    return _errorCount;
}
