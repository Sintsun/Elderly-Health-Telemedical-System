# Elderly Healthcare Telemedical System

## Project Overview
The Elderly Healthcare Telemedical System, is an Arduino-based health monitoring solution tailored for elderly users. It integrates sensors to measure vital signs (heart rate, blood oxygen, temperature, blood pressure), an OLED screen for real-time data display, RFID for user authentication, and ThingSpeak for cloud storage. The system aims to simplify health checkups, reduce hospital visits, and enable remote data access via a website. With a user-friendly interface, it ensures ease of use for elderly patients and efficient data management for healthcare providers.
images/overview.png

## Core Functionalities
The system provides the following key features, each designed for simplicity and reliability:

1. **OLED Screen Display**:
   - Displays a menu for selecting tests (heart rate, SpO2, temperature, blood pressure) via inputs '1' to '4' or 'Q' to exit.
   - Shows real-time health data (e.g., BPM, SpO2 percentage) and system status (e.g., "Please tap your RFID card").
   - Renders a heart rate trend graph for visual monitoring.

2. **Heart Rate Monitoring**:
   - Measures beats per minute (BPM) using a pulse sensor connected to Arduino Mega 2560 (port A2).
   - Displays BPM numerically and as a trend graph on the OLED screen.
   - Uploads results to ThingSpeak for historical tracking.

3. **RFID Authentication**:
   - Allows users to log in by tapping an RFID card, reading the unique ID to access their cloud-stored data.
   - Simplifies access for elderly users, avoiding complex login credentials.

4. **SpO2 (Blood Oxygen) Monitoring**:
   - Measures blood oxygen levels, ensuring readings are within 90–99%.
   - Displays results on the OLED screen and uploads to ThingSpeak.

5. **Blood Pressure Monitoring**:
   - Automates the pump (pin 12) and valve (pin 13) to measure blood pressure, stopping at 130 mmHg.
   - Displays results on the OLED screen with status updates (e.g., "Pumping").

6. **Cloud Storage and Website**:
   - Uploads health data to ThingSpeak every 60 seconds, stored in user-specific channels.
   - Provides a website for doctors to log in and view real-time or historical data.

## Functional Visuals
The following images showcase the system’s core functionalities in a grid layout:

| OLED Screen Menu | Heart Rate Trend Graph | RFID Authentication |
|------------------|------------------------|---------------------|
| ![OLED Screen Menu](images/oled_menu.png) | ![Heart Rate Graph](images/heart_rate_graph.png) | ![RFID Authentication](images/rfid_scan.png) |

| SpO2 Output | Blood Pressure Status |
|-------------|-----------------------|
| ![SpO2 Output](images/spo2_output.png) | ![Blood Pressure Status](images/bp_status.png) |

*Note*: Ensure images are uploaded to the `images` folder in the repository. See the guide below for instructions on creating and embedding images.

## Installation and Setup
### Prerequisites
- Arduino IDE (version 1.8.x or higher)
- Arduino Mega 2560 and Arduino Micro boards
- Libraries: `Adafruit_SSD1306`, `U8g2`, `SoftwareSerial`, `ThingSpeak`, `MFRC522`
- Hardware: Pulse sensor, SpO2 sensor, temperature sensor, blood pressure sensor, OLED (128x64, I2C), e-paper, RFID reader, ESP8266 WiFi module
- WiFi network for connectivity

### Setup Steps
1. **Connect Hardware**:
   - Wire sensors, OLED, e-paper, RFID, and WiFi modules to Arduino boards (see report pages 8–9 for pin connections).
2. **Install Arduino IDE and Libraries**:
   - Download from [arduino.cc](https://www.arduino.cc/en/software).
   - Install libraries via the Library Manager.
3. **Load and Upload Code**:
   - Clone the repository: `https://github.com/Sintsun/Elderly-Health-Telemedical-System`.
   - Open `.ino` files (e.g., `main.ino`) in Arduino IDE and upload to respective boards.
4. **Run the System**:
   - Open Serial Monitor (baud rate: 9600 or 115200).
   - Tap an RFID card, then select tests (1–4) or quit ('Q').
5. **Access Website**:
   - Log in to view health data on ThingSpeak.

## Testing Results
- **OLED Screen**: Displays menu, real-time data, and heart rate graph correctly.
- **Heart Rate**: Accurately measures BPM and renders trend graph.
- **RFID**: Successfully reads card UIDs and retrieves cloud data.
- **SpO2**: Measures oxygen levels reliably.
- **Blood Pressure**: Automates measurement with proper timing.
- **Cloud Storage**: Uploads/downloads data consistently.

## References
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [Heart Rate Tutorial](https://create.arduino.cc/projecthub/Techatronic/how-to-make-heart-rate-monitor-15c86b)
- [SpO2 and Blood Pressure Tutorial](https://create.arduino.cc/projecthub/protocentral/measure-spo2-heart-rate-and-bp-trends-bptusing-arduino-bcc316)
- [RFID Tutorial](https://create.arduino.cc/projecthub/Aritro/security-access-using-rfid-reader-f7c746)
- Technical Training II Manual

## License
This project is for academic purposes only. Copyright © 2021 Tang Sin Tsun, Chung Chi Kin, Ng Long Hin.
