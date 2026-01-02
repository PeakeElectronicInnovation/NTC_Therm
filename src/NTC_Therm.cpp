/**
 * @file NTC_Therm.cpp
 * @brief Implementation of NTC thermistor library
 * @author Your Name <your.email@example.com>
 * @version 1.0.0
 * @date 2026
 * 
 * License: MIT
 */

#include "NTC_Therm.h"

// Main methods
NTC_Therm::NTC_Therm(int pin, float R_Ref, float R_NTC_25, float Beta, bool isHighSide, uint16_t adcRes, float adcVoltageRef)
    : _pin(pin), _R_Ref(R_Ref), _R_NTC_25(R_NTC_25), _Beta(Beta), _isHighSide(isHighSide), _adcRes(adcRes), _adcMax((1 << adcRes) - 1), _adcVoltageRef(adcVoltageRef) {
}

void NTC_Therm::begin() {
    pinMode(_pin, INPUT);
    analogRead(_pin);
}

float NTC_Therm::temperature() {
    if (!isConnected()) {
        return NAN;
    }
    
    uint32_t adcRaw = 0;

    // Apply oversampling if enabled
    if (_oversamplingEnabled && _oversamplingSamples > 1) {
        for (uint8_t i = 0; i < _oversamplingSamples; i++) {
            adcRaw += analogRead(_pin);
        }
        adcRaw /= _oversamplingSamples;
    } else adcRaw = analogRead(_pin);

    float R_ntc = 0;

    if (!_isHighSide) {
        // Low-side NTC, high-side fixed resistor
        R_ntc = _R_Ref * ((float)adcRaw / (float)(_adcMax - adcRaw));
    } else {
        // High-side NTC, low-side fixed resistor
        R_ntc = _R_Ref * ((float)(_adcMax - adcRaw) / (float)adcRaw);
        
    }
    
    const float T0 = 298.15f; // 25°C in Kelvin
    float invT = (1.0f / T0) + (1.0f / _Beta) * log(R_ntc / _R_NTC_25);
    float T = ((1.0f / invT) - 273.15f - _offsetC) * _scale;

    if (_maEnabled) {
        return movingAverageAddSample(T);
    } else {
        return T;
    }
}

bool NTC_Therm::isConnected() {
    uint16_t adcRaw = analogRead(_pin);
    return (adcRaw > (_adcMax / 20) && adcRaw < (_adcMax - (_adcMax / 20)));
}

// Calibration
void NTC_Therm::setOffset(float offsetC) {
    _offsetC = offsetC;
}

void NTC_Therm::setScale(float scale) {
    _scale = scale;
}

float NTC_Therm::getOffset() {
    return _offsetC;
}

float NTC_Therm::getScale() {
    return _scale;
}

// Filtering
bool NTC_Therm::enableMovingAverage(uint8_t samples) {
    if (samples == 0) {
        disableMovingAverage();
        return false;
    }

    // If size is unchanged, do nothing
    if (_maEnabled && _maSize == samples) {
        return true;
    }

    // Clean up old buffer
    delete[] _maBuffer;
    _maBuffer = nullptr;

    // Allocate new buffer
    _maBuffer = new float[samples];
    if (!_maBuffer) {
        // Allocation failed – fail gracefully
        _maEnabled = false;
        _maSize = 0;
        return false;
    }

    _maSize = samples;
    _maIndex = 0;
    _maCount = 0;
    _maSum = 0.0f;
    _maEnabled = true;

    for (size_t i = 0; i < _maSize; i++) {
        _maBuffer[i] = 0.0f;
    }

    return true;
}

void NTC_Therm::disableMovingAverage() {
    delete[] _maBuffer;
    _maBuffer = nullptr;

    _maEnabled = false;
    _maSize = 0;
    _maIndex = 0;
    _maCount = 0;
    _maSum = 0.0f;
}

bool NTC_Therm::enableOversampling(uint8_t samples) {
    _oversamplingSamples = samples;
    _oversamplingEnabled = true;
    return true;
}

void NTC_Therm::disableOversampling() {
    _oversamplingSamples = 0;
    _oversamplingEnabled = false;
}

// Private
float NTC_Therm::movingAverageAddSample(float newSample) {
    if (!_maEnabled || !_maBuffer) {
        return newSample;
    }

    // Remove old value if buffer is full
    if (_maCount == _maSize) {
        _maSum -= _maBuffer[_maIndex];
    } else {
        _maCount++;
    }

    // Add new value
    _maBuffer[_maIndex] = newSample;
    _maSum += newSample;

    // Advance circular index
    _maIndex++;
    if (_maIndex >= _maSize) {
        _maIndex = 0;
    }

    return _maSum / _maCount;
}