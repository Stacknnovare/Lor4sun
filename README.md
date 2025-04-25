# ☀️ LoR4Sun — Photovoltaic System Monitoring via LoRaWAN and IoT

LoR4Sun is an open-source project developed as a Computer Engineering capstone (TCC) to enable efficient, low-power, long-range monitoring of photovoltaic systems in both residential and industrial environments. Leveraging LoRaWAN and IoT, this project ensures real-time visibility, extended system lifespan, and early fault detection.

---

## 📌 Features

- 🌐 Real-time monitoring of solar panel parameters (voltage, current, temperature, light, humidity)
- 📶 LoRaWAN-based long-range communication
- 🔋 Low energy consumption with ESP32 + LoRa modules
- 📊 Custom web dashboard for data visualization and alerting
- 🔧 Fault detection for panels, wiring, and batteries

---

## 📦 Hardware Components

| Component        | Description                              |
|------------------|------------------------------------------|
| ESP32            | Main microcontroller                     |
| LoRaWAN Bee     | LoRa module (SX1276) for transmission    |
| Sensors          | Voltage divider, DHT11, LDR, accelerometer |
| Batteries        | 12V 1.3Ah sealed lead-acid               |
| Prototype Board  | Breadboard + MDF base                    |

---

## 🛠️ System Architecture

- **Sensor Layer:** ESP32 reads analog/digital data from various sensors.
- **Transmission Layer:** LoRaWAN Bee sends data to network gateways.
- **Visualization Layer:** Data arrives via network server to web-based dashboard (via MQTT/HTTP API).

---

## 🚀 Getting Started

### Requirements
- Arduino IDE or PlatformIO
- ESP32 board libraries
- TTN/Chirpstack account for LoRaWAN testing
- Web dashboard (e.g., ThingsBoard, Grafana)

### Flashing the Code
1. Install drivers and board support for ESP32
2. Clone this repo and open `main.ino`
3. Configure LoRa settings (DevEUI, AppKey, etc.)
4. Upload to ESP32

---

## 🔍 Monitoring Metrics
- 📈 Real-time voltage analysis using analog-to-digital conversion and calibrated resistors
- 🧠 Custom logic to convert raw sensor values to real-world metrics
- ⚠️ Alerts based on user-defined thresholds (battery drop, overheat, etc.)

---

## 💡 Future Improvements
- Develop custom mobile app for monitoring
- Add MPPT data logging
- Use solar-powered ESP32 variant
- Design a dedicated PCB

---

## 📜 License
This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

## 👨‍💻 Authors
- Gabriel Correa dos Santos Barbosa

> Project developed at Universidade Metodista de São Paulo, 2022
