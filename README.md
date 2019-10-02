# NexUploadWIFI
This is library for Arduino IDE that can be used for uploading firmware to Nextion HMI displays over WiFi using ESP32. To use this library connect ESP32 to Nextion HMI over UART and flash ESP32 with sketch from example.

## Dependencies
To use NexUploadWIFI you need to:
* Setup [Arduino core for the ESP32](https://github.com/espressif/arduino-esp32)
* Install [Nextion](https://github.com/itead/ITEADLIB_Arduino_Nextion) Library
* Install [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer#installation)

## Instalation
Download Library from [release page](https://github.com/baltazorr/NexUploadWIFI/releases/tag/1.0) and unpack it to your Arduino library directory. Restart Arduino IDE and find NexUploadWIFI in examples.

## How to use
* Use ESP32 connected to Nextion HMI over UART2
* Open NexUploadWIFI example and set `ssid` (line 29) and `password` (line 30) of your WiFi network.
* Flash ESP32 with sketch from example.
* Run Serial Monitor in Arduino IDE, wait until ESP32 will connect to the network. The IP address will be outputted in Serial Monitor.
* Open web browser on PC that connected to the same network. Input in address line IP of ESP32 and `/f` for example: `http://192.168.1.10/f`
* On page that will be open select Nextion firmware and press Upload button
