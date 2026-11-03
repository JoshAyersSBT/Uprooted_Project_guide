# Uprooted FLL Sensor Station - Vocabulary

Kit v0.2.3 | quick reference for student builders

## Hardware and sensors

- **ESP32-S3:** The tiny programmable computer on our Heltec V4. It runs the code, Wi-Fi, and sensor connections.
- **Heltec V4:** Our circuit board: it combines an ESP32-S3, an OLED screen, and a LoRa radio.
- **PT100 / RTD:** A platinum temperature sensor. A PT100 is about 100 ohms at 0°C; its resistance changes with temperature.
- **MAX31865:** A chip that measures the PT100 resistance, sends the result to the ESP32, and reports wiring faults.
- **SPI:** A fast, short wired connection between chips using clock, data, and chip-select signals.
- **GPIO:** A programmable pin on the ESP32 used to connect sensors and other electronics.
- **OLED:** The small onboard screen that shows temperature and connection status.

## Radio and networking

- **LoRa radio:** A long-range, low-power wireless link for sending small messages. It is different from Wi-Fi.
- **Packet:** One short message sent over the radio, such as a node ID and a temperature reading.
- **Node:** One Heltec board participating in the sensor network.
- **Relay:** When a node receives a packet and passes it along so another board can hear it.
- **Mesh network:** A network where nodes can route messages through other nodes, often using alternate paths.
- **Exit node:** In THIS project, the board that shows LoRa sensor readings on a local Wi-Fi webpage; not an internet exit.
- **Wi-Fi AP:** Access point: a board-created Wi-Fi network you connect your phone or laptop to.
- **Captive portal:** The webpage that a phone may offer to open after joining the board’s Wi-Fi.
- **RSSI:** Received signal strength. A more negative number (like -110 dBm) usually means a weaker signal.

**Important:** This kit uses raw LoRa and limited one-hop relaying. It is not LoRaWAN and does not implement a fully routed, self-healing mesh. Its exit node serves readings on local Wi-Fi, not the internet.

## Further reading
- LoRa explained by Semtech: https://www.semtech.com/lora/what-is-lora
- ESP32-S3 (Espressif): https://www.espressif.com/en/products/socs/esp32-s3
- MAX31865 (Analog Devices): https://www.analog.com/en/products/max31865.html
