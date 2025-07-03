# Remote Traffic Lights with LoRa

## Overview

This project implements a remote-controlled traffic light system using two LoRa-enabled microcontrollers, the XIAO ESP32S3 and the Lilygo T-Beam. The system allows you to control a set of traffic lights (represented by LEDs) from a remote location using the LoRa communication protocol. Additionally, the project integrates with Adafruit IO, allowing you to monitor and control the traffic lights from a web-based dashboard.

## Features

  * **Remote Control:** Control traffic lights from a distance using LoRa communication.
  * **Adafruit IO Integration:** Monitor and control the traffic lights from an Adafruit IO dashboard.
  * **Real-time Status Updates:** Get real-time status updates of the traffic lights on the Adafruit IO dashboard.
  * **Easy to build:** The project uses readily available components and can be assembled with basic electronics knowledge.

## Hardware Requirements

  * **XIAO ESP32S3:** This microcontroller sends the control signals.
  * **Lilygo T-Beam:** This microcontroller receives the control signals and controls the LEDs.
  * **LEDs:** Red, yellow, and green LEDs to represent the traffic lights.
  * **Resistors:** To limit the current to the LEDs.
  * **Breadboard and Jumper Wires:** For prototyping the circuit.

## Software Requirements

  * **Arduino IDE:** To program the microcontrollers.
  * **Adafruit IO Account:** To monitor and control the traffic lights from a web dashboard.

## Libraries

  * **Adafruit IO Arduino:** To connect to the Adafruit IO service.
  * **Arduino LoRa:** To enable LoRa communication.
  * **Adafruit MQTT Library:** For MQTT communication with Adafruit IO.
  * **SPI:** For communication with the LoRa module.

## Adafruit IO Setup

1.  Create an account on [Adafruit IO](https://io.adafruit.com/).
2.  Create a new dashboard.
3.  Create three new feeds:
      * `traffic-light-red`
      * `traffic-light-yellow`
      * `traffic-light-green`
4.  Add three toggle blocks to your dashboard and connect them to the feeds you just created.
5.  Get your Adafruit IO username and AIO key.

## Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/caringalml/remote-traffic-lights-with-lora-.git
    ```
2.  **Install the required libraries:**
      * Open the Arduino IDE.
      * Go to **Sketch \> Include Library \> Manage Libraries...**
      * Search for and install the following libraries:
          * Adafruit IO Arduino
          * Arduino LoRa
          * Adafruit MQTT Library
3.  **Configure the `secret.h` file:**
      * Open the `send_to_lora_and_to_adafruit_server/secret.h` file.
      * Replace `"<your-username>"` with your Adafruit IO username.
      * Replace `"<your-aio-key>"` with your Adafruit IO AIO key.
4.  **Configure the `config.h` file:**
      * Open the `send_to_lora_and_to_adafruit_server/config.h` file.
      * Configure your WiFi credentials.
5.  **Upload the code:**
      * Upload the `send_to_lora_and_to_adafruit_server.ino` sketch to your XIAO ESP32S3.
      * Upload the `recieve_from_lora_and_control_led.ino` sketch to your Lilygo T-Beam.

## Usage

1.  Power on both the XIAO ESP32S3 and the Lilygo T-Beam.
2.  The XIAO ESP32S3 will connect to your WiFi network and the Adafruit IO service.
3.  The Lilygo T-Beam will wait for LoRa signals.
4.  Open your Adafruit IO dashboard.
5.  Use the toggle blocks to control the traffic lights.

## Contributing

Contributions are welcome\! Please open an issue or submit a pull request if you have any improvements or suggestions.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.
