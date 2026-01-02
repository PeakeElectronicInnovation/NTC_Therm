/**
 * @file NTC_Therm.h
 * @brief Arduino library for reading NTC thermistors in voltage divider configurations
 * @author Your Name <your.email@example.com>
 * @version 1.0.0
 * @date 2026
 * 
 * This library provides accurate temperature readings from NTC thermistors using 
 * the Beta parameter equation. It supports both low-side and high-side voltage 
 * divider configurations with configurable ADC resolution and reference voltage.
 * 
 * License: MIT
 */

#pragma once
#include <Arduino.h>

// NTC Thermistor class - to read temperature from NTC thermistor voltage divider
// For lowside and highside configurations
// Lowside configuration: NTC connected between ADC input and ground, reference resistor between Vref and ADC input
// 
//  +Vref
//    |
// [R_Ref]
//    |─── ADC Input Pin
//  [NTC]
//   _|_
//   GND
//
// Highside configuration: NTC connected between Vref and ADC input, reference resistor between ADC input and ground
//
//  +Vref
//    |
//  [NTC]
//    |─── ADC Input Pin
// [R_Ref]
//   _|_
//   GND

class NTC_Therm {
public:
    // Main methods
    NTC_Therm(int pin, float R_Ref, float R_NTC_25, float Beta, bool isHighSide, uint16_t adcRes, float adcVoltageRef);
    void begin();
    float temperature();
    bool isConnected();

    // Calibration
    void setOffset(float offsetC);
    void setScale(float scale);
    float getOffset();
    float getScale();

    // Filtering
    bool enableMovingAverage(uint8_t samples);
    void disableMovingAverage();
    bool enableOversampling(uint8_t samples);
    void disableOversampling();

private:
    // Methods
    float movingAverageAddSample(float newSample);

    // Variables
    int _pin;
    float _R_Ref;
    float _R_NTC_25;
    float _Beta;
    bool _isHighSide;
    uint16_t _adcRes;
    uint32_t _adcMax;
    float _adcVoltageRef;

    float _offsetC = 0.0;
    float _scale = 1.0;

    uint8_t _oversamplingSamples = 0;
    bool _oversamplingEnabled = false;

    float* _maBuffer = nullptr;
    size_t _maSize = 0;
    size_t _maIndex = 0;
    size_t _maCount = 0;
    float _maSum = 0.0f;
    bool _maEnabled = false;
};