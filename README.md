# LoRa Temperature and Humidity Monitor

This project demonstrates a simple implementation of LoRa communication for monitoring temperature and humidity. The system consists of two modules:

1. **Sender Module**: Reads temperature and humidity data using a DHT sensor and transmits it over LoRa.
2. **Receiver Module**: Receives the transmitted data and displays it along with RSSI and SNR information.

## Project Structure

```
LoRa-Temperature-Humidity-Monitor/
├── sender/
│   ├── sender.ino           # Code for the sender
├── receiver/
│   ├── receiver.ino         # Code for the receiver
├── README.md                # Project documentation
```

## Hardware Requirements

### Sender Module
- Heltec LoRa board (or compatible LoRa module)
- DHT11/DHT22 sensor
- Jumper wires

### Receiver Module
- Heltec LoRa board (or compatible LoRa module)

## Software Requirements
- Arduino IDE
- Heltec LoRa library
- RadioLib library

## How It Works

### Sender
- The sender module reads temperature and humidity data using the DHT sensor.
- The data is formatted into a string and sent via LoRa at regular intervals.

### Receiver
- The receiver module listens for incoming LoRa packets.
- When a packet is received, it displays the temperature, humidity, RSSI, and SNR on the console.

## Setup Instructions

1. Clone this repository:
   ```bash
   git clone https://github.com/wassim-55/LoRa-Temperature-Humidity-Monitor.git
   ```

2. Open the `sender/sender.ino` and `receiver/receiver.ino` files in Arduino IDE.

3. Install required libraries in Arduino IDE:
   - Heltec LoRa library
   - RadioLib library

4. Configure the LoRa settings (frequency, bandwidth, spreading factor) in both the sender and receiver code to match your region's regulations.

5. Upload the `sender.ino` code to the sender device and `receiver.ino` code to the receiver device.
