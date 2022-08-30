#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define PinLDR A0 // Pin for photoresistor.
#define OneWireBus D4 // Bus of input data from the temperature sensor.
#define LEDBlue D6
#define LEDGreen D7
#define LEDRed D8


void setup_sensors_and_actuators()
{
    pinMode(PinLDR, INPUT);
    pinMode(LEDBlue, OUTPUT);
    pinMode(LEDGreen, OUTPUT);
    pinMode(LEDRed, OUTPUT);
}

// Measure the luminosity using the LDR sensor and returns an integer value.
int get_luminosity()
{
    int value = analogRead(PinLDR);
    return value;
}

// Measures the temperature using the DS18B20 temperature sensor through the one wire bus
// and returns a float value with the temperature in celsius degrees.
float get_temperature()
{
    OneWire oneWireObject(OneWireBus);
    DallasTemperature sensor_temperature(&oneWireObject);
    sensor_temperature.begin();
    sensor_temperature.requestTemperatures();
    float value = sensor_temperature.getTempCByIndex(0);
    return value;
}

// Set a new color to the RGB LED.
void set_color_rgb(int red, int green, int blue)
{
    digitalWrite(LEDRed, red);
    digitalWrite(LEDGreen, green);
    digitalWrite(LEDBlue, blue);
}
