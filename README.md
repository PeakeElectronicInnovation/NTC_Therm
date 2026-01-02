# NTC_Therm - Arduino NTC Thermistor Library

A lightweight and accurate Arduino library for reading temperature from NTC thermistors in voltage divider configurations.

## Features

- **Flexible Configuration**: Supports both low-side and high-side voltage divider setups
- **High Accuracy**: Uses the Beta parameter equation for precise temperature calculations
- **Configurable ADC**: Works with any ADC resolution and reference voltage
- **Connection Detection**: Built-in method to check if the sensor is properly connected
- **Calibration Support**: Easy offset and scaling for sensor calibration
- **Noise Resilient**: Includes connection validation to detect disconnected sensors

## Installation

### Arduino IDE
1. Download the library as a ZIP file
2. In Arduino IDE, go to Sketch → Include Library → Add .ZIP Library...
3. Select the downloaded ZIP file

### PlatformIO
1. Add to your `platformio.ini`:
   ```ini
   lib_deps = 
       NTC_Therm
   ```
   Or install via command line:
   ```bash
   pio lib install "NTC_Therm"
   ```

### Manual Installation
1. Clone or download this repository
2. Copy the `NTC_Therm` folder to your Arduino `libraries` directory
3. Restart the Arduino IDE

## Theory of Operation

The library uses the Beta parameter equation to calculate temperature from the NTC thermistor's resistance:

```
1/T = 1/T₀ + (1/β) × ln(R/R₀)
```

Where:
- T = Temperature in Kelvin
- T₀ = Reference temperature (25°C = 298.15K)
- β = Beta coefficient (thermistor-specific)
- R = Measured resistance
- R₀ = Resistance at T₀ (typically at 25°C)

## Circuit Configurations

### Low-Side Configuration
```
+Vref
  |
[R_Ref]
  |─── ADC Input Pin
[NTC]
 _|_
 GND
```

### High-Side Configuration
```
+Vref
  |
[NTC]
  |─── ADC Input Pin
[R_Ref]
 _|_
 GND
```

## Usage

### Basic Example

```cpp
#include <NTC_Therm.h>

// Thermistor parameters
const int PIN = A0;
const float R_REF = 10000.0;      // 10kΩ reference resistor
const float R_NTC_25 = 10000.0;   // 10kΩ at 25°C
const float BETA = 3950.0;        // Beta coefficient
const bool HIGH_SIDE = false;     // Low-side configuration
const uint16_t ADC_RES = 12;      // 12-bit ADC
const float V_REF = 3.3;          // 3.3V reference

NTC_Therm therm(PIN, R_REF, R_NTC_25, BETA, HIGH_SIDE, ADC_RES, V_REF);

void setup() {
  Serial.begin(115200);
  therm.begin();
}

void loop() {
  float temp = therm.temperature();
  if (!isnan(temp)) {
    Serial.print("Temperature: ");
    Serial.print(temp, 2);
    Serial.println(" °C");
  }
  delay(1000);
}
```

### Calibration

```cpp
// Apply offset to correct systematic error
therm.setOffset(0.5);  // Add 0.5°C offset

// Apply scaling factor
therm.setScale(1.02);  // Scale by 2%

// Get current calibration values
float offset = therm.getOffset();
float scale = therm.getScale();
```

### Connection Check

```cpp
if (therm.isConnected()) {
  Serial.println("Sensor is connected");
} else {
  Serial.println("Sensor disconnected or out of range!");
}
```

## API Reference

### Constructor

```cpp
NTC_Therm(int pin, float R_Ref, float R_NTC_25, float Beta, bool isHighSide, uint16_t adcRes, float adcVoltageRef)
```

- `pin`: Analog input pin
- `R_Ref`: Reference resistor value in ohms
- `R_NTC_25`: Thermistor resistance at 25°C in ohms
- `Beta`: Beta coefficient of the thermistor
- `isHighSide`: `true` for high-side, `false` for low-side configuration
- `adcRes`: ADC resolution in bits (e.g., 10, 12, 16)
- `adcVoltageRef`: ADC reference voltage in volts

### Methods

#### `void begin()`
Initializes the pin and performs an initial ADC reading.

#### `float temperature()`
Returns the temperature in Celsius. Returns `NAN` if reading fails.

#### `bool isConnected()`
Checks if the sensor is connected and within valid range. Returns `true` if connected.

#### `void setOffset(float offsetC)`
Sets a temperature offset in Celsius for calibration.

#### `void setScale(float scale)`
Sets a scaling factor for calibration (1.0 = no scaling).

#### `float getOffset()`
Returns the current temperature offset.

#### `float getScale()`
Returns the current scaling factor.

## Thermistor Selection

Common NTC thermistor parameters:

| Thermistor | R₀ (25°C) | Beta (°K) | Temperature Range |
|------------|-----------|-----------|-------------------|
| 10KΩ NTC   | 10,000Ω   | 3950      | -40°C to +125°C   |
| 10KΩ NTC   | 10,000Ω   | 3977      | -40°C to +125°C   |
| 100KΩ NTC  | 100,000Ω  | 3950      | -40°C to +125°C   |
| 100KΩ NTC  | 100,000Ω  | 4250      | -20°C to +150°C   |

Always check your thermistor's datasheet for accurate values.

## Tips for Best Results

1. **Reference Resistor**: Use a 1% tolerance or better resistor for R_Ref
2. **Noise Reduction**: Add a 100nF capacitor in parallel with the NTC
3. **Calibration**: Calibrate at your target temperature range
4. **ADC Resolution**: Use the highest available ADC resolution
5. **Wiring**: Keep wires short and away from noise sources
6. **Temperature Stability**: Allow the sensor to stabilize before reading

## Troubleshooting

### Always Returns NAN
- Check wiring connections
- Verify the sensor is connected
- Ensure ADC reference voltage is correct

### Inaccurate Readings
- Verify thermistor parameters (R₀ and Beta)
- Check reference resistor value
- Apply calibration offset if needed

### Erratic Readings
- Add a capacitor for noise reduction
- Check for loose connections
- Ensure stable power supply

## Examples

The library includes several examples:

- **BasicLowSide**: Simple low-side configuration
- **BasicHighSide**: Simple high-side configuration
- **Calibration**: How to calibrate your sensor

## License

This library is licensed under the MIT License. See the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Version History

- **v1.0.0**: Initial release with full feature set

## Support

If you encounter any issues or have questions, please:
1. Check the examples
2. Review this documentation
3. Open an issue on GitHub
