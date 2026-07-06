# IoT Smart RFID Attendance System (ESP32-C3 & Google Sheets)

An automated, cloud-connected attendance logging system built using an ESP32-C3 microcontroller, an MFRC522 RFID module, and an SSD1306 OLED display. The device offloads database heavy lifting to the cloud using Google Apps Script, allowing the system to scale seamlessly to handle any number of students without running out of local hardware memory constraints.

---

## 🚀 How It Works
1. **Token Scan:** A student taps their RFID token on the reader.
2. **Secure Transmission:** The ESP32-C3 captures the raw UID and securely transmits it via an HTTPS GET request to a deployment endpoint of a Google Web App API.
3. **Cloud Processing:** The serverless Google Apps Script catches the ID, dynamically searches a Roster spreadsheet database, logs a live timestamped record to the Attendance sheet, and relays the student's name back to the device.
4. **Local Feedback:** The ESP32-C3 reads the dynamic plain-text response, blinks the indicator status LEDs, triggers a success chime on the buzzer, and centers the student's real name onto the OLED screen.

---

## 🛠️ Hardware Components
* **Microcontroller:** ESP32-C3 Development Board
* **RFID Reader:** MFRC522 (SPI Interface)
* **Display:** 128x64 SSD1306 I2C OLED
* **Indicators:** Status LEDs (Green/Red) & Active Piezo Buzzer

---

## 🔌 Hardware Pinout Configuration

| Component | Component Pin | ESP32-C3 GPIO Pin | Description |
| :--- | :--- | :--- | :--- |
| **MFRC522 RFID** | SDA (SS) | GPIO 5 | SPI Chip Select |
| | SCK | GPIO 4 | SPI Clock |
| | MOSI | GPIO 6 | SPI Master Out Slave In |
| | MISO | GPIO 7 | SPI Master In Slave Out |
| | RST | GPIO 3 | Reset Pin |
| | 3.3V | 3.3V | Power Supply |
| | GND | GND | Ground |
| **SSD1306 OLED** | SDA | GPIO 8 | I2C Data Line |
| | SCL | GPIO 9 | I2C Clock Line |
| | VCC | 3.3V | Power Supply |
| | GND | GND | Ground |
| **Indicators** | Green LED (+) | GPIO 10 | Success Indicator (Inline 220Ω Resistor) |
| | Red LED (+) | GPIO 1 | Failure/Error Indicator (Inline 220Ω Resistor) |
| | Buzzer (+) | GPIO 2 | Piezo Audio Chime |

---

## 📊 Cloud Database Architecture
The connected Google Spreadsheet utilizes two structured tabs to break down data processing cleanly:
* **Roster:** Database column tracking (`Card ID` | `Student Name`). Features a scale-independent lookup structure using dynamic row lengths.
* **Attendance:** Live running logs table tracking (`Timestamp` | `Card ID` | `Student Name`).

---

## 💻 Tech Stack
* **Firmware:** C++ (Arduino Core)
* **Cloud API Architecture:** JavaScript (Google Apps Script Engine)
* **Communication Protocol:** HTTPS REST API Client

---

## 📚 Software Dependencies & Setup

### Required Arduino Libraries
Ensure you have installed these packages via your Library Manager before compiling:
* `MFRC522` by GitHubCommunity
* `Adafruit SSD1306` & `Adafruit GFX Library`

### Google Apps Script Deployment
1. Create a Google Sheet with two tabs named `Roster` and `Attendance`.
2. Navigate to **Extensions > Apps Script** and paste the contents of `Google_Script.js` included in this repository.
3. Click **Deploy > New Deployment**, select **Web App**, set Access to **"Anyone"**, and copy your generated Web App URL deployment ID into the firmware configuration.

---

## 📸 Project Media & Demo

### 🛠️ Hardware Prototype
Here is the physical layout wired up on the breadboard:

<p align="center">
  <img src="IMG_20260703_214258_4.jpg" alt="Physical Hardware Setup" width="600">
</p>

### 🎥 Live Demonstration

[Click here to watch the live video demonstration of the system in action on LinkedIn!](https://www.linkedin.com/posts/adeleye-dara-235579406_iot-esp32-embeddedsystems-ugcPost-7479179852152217600-mZE8/)
