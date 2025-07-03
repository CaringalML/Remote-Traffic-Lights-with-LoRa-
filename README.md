# Remote Traffic Lights with LoRa

## Overview

This project implements a portable, remote-controlled **traffic light system** designed for **Temporary Traffic Management (TTM)** zones in **New Zealand**. It uses **two LoRa-enabled microcontrollers**—the **XIAO ESP32S3** (transmitter) and the **Lilygo T-Beam** (receiver)—to wirelessly control traffic lights **without any internet or Wi-Fi connection**.

The system uses **only Red and Green lights**, mirroring the manual stop/go paddles commonly used in TTM sites with a 30 km/h speed limit. This makes it suitable for deployment in remote, hazard-prone, or network-limited environments.

---

## ℹ️ Why Only Red and Green?

New Zealand’s Temporary Traffic Management standards for low-speed zones (typically 30 km/h) use only **Red** and **Green** lights. Amber/Yellow lights are omitted because drivers are expected to stop immediately when the red light shows, minimizing hesitation and confusion.

---

## 🖼️ Project Images

Below are some snapshots of the setup:

![Image 6](./images/image-6.jpg)  
*Transmitter (XIAO ESP32S3) wired and ready to send.*

![Image 7](./images/image-7.jpg)  
*Receiver (Lilygo T‑Beam) with LEDs and breadboard.*

![Image 1](./images/image-1.jpg)  
*Traffic Light Body*

![Image 2](./images/image-2.jpg)  
*Traffic Light Body*

![Image 3](./images/image-3.jpg)  
*Traffic Light Electronics components*

![Image 4](./images/image-4.jpg)  
*Traffic Light Body*

![Image 5](./images/image-5.jpg)  
*Traffic Light Body*

---

## 🎥 Project Video

Watch a demo of the system in action:

[![Remote Traffic Lights with LoRa Demo](https://img.youtube.com/vi/X4vUCieE72s/0.jpg)](https://youtube.com/shorts/X4vUCieE72s)

---

## Features

- ✅ **Fully Offline:** Operates without Wi-Fi or internet  
- 📡 **LoRa Communication:** Long-range, low-power radio control  
- 🟥🟩 **Red and Green Lights Only:** Following NZ TTM standards  
- 🔋 **Portable & Battery Friendly:** Ideal for field use  
- 🛠️ **Optional Adafruit IO Integration:** For simulation/testing only  

---

## Hardware Requirements

- **XIAO ESP32S3** – Transmitter microcontroller  
- **Lilygo T-Beam** – Receiver microcontroller  
- **LEDs** – Red and Green only  
- **Resistors** – 220–330 Ω current-limiting  
- **Breadboard & Jumper Wires** – For prototyping  
- **Power Source** – USB battery pack or similar  

---

## Software Requirements

- **Arduino IDE**

## Required Libraries

- **Arduino LoRa**  
- **SPI**

---

## Installation Steps

1. Clone the repository:

   ```bash
   git clone https://github.com/caringalml/remote-traffic-lights-with-lora-.git
