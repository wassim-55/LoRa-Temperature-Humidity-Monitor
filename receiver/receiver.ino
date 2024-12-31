#include <heltec_unofficial.h>

// LoRa Configuration Parameters
#define HELTEC_POWER_BUTTON
#define FREQUENCY           866.3       // LoRa frequency in MHz (Europe)
#define BANDWIDTH           125.0       // Bandwidth in kHz
#define SPREADING_FACTOR    7           // Spreading factor for LoRa

// Variables for Reception Control
String rxdata;
volatile bool rxFlag = false;

void setup() {
  heltec_setup();                 // Initialize the Heltec board
  both.println("LoRa Receiver Initialized");

  // Initialize LoRa module
  RADIOLIB_OR_HALT(radio.begin());

  // Configure LoRa Parameters
  both.printf("Frequency: %.2f MHz\n", FREQUENCY);
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  both.printf("Bandwidth: %.1f kHz\n", BANDWIDTH);
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  both.printf("Spreading Factor: %i\n", SPREADING_FACTOR);
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));

  // Start listening for packets
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
}

void loop() {
  heltec_loop();

  // If a packet is received, process it
  if (rxFlag) {
    rxFlag = false;
    radio.readData(rxdata);

    if (_radiolib_status == RADIOLIB_ERR_NONE) {
      // Display the received data
      both.printf("Received Data: %s\n", rxdata.c_str());
      both.printf("RSSI: %.2f dBm\n", radio.getRSSI());
      both.printf("SNR: %.2f dB\n", radio.getSNR());
    } else {
      both.println("Error reading data");
    }

    // Resume listening for packets
    RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
  }
}

// Interrupt Service Routine for Packet Reception
void rx() {
  rxFlag = true;
}

