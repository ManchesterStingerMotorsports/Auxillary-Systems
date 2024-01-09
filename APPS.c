#include "stm32f4xx"

// replace with real pin number
const int sensor1Pin = 0;
const int sensor2Pin = 1;
const int power1Pin = 2;
const int power2Pin = 3;

// Define the range of the sensor output (0-1023 for a 10-bit ADC)
const int sensorRange = 1023;

// Function to read sensor value
int readSensor(int sensorPin) {
    return analogRead(sensorPin);
}

// Function to scale sensor value to pedal travel percentage (0-100%)
float scaleToPercentage(int sensorValue) {
    return static_cast<float>(sensorValue) / sensorRange * 100.0;
}

// Function to check for implausibility
bool checkImplausibility(float value1, float value2) {
    // Check for a deviation of more than ten percentage points
    return abs(value1 - value2) > 10.0;
}

// Function to shut down power in case of implausibility
void shutDownPower() {
}

int main() {
    // Read sensor values
    int sensor1Value = readSensor(sensor1Pin);
    int sensor2Value = readSensor(sensor2Pin);

    // Scale sensor values to percentage
    float pedalPosition1 = scaleToPercentage(sensor1Value);
    float pedalPosition2 = scaleToPercentage(sensor2Value);

    // Check for implausibility
    if (checkImplausibility(pedalPosition1, pedalPosition2)) {
        shutDownPower();
    }

    return 0;
}
