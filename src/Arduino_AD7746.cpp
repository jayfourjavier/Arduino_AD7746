#include "Arduino_AD7746.h"

bool Arduino_AD7746::begin(TwoWire &w) {
  wire_ = &w;
  generalCallReset();
  if (!applyDefaultConfig()) return false;
  uint8_t st;
  return readStatus(st);
}

bool Arduino_AD7746::testConnection() {
  wire_->beginTransmission(I2C_ADDR);
  return (wire_->endTransmission() == 0);
}

bool Arduino_AD7746::setCapSetup(uint8_t v)       { return writeReg(REG_CAP_SETUP, v); }
bool Arduino_AD7746::setExcSetup(uint8_t v)       { return writeReg(REG_EXC_SETUP, v); }
bool Arduino_AD7746::setConfiguration(uint8_t v)  { return writeReg(REG_CONFIGURATION, v); }
bool Arduino_AD7746::setCapDacA(uint8_t v)        { return writeReg(REG_CAP_DAC_A, v); }
bool Arduino_AD7746::setCapDacB(uint8_t v)        { return writeReg(REG_CAP_DAC_B, v); }

bool Arduino_AD7746::readOnce(double &cap_pf) {
  uint32_t raw; uint8_t st;
  if (!readCapRaw(raw, st)) return false;
  cap_pf = codeToPf(raw);
  return true;
}

double Arduino_AD7746::readAverage(uint16_t N) {
  double sum = 0.0;
  uint16_t good = 0;
  for (uint16_t i = 0; i < N; ) {
    double c;
    if (readOnce(c)) { sum += c; good++; i++; }
    else { delay(2); }
  }
  return (good == N) ? (sum / good) : NAN;
}

bool Arduino_AD7746::readStatus(uint8_t &status) {
  return readRegs(REG_STATUS, &status, 1) == 1;
}

bool Arduino_AD7746::readCapRaw(uint32_t &code24, uint8_t &status) {
  // Poll until CAP data ready (RDYCAP == 0)
  do {
    if (readRegs(REG_STATUS, &status, 1) != 1) return false;
  } while (status & (1 << STATUS_RDYCAP_BIT));

  uint8_t d[3];
  if (readRegs(REG_CAP_DATA_H, d, 3) != 3) return false;
  code24 = ((uint32_t)d[0] << 16) | ((uint32_t)d[1] << 8) | d[2];
  return true;
}

double Arduino_AD7746::codeToPf(uint32_t code24) {
  const double mid = 8388608.0;          // 0x800000 = 2^23 -> 0 pF
  const double lsb = 4.096 / 8388608.0;  // pF per code (±4.096 pF span)
  return (double((int32_t)code24) - mid) * lsb;
}

bool Arduino_AD7746::writeReg(uint8_t reg, uint8_t val) {
  wire_->beginTransmission(I2C_ADDR);
  wire_->write(reg);
  wire_->write(val);
  return wire_->endTransmission() == 0;
}

uint8_t Arduino_AD7746::readRegs(uint8_t reg, uint8_t *buf, uint8_t n) {
  wire_->beginTransmission(I2C_ADDR);
  wire_->write(reg);
  if (wire_->endTransmission(false) != 0) return 0; // repeated start
  uint8_t got = wire_->requestFrom(I2C_ADDR, n);
  for (uint8_t i = 0; i < got; i++) buf[i] = wire_->read();
  return got;
}

bool Arduino_AD7746::applyDefaultConfig() {
  // CAP SETUP: CAPEN=1, CIN1, single-ended, CAPCHOP=1
  if (!writeReg(REG_CAP_SETUP, 0x81)) return false;
  // EXC SETUP: EXCA on, non-inverted, EXCLVL=00 (lowest)
  if (!writeReg(REG_EXC_SETUP, 0x08)) return false;
  // CONFIG: MD=001 (continuous), CAPF=100 (slower -> less noise)
  if (!writeReg(REG_CONFIGURATION, 0x21)) return false;
  return true;
}

void Arduino_AD7746::generalCallReset() {
  wire_->beginTransmission(0x00);  // general call address
  wire_->write(0x06);              // reset command
  wire_->endTransmission();
  delay(2);
}
