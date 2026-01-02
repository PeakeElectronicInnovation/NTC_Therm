/*
  NTC Thermistor Calibration Example
  
  This example demonstrates how to calibrate your NTC thermistor
  by comparing readings with a known accurate temperature source.
  
  Instructions:
  1. Connect your thermistor circuit
  2. Place the sensor and a reference thermometer together
  3. Let them stabilize for several minutes
  4. Enter the reference temperature when prompted
  5. The sketch will calculate and display the offset
  
  Created for NTC_Therm library
*/

#include <NTC_Therm.h>

// Thermistor configuration
const int THERMISTOR_PIN = A0;
const float REFERENCE_RESISTOR = 10000.0;    // 10kΩ reference resistor
const float NTC_RESISTANCE_25C = 10000.0;    // 10kΩ at 25°C
const float BETA_COEFFICIENT = 3950.0;        // Beta coefficient
const bool IS_HIGH_SIDE = false;              // Low-side configuration
const uint16_t ADC_RESOLUTION = 12;           // 12-bit ADC
const float ADC_VOLTAGE_REF = 3.3;            // 3.3V reference

NTC_Therm thermistor(THERMISTOR_PIN, REFERENCE_RESISTOR, NTC_RESISTANCE_25C, 
                     BETA_COEFFICIENT, IS_HIGH_SIDE, ADC_RESOLUTION, ADC_VOLTAGE_REF);

float calibrationOffset = 0.0;
bool calibrated = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("NTC Thermistor Calibration");
  Serial.println("==========================");
  Serial.println();
  Serial.println("This sketch helps you calibrate your thermistor.");
  Serial.println("You'll need a reference thermometer for accurate calibration.");
  Serial.println();
  Serial.println("Instructions:");
  Serial.println("1. Place your NTC sensor and reference thermometer together");
  Serial.println("2. Wait for temperature to stabilize (5-10 minutes)");
  Serial.println("3. Enter the reference temperature in Celsius");
  Serial.println("4. The sketch will calculate the offset");
  Serial.println();
  
  thermistor.begin();
}

void loop() {
  if (!calibrated) {
    // Show current reading
    float currentTemp = thermistor.temperature();
    if (!isnan(currentTemp)) {
      Serial.print("Current reading: ");
      Serial.print(currentTemp, 2);
      Serial.println(" °C");
    }
    
    Serial.println();
    Serial.println("Enter reference temperature in Celsius (e.g., 23.5):");
    
    // Wait for user input
    while (!Serial.available()) {
      delay(100);
    }
    
    // Read reference temperature
    float referenceTemp = Serial.parseFloat();
    
    if (referenceTemp != 0) {
      // Calculate offset
      calibrationOffset = referenceTemp - currentTemp;
      thermistor.setOffset(calibrationOffset);
      calibrated = true;
      
      Serial.println();
      Serial.println("Calibration complete!");
      Serial.print("Calculated offset: ");
      Serial.print(calibrationOffset, 3);
      Serial.println(" °C");
      Serial.println();
      Serial.println("Use this offset in your sketch:");
      Serial.print("thermistor.setOffset(");
      Serial.print(calibrationOffset, 3);
      Serial.println(");");
      Serial.println();
      Serial.println("Showing calibrated readings...");
      Serial.println();
    } else {
      Serial.println("Invalid input. Please try again.");
      while (Serial.available()) Serial.read(); // Clear buffer
    }
  } else {
    // Show calibrated readings
    float temp = thermistor.temperature();
    if (!isnan(temp)) {
      Serial.print("Calibrated Temperature: ");
      Serial.print(temp, 2);
      Serial.println(" °C");
    } else {
      Serial.println("Error reading temperature");
    }
    
    delay(1000);
  }
}
