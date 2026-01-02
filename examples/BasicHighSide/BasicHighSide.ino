/*
  Basic High-Side NTC Thermistor Example
  
  This example shows how to read temperature from an NTC thermistor
  in a high-side voltage divider configuration.
  
  Circuit:
    - NTC thermistor connected between 3.3V and analog pin A0
    - Reference resistor (10kΩ) connected between A0 and GND
    - 100nF capacitor parallel to NTC for noise reduction (optional)
  
  Created for NTC_Therm library
*/

#include <NTC_Therm.h>

// Thermistor parameters (adjust for your specific thermistor)
const int THERMISTOR_PIN = A0;
const float REFERENCE_RESISTOR = 10000.0;    // 10kΩ reference resistor
const float NTC_RESISTANCE_25C = 10000.0;    // 10kΩ at 25°C
const float BETA_COEFFICIENT = 3950.0;        // Beta coefficient (check datasheet)
const bool IS_HIGH_SIDE = true;               // High-side configuration
const uint16_t ADC_RESOLUTION = 12;           // 12-bit ADC (0-4095)
const float ADC_VOLTAGE_REF = 3.3;            // 3.3V reference voltage

NTC_Therm thermistor(THERMISTOR_PIN, REFERENCE_RESISTOR, NTC_RESISTANCE_25C, 
                     BETA_COEFFICIENT, IS_HIGH_SIDE, ADC_RESOLUTION, ADC_VOLTAGE_REF);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("NTC Thermistor - High Side Configuration Example");
  Serial.println("===============================================");
  
  thermistor.begin();
  
  // Optional: Apply calibration offset and scale
  // thermistor.setOffset(-0.3);  // Subtract 0.3°C offset
  // thermistor.setScale(1.02);   // Scale by 2%
}

void loop() {
  float temperature = thermistor.temperature();
  
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" °C");
    
    // Convert to Fahrenheit if desired
    float tempF = temperature * 9.0 / 5.0 + 32.0;
    Serial.print("Temperature: ");
    Serial.print(tempF, 2);
    Serial.println(" °F");
    
    // Check sensor connection
    Serial.println(thermistor.isConnected() ? "Sensor: Connected" : "Sensor: Disconnected");
  } else {
    Serial.println("Error: Could not read temperature");
  }
  
  Serial.println();
  delay(2000);  // Read every 2 seconds
}
