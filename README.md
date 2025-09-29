# Arduino_AD7746

Arduino and ESP32 driver for the **Analog Devices AD7746** capacitance-to-digital converter (CDC).
Reads single-ended or differential capacitance with sub-femtofarad resolution over I²C.

---

## ✨ Features

* Simple API: read capacitance once or as an average (in **pF**)
* Configures AD7746 for:

  * Single-ended (CIN1+ vs GND) or differential operation
  * CAPCHOP, CAPDAC, excitation setup
  * Continuous conversion mode
* No RDY pin required — uses **status polling**
* **Tested on Arduino Uno/Nano (AVR) and ESP32**

---

## 📦 Installation

### Arduino IDE

1. Download ZIP from [Releases](https://github.com/jayfourjavier/Arduino_AD7746).
2. In Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**

### PlatformIO

Add to `platformio.ini`:

```ini
lib_deps =
  https://github.com/jayfourjavier/Arduino_AD7746.git
```

---

## 🔌 Wiring

| AD7746 Pin | Connects to                                                  |
| ---------- | ------------------------------------------------------------ |
| **VDD**    | 3.3V or 5V (depending on module)                             |
| **GND**    | GND                                                          |
| **SCL**    | Arduino `A5` (Uno/Nano) or ESP32 `GPIO22` (default SCL)      |
| **SDA**    | Arduino `A4` (Uno/Nano) or ESP32 `GPIO21` (default SDA)      |
| **EXCA**   | One plate of sensor capacitor                                |
| **CIN1+**  | Other plate of sensor capacitor                              |
| **CIN1-**  | Leave floating for single-ended, or connect for differential |
| **RDY**    | (Optional) not required — library polls status               |

⚠️ Don’t forget I²C pull-up resistors (usually already on AD7746 breakout boards).

---

## 🖥️ Example

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Arduino_AD7746.h>

Arduino_AD7746 cdc;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!cdc.testConnection()) {
    Serial.println("AD7746 not found at 0x48");
    while (1);
  }

  if (!cdc.begin()) {
    Serial.println("Init failed");
    while (1);
  }

  Serial.println("AD7746 ready!");
}

void loop() {
  double avg = cdc.readAverage(20);
  Serial.print("AVG(20): ");
  Serial.print(avg, 6);
  Serial.println(" pF");
  delay(1000);
}
```

---

## 🛠️ Tested Hardware

* ✅ Arduino Uno R3
* ✅ Arduino Nano
* ✅ ESP32-DevKitC
* ❓ Other MCUs may work (e.g. STM32, Teensy) but not tested yet

---

## 📜 License

MIT License © 2025 [Jay Four C. Javier](https://github.com/jayfourjavier)

---

