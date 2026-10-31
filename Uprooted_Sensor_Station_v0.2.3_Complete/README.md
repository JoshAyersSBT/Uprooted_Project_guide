# Uprooted FLL Sensor Station v0.2.3

Use the Heltec WiFi LoRa 32 V4 with a 2-wire PT100 through MAX31865.

1. Remove the old `HeltecPT100Kit` folder from Arduino's `libraries` folder; install `HeltecPT100Kit_Library.zip` through Sketch > Include Library > Add .ZIP Library.
2. Open `Uprooted_Sensor_Station/Uprooted_Sensor_Station.ino`; keep `website.h` **in the same sketch folder**.
3. Follow the included three-page student guide (`Uprooted_FLL_Build_Guide.pdf`) to fill the TODOs, create the HTML page, and test the complete system.
4. Select the appropriate Heltec V4 board, attach the LoRa antenna, compile, upload, open Serial Monitor at 115200.
5. Join Wi-Fi `PT100-UPROOTED-XXXX` (last digits are unique per board) and open the captive portal or `http://192.168.4.1`.

The student HTML is explicitly passed to the library via `station.setupAP(WIFI_PASSWORD, STUDENT_HTML)`. Changing the HTML requires recompiling/uploading. The HTML is **not** uploaded separately to LittleFS and cannot be replaced in the captive portal without reflashing. The /api/status and /api/nodes endpoints remain available automatically.

**Pins:** MAX31865 CLK -> GPIO4, SDO -> GPIO6, SDI -> GPIO47, CS -> GPIO48. VIN -> 3V3 and GND -> GND. On the 2-wire MAX31865 breakout, jumper F+ / RTD+, jumper F- / RTD-, and select 2/4 position. Keep LoRa antenna attached.

**Instructor:** Full working code is under `Teacher_Example`. Keep separate from student files.

This package preserves the previous LoRa radio cooldown and relay service. Do not use long `delay()` calls in loop() or the webpage will stop responding.

Validation: Static source checks and HTML syntax only. Hardware build and test with Heltec's Arduino package still needed.

**Guide included:** Three-page printable PDF and matching editable DOCX. Page 3 covers building the student website using `website.h`.


## When the MAX31865 cannot initialize

You can run a Heltec as a LoRa-to-Wi-Fi **exit node** without a local PT100.
Still call `station.setupLoRa()` and `station.setupAP(WIFI_PASSWORD, STUDENT_HTML)`
even when `station.setupPT100()` fails. Do not stop the sketch or enter an
endless error loop. Keep `station.update()` running. The supplied `website.h`
will automatically replace the local PT100 card with the **Operating as an
exit node** message and prioritize available remote sensors (temperature,
fault status, RSSI, age, direct/relayed) from `/api/nodes`.

Exit node here means a LoRa-to-local-Wi-Fi sensor dashboard and packet relay;
**it is not an internet gateway**. `mode` and `exit_node` are available in
`/api/status`, and `station.exitNodeMode()` is available for optional student
logic. Reconnecting a working MAX31865 automatically restores the sensor
station view after SPI retries (when readTemp() continues running).
