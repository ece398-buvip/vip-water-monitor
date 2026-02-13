#include <Arduino.h> // Using Arduino framework
#include <Wire.h>  // Needed for I2C communication
#include <SPI.h>   // Needed for LoRa

// Ultrasonic Sensor Pins
const int trigPin = 14; // 12
const int echoPin = 12; // 14
const int distanceThreshold = 10; // Threshold in cm. Can be changed depending on distance requirements

// LoRa Module Pins
#define RADIO_RST_PIN  4   // 2 Change this to match your actual reset pin. On LoRa module, it's pin 2 (NRST)
#define RADIO_TX_PIN   17  // 17 Change this based on ESP32 wiring. On LoRa module, it's pin 4 (TXD)
#define RADIO_RX_PIN   16  // 16 Change this based on ESP32 wiring. On LoRa module, it's pin 3 (RXD)
#define RADIO_ADDR     0   // Each radio must have a different address

// When wiring the radios and Pi, make sure their TX and RX pins are swapped. For example;
// on the radio, its TX pin should go to the ESP32's RX pin (16). It's TX-RX and RX-TX, not RX-RX or TX-TX

// LED Pin to verify a message has been sent
 const int ledPin = 25;


// Function to reset the radio module
void reset_radio() {
    digitalWrite(RADIO_RST_PIN, LOW);
    delay(5);
    digitalWrite(RADIO_RST_PIN, HIGH);
    // This simply turns off and turns back on the radio
}

// Function to send AT commands to the radio module
void send_cmd(const String& str) {
    Serial1.print(str + "\r\n");  // Send AT commands over Serial1
}

// Function to set the radio address
void set_address(int address) {
    send_cmd("AT+ADDRESS=" + String(address));
}

// Function to send data over LoRa
void send_data(int address, const String& data) {
    String command = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data;
    send_cmd(command);
}

// Initialize the radio module
void radio_init() {
    pinMode(RADIO_RST_PIN, OUTPUT);
    reset_radio();
    
    Serial1.begin(115200, SERIAL_8N1, RADIO_RX_PIN, RADIO_TX_PIN); // Initialize Serial1 for radio communication
    set_address(RADIO_ADDR);
}

// Function to send warning message
void radio_send(const String& message) {
    int receiver_address = 2; // Address of the receiving device
    send_data(receiver_address, message);
}

// Function to measure distance using the ultrasonic sensor
long measure_distance_cm() {
    long duration;
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    
    return duration / 29.155 / 2; // Convert duration to cm
    // This function measures distance by measuring how long it takes for the ultrasonic sensor 
    // to receive feedback from transmit
}

void setup() { 
    Serial.begin(115200);
    
    // Initialize ultrasonic sensor and led
    pinMode(trigPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Initialize radio module
    radio_init();
}

void loop() {
    long distance = measure_distance_cm();
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    // Prints distance to serial monitor

    if (distance > distanceThreshold) {
        Serial.println("WARNING: Water level is too low!");
        digitalWrite(ledPin, HIGH);

        // Send warning message
        radio_send("Water Level is Low. Current reading: " + String(distance) + " cm");
        delay(500);
        digitalWrite(ledPin, LOW);

        delay(5000);  // Prevent spamming messages
    } else {
    }

    delay(1000);  // Measure every second
}