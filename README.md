# ECE 398 vip-water-monitor

The Water Monitor System / Chicken Nanny provides users with a way to monitor a chicken's water tank remotely. The system uses an ultrasonic sensor, 915MHz LoRa radios, a Raspberry Pi 3B+, and a notification system through Pushbullet that provides your phone with a notification.

# How to build

See Water Monitor folder in VIP Google Drive.

# Hardware

Includes all designed CAD files.

# Software

## ESP32 Folder
Houses the program that goes on the ESP32 MCU. 

Ultrasonic program interfaces with the ESP32 MCU and the Ultrasonic sensor. It detects distance: in our case, water level, and sends a radio transmission.

## Raspberry Pi Folder
Houses the program that goes on the Raspberry Pi.

Bridge program receives on the radio and interfaces with the Pushbullet software to send text messages.

---

An ECE 398 Bradley University Vertically Integrated Projects Design

[vip.bradley.edu](https://vip.bradley.edu)
