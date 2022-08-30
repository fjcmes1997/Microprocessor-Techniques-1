#include <Servo.h>
#include <Arduino.h>


#define UltrasonicEcho D1
#define UltrasonicTrigger D3
#define PinServo D2
#define PinLED D6

Servo myservo;


void setup_sensors_and_actuators()
{
    myservo.attach(PinServo);
    pinMode(UltrasonicTrigger, OUTPUT);
    pinMode(UltrasonicEcho, INPUT);
    pinMode(PinLED, OUTPUT);
}

// Moves the servo to the specified position.
void move_servo(int new_position)
{
    myservo.write(new_position);
}

// Measures the distance in centimeters using the ultrasonic sensor and returns a float value.
float get_distance_sonar()
{
    float duration; // Duration of sound wave travel
    float distance; // Distance measurement
    // Clears the trigger pin condition
    digitalWrite(UltrasonicTrigger, LOW);
    delayMicroseconds(2);
    // Active the trigger pin for 10 ms
    digitalWrite(UltrasonicTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(UltrasonicTrigger, LOW);
    // Reads the echo pin and gets the sound wave travel time in ms
    duration = pulseIn(UltrasonicEcho, HIGH);
    // Calculate the distance in cm
    distance = duration * 0.034 / 2;
    return distance;
}

void show_number(int counter)
{
    for (int i=0; i<counter; i++) {
        digitalWrite(PinLED, HIGH);
        delay(250);
        digitalWrite(PinLED, LOW);
        delay(250);
    }
}