#include <heltec_unofficial.h>
#include <DHT.h>

// DHT11 Sensor Configuration
#define DHTPIN 42          // Pin where the DHT11 sensor is connected
#define DHTTYPE DHT11      // Type of sensor (DHT11, DHT22, etc.)
DHT dht(DHTPIN, DHTTYPE);

// LoRa Configuration Parameters
#define HELTEC_POWER_BUTTON
#define PAUSE               5   // Interval between transmissions (in seconds)
#define FREQUENCY           866.3       // LoRa frequency in MHz (Europe)
#define BANDWIDTH           125.0       // Bandwidth in kHz
#define SPREADING_FACTOR    7           // Spreading factor for LoRa
#define TRANSMIT_POWER      14          // Transmission power in dBm

// Variables for Transmission Control
long counter = 0;
uint64_t last_tx = 0;
uint64_t tx_time;
uint64_t minimum_pause;

void setup() {
  heltec_setup();                 // Initialize the Heltec board
  dht.begin();                    // Initialize the DHT11 sensor
  both.println("LoRa Transmitter Initialized");

  // Initialize LoRa module
  RADIOLIB_OR_HALT(radio.begin());

  // Configure LoRa Parameters
  both.printf("Frequency: %.2f MHz\n", FREQUENCY);
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  both.printf("Bandwidth: %.1f kHz\n", BANDWIDTH);
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  both.printf("Spreading Factor: %i\n", SPREADING_FACTOR);
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  both.printf("Transmission Power: %i dBm\n", TRANSMIT_POWER);
  RADIOLIB_OR_HALT(radio.setOutputPower(TRANSMIT_POWER));
}

void loop() {
  heltec_loop();

  // Check if the legal transmission interval has passed
  bool tx_legal = millis() > last_tx + minimum_pause;
  if (PAUSE && tx_legal && millis() - last_tx > (PAUSE * 1000)) {
    // Read temperature and humidity values from the sensor
    float temperature = dht.readTemperature();  // Temperature in Celsius
    float humidity = dht.readHumidity();        // Humidity in percentage

    // Check for reading errors
    if (isnan(temperature) || isnan(humidity)) {
      both.println("Error reading from DHT11 sensor!");
      return;
    }

    // Prepare the message to be transmitted
    String txMessage = "Temperature: " + String(temperature, 1) + "°C, Humidity: " + String(humidity, 1) + "% [" + String(counter++) + "]";
    both.printf("Sending at %.2f MHz: %s\n", FREQUENCY, txMessage.c_str());

    // Transmit the message
    heltec_led(50);                // Turn on LED at 50% brightness during transmission
    tx_time = millis();            // Record transmission start time
    RADIOLIB(radio.transmit(txMessage.c_str()));
    tx_time = millis() - tx_time;  // Calculate transmission duration
    heltec_led(0);                 // Turn off the LED

    if (_radiolib_status == RADIOLIB_ERR_NONE) {
      both.printf("Transmission successful (%i ms)\n", (int)tx_time);
    } else {
      both.printf("Transmission failed (%i)\n", _radiolib_status);
    }

    // Enforce 1% duty cycle by calculating minimum pause time
    minimum_pause = tx_time * 100;
    last_tx = millis();
  }
}

