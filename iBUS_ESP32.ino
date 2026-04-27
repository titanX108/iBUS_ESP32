/*
* MIT License

* Copyright (c) 2026 [Your Name]

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#define IBUS_BUFFSIZE 32
#define IBUS_HEADER1 0x20
#define IBUS_HEADER2 0x40

HardwareSerial ibusSerial(1);
uint8_t ibusBuffer[IBUS_BUFFSIZE];
uint16_t channels[14];

bool readIbus() {
  if (ibusSerial.available() < IBUS_BUFFSIZE) return false;
  
  if (ibusSerial.peek() != IBUS_HEADER1) {
    ibusSerial.read();
    return false;
  }
  
  ibusSerial.readBytes(ibusBuffer, IBUS_BUFFSIZE);
  
  if (ibusBuffer[0] != IBUS_HEADER1 || ibusBuffer[1] != IBUS_HEADER2) return false;
  
  uint16_t checksum = 0xFFFF;
  for (int i = 0; i < 30; i++) checksum -= ibusBuffer[i];
  uint16_t rxChecksum = ibusBuffer[30] | (ibusBuffer[31] << 8);
  if (checksum != rxChecksum) return false;
  
  for (int i = 0; i < 14; i++) {
    channels[i] = (ibusBuffer[2 + i * 2] | (ibusBuffer[3 + i * 2] << 8)) & 0x0FFF;
  }
  
  return true;
}

void setup() {
  Serial.begin(115200);
  ibusSerial.begin(115200, SERIAL_8N1, 23, -1, false); // RX=23, no invert
}

void loop() {
  if (readIbus()) {
    Serial.print("CH: ");
    for (int i = 0; i < 12; i++) {
      Serial.print(channels[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
