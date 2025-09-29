#pragma once
#include <Arduino.h>
#include <Wire.h>

/**
 * Arduino_AD7746 - Minimal Wire-only driver for Analog Devices AD7746 CDC
 * - Default: CIN1+, single-ended, EXCA enabled, CAPCHOP on, continuous mode
 * - No RDY pin required (polls STATUS over I2C)
 * - Read single or averaged capacitance in pF
 *
 * Tested on: AVR (Arduino Uno/Nano) and ESP32
 */
class Arduino_AD7746 {
public:
  // I2C address (AD7746 fixed)
  static constexpr uint8_t I2C_ADDR = 0x48;

  // Register map
  enum : uint8_t {
    REG_STATUS        = 0x00,
    REG_CAP_DATA_H    = 0x01,
    REG_CAP_DATA_M    = 0x02,
    REG_CAP_DATA_L    = 0x03,
    REG_VT_DATA_H     = 0x04,
    REG_VT_DATA_M     = 0x05,
    REG_VT_DATA_L     = 0x06,
    REG_CAP_SETUP     = 0x07,
    REG_VT_SETUP      = 0x08,
    REG_EXC_SETUP     = 0x09,
    REG_CONFIGURATION = 0x0A,
    REG_CAP_DAC_A     = 0x0B,
    REG_CAP_DAC_B     = 0x0C,
    REG_CAP_OFF_H     = 0x0D,
    REG_CAP_OFF_L     = 0x0E,
    REG_CAP_GAIN_H    = 0x0F,
    REG_CAP_GAIN_L    = 0x10
  };

  // STATUS bits
  static constexpr uint8_t STATUS_EXCERR_BIT = 3;
  static constexpr uint8_t STATUS_RDYVT_BIT  = 1;
  static constexpr uint8_t STATUS_RDYCAP_BIT = 0;

  Arduino_AD7746() = default;

  // Bring-up: general-call reset + known-good default config
  // Returns true if device ACKs and STATUS can be read
  bool begin(TwoWire &w = Wire);

  // Quick ping on I2C
  bool testConnection();

  // High-level reads
  bool   readOnce(double &cap_pf);       // blocking single read
  double readAverage(uint16_t N);        // blocking average of N samples

  // Raw accessors
  bool     readStatus(uint8_t &status);
  bool     readCapRaw(uint32_t &code24, uint8_t &status);
  static double codeToPf(uint32_t code24);

  // Direct register writers (advanced use)
  bool setCapSetup(uint8_t v);
  bool setExcSetup(uint8_t v);
  bool setConfiguration(uint8_t v);
  bool setCapDacA(uint8_t v);
  bool setCapDacB(uint8_t v);

private:
  TwoWire *wire_ = &Wire;

  bool    writeReg(uint8_t reg, uint8_t val);
  uint8_t readRegs(uint8_t reg, uint8_t *buf, uint8_t n);

  // Defaults applied by begin():
  // CAP_SETUP = 0x81 (CAPEN=1, CIN1, single-ended, CAPCHOP=1)
  // EXC_SETUP = 0x08 (EXCA on, EXCLVL=00 lowest)
  // CONFIG    = 0x21 (MD=001 continuous, CAPF=100 slower)
  bool applyDefaultConfig();

  // Datasheet general-call reset (addr 0x00, data 0x06)
  void generalCallReset();
};
