# Arduino_AD7746

EVAL-CN0552-PMDZ bought from [Digikey](https://www.digikey.ph/en/products/detail/analog-devices-inc/EVAL-CN0552-PMDZ/15848866) is an evaluation board for AD7746 capacitance to digital converter. The official website from [Analog Devices](https://www.analog.com/en/resources/reference-designs/circuits-from-the-lab/cn0552.html) provided different methods to utilize this module such as using Raspberry Pi and The [EVAL-ADICUP3029](https://www.digikey.com/en/products/detail/analog-devices-inc/EVAL-ADICUP3029/7068733) development board with a PC. However, there are no official documentation on how to use the module directly with an Arduino-compatible boards such as Arduino and ESP32 development boards. 

I found an Arduino library at Github [i2cdevlib](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/AD7746). Unfortunately, it doesn't have an ```example``` code on how to use it, and after several attempts of using the said library and tweak different wiring configurations, I cannot get it to work.

With the project deadline coming close, and other projects waiting for me, I decided to use my prior knowledge and experience in using I2C in my previous prototype projects, such as [FPJ Maker](#) and [I2C Relay](#), to create a simple Arduino Library to use this module. I referred to the official [Datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ad7745_7746.pdf), and after some tweak with the code and hardware setup, I was able to get consistent reading with the module using single-ended configuration. 

My setup includes two copper clad board (the one used to make PCBs) fixed at a certain distance between each other and a wire soldered on each of them, and connected to EXCA and CIN1+ pins of the AD7746 module. I used 5V VDD from the Arduino Uno board, and connected the SDA, SCK, and GND pins to the A4(SDA), A5(SCL), and GND pins of the Arduino Uno, respectively.

Based on the specifications in the [Digikey](https://wiki.analog.com/resources/eval/user-guides/circuits-from-the-lab/cn0552) datasheet, the module is capable of using I/O voltage range of 2.7V - 5.5V I2C bus. This means that you can use it directly with Arduino Uno (5V I2C) and ESP32 (3.3V I2C) without the need to use logic level shifters. 

---

## Features

* Simple API: read capacitance once or as an average (in **pF**)
* Configures AD7746 for:

  * Single-ended (CIN1+ vs GND) or differential operation
  * CAPCHOP, CAPDAC, excitation setup
  * Continuous conversion mode
* No RDY pin required — uses **status polling**
* **Tested on Arduino Uno/Nano (AVR) and ESP32**

---

## Installation

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

## Wiring

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



## Tested Hardware

* ✅ Arduino Uno R3


---

## 📜 License

MIT License © 2025 [Jay Four C. Javier](https://github.com/jayfourjavier)

---

## Contribution

Contributions are welcome! Feel free to open issues, submit pull requests, or suggest improvements for broader Arduino/ESP32 support.

## Donation
To support this initiative, feel free to send donations via Gcash (0963-214-6348).

