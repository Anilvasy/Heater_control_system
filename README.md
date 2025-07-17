# Temperature and Humidity Controller with DHT22 and LCD

This project implements a real-time temperature and humidity monitoring system using the DHT22 sensor, LCD (I2C), and interrupt-based button handling using FreeRTOS (optional). It includes both auto and manual override modes.

## 💡 Features
- Read temperature and humidity from DHT22
- Display on I2C LCD
- Mode switch via button (with ISR)
- Overheat warning with buzzer and LEDs
- Manual mode timeout using millis()
- Clean interface using Serial and LCD

## 🔧 Hardware Used
- ESP32 Dev Module
- DHT22 Sensor
- I2C LCD 16x2
- Push Button
- 4 LEDs (Status indication)
- Buzzer
  

## 🖥️ Simulation Link
👉 Wokwi Simulation: https://wokwi.com/projects/436618122468225025



## 🚀 Getting Started

- DHT22 → GPIO14
- LCD SDA/SCL → GPIO21/22
- Button → GPIO34
- LEDs → GPIO27, 26, 25, 33
- Buzzer → GPIO16

### Uploading
1. Clone this repo
2. Open `main.ino` in Arduino IDE
3. Select "ESP32 Dev Module"
4. Install dependencies:
   - DHT Sensor Library
   - Adafruit Unified Sensor
   - LiquidCrystal_I2C
5. Upload to board

## 📄 Design Document
See ./DESIGN.pdf) for complete architecture, state diagrams, flow, and system overview.

## 📷 Screenshots
LCD Display:(media/simulation_screenshot.png)

## 🔗 License
This project is licensed under the MIT License
