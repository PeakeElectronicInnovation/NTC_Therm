/*
  Basic Low-Side NTC Thermistor Example
  
  This example shows how to read temperature from an NTC thermistor
  in a low-side voltage divider configuration.
  
  Circuit:
    - NTC thermistor connected between analog pin A0 and GND
    - Reference resistor (10kΩ) connected between 3.3V and A0
    - 100nF capacitor parallel to NTC for noise reduction (optional)
  
  Created for NTC_Therm library
*/

#include <NTC_Therm.h>

// Thermistor parameters (adjust for your specific thermistor)
const int THERMISTOR_PIN = A0;
const float REFERENCE_RESISTOR = 10000.0;    // 10kΩ reference resistor
const float NTC_RESISTANCE_25C = 10000.0;    // 10kΩ at 25°C
const float BETA_COEFFICIENT = 3950.0;        // Beta coefficient (check datasheet)
const bool IS_HIGH_SIDE = false;              // Low-side configuration
const uint16_t ADC_RESOLUTION = 12;           // 12-bit ADC (0-4095)
const float ADC_VOLTAGE_REF = 3.3;            // 3.3V reference voltage

NTC_Therm thermistor(THERMISTOR_PIN, REFERENCE_RESISTOR, NTC_RESISTANCE_25C, 
                     BETA_COEFFICIENT, IS_HIGH_SIDE, ADC_RESOLUTION, ADC_VOLTAGE_REF);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("NTC Thermistor - Low Side Configuration Example");
  Serial.println("===============================================");
  
  thermistor.begin();
  
  // Optional: Apply calibration offset if needed
  // thermistor.setOffset(0.5);  // Add 0.5°C offset
}

void loop() {
  float temperature = thermistor.temperature();
  
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" °C");
    
    // Also check if sensor is connected
    if (thermistor.isConnected()) {
      Serial.println("Sensor: Connected");
    } else {
      Serial.println("Sensor: Disconnected or out of range!");
    }
  } else {
    Serial.println("Error: Could not read temperature");
  }
  
  Serial.println();
  delay(1000);  // Read every second
}
