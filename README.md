# Remote Traffic Lights with LoRa

## Overview

This project implements a remote-controlled traffic light system using two LoRa-enabled microcontrollers: the **XIAO ESP32S3** (transmitter) and the **Lilygo T-Beam** (receiver). Leveraging the LoRa communication protocol and Adafruit IO integration, you can monitor and control LEDs that simulate a traffic light setup from anywhere with an internet connection.

---

## 🖼️ Project Images

Below are some snapshots of the setup:

![Image 1](./images/image-1.jpg)  
*Transmitter (XIAO ESP32S3) wired up and ready to send.*

![Image 2](./images/image-2.jpg)  
*Receiver (Lilygo T‑Beam) with LEDs and breadboard layout.*

![Image 3](./images/image-3.jpg)  
*Close-up showing the traffic light LEDs in action.*

![Image 4](./images/image-4.jpg)  
*Adafruit IO dashboard displaying live toggle controls.*

![Image 5](./images/image-5.jpg)  
*Final prototype setup with both microcontrollers powered.*

---

## Features

- **Remote Control:** Operate traffic lights wirelessly via LoRa.
- **Adafruit IO Integration:** Use a web dashboard for monitoring and controlling lights.
- **Real-time Updates:** Instant feedback on LED status through the dashboard.
- **Simple Build:** Uses common components and straightforward wiring.

---

## Hardware Requirements

- **XIAO ESP32S3** – sends control signals  
- **Lilygo T-Beam** – receives signals and drives LEDs  
- **LEDs** – red, yellow, green  
- **Resistors** – current-limiting (220–330 Ω recommended)  
- **Breadboard & Jumper Wires** – for assembly  

---

## Software Requirements

- **Arduino IDE**
- **Adafruit IO account**

---

## Required Libraries

- **Adafruit IO Arduino**  
- **Arduino LoRa**  
- **Adafruit MQTT Library**  
- **SPI**  

---

## Adafruit IO Setup

1. Sign up at [Adafruit IO](https://io.adafruit.com/).
2. Create a new dashboard.
3. Add three feeds:  
   - `traffic-light-red`  
   - `traffic-light-yellow`  
   - `traffic-light-green`  
4. Add toggle blocks for each feed.
5. Copy your Adafruit IO username and AIO key for later.

---

## Installation Steps

1. **Clone the repo:**
   ```bash
   git clone https://github.com/caringalml/remote-traffic-lights-with-lora-.git
````

2. **Install Libraries:**

   * In Arduino IDE: **Sketch → Include Library → Manage Libraries…**
   * Search for and install:

     * *Adafruit IO Arduino*
     * *Arduino LoRa*
     * *Adafruit MQTT Library*
3. **Configure WiFi & Adafruit IO:**

   * Open `send_to_lora_and_to_adafruit_server/secret.h`

     ```cpp
     #define IO_USERNAME "<your-username>"
     #define IO_KEY      "<your-aio-key>"
     ```
   * Open `config.h` and set your SSID and password:

     ```cpp
     #define WIFI_SSID     "your_SSID"
     #define WIFI_PASSWORD "your_password"
     ```
4. **Upload Sketches:**

   * `send_to_lora_and_to_adafruit_server.ino` → **XIAO ESP32S3**
   * `receive_from_lora_and_control_led.ino` → **Lilygo T-Beam**

---

## Usage Guide

1. Power on both microcontrollers.
2. The XIAO ESP32S3 will connect to WiFi and Adafruit IO.
3. The Lilygo T‑Beam listens for LoRa commands.
4. Open your Adafruit IO dashboard.
5. Use the toggles to activate the red, yellow, and green LEDs.

---


## 🎥 Project Video

Check out a quick demo of the system in action:

[![Remote Traffic Lights with LoRa Demo](https://img.youtube.com/vi/X4vUCieE72s/0.jpg)](https://youtube.com/shorts/X4vUCieE72s)



## Contributing

We welcome contributions! Feel free to [open an issue](https://github.com/caringalml/remote-traffic-lights-with-lora-/issues) or submit a PR with enhancements or fixes.

---

## License

Distributed under the MIT License—see the [LICENSE](./LICENSE) file for details.
