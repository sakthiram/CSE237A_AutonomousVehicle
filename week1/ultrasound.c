#include <stdio.h>
#include <wiringPi.h>

#define GPIO22     3
#define GPIO23     4

static int trigger = GPIO22;
static int echo = GPIO23;

static volatile long startTimeUsec;
static volatile long endTimeUsec;

void recordPulseLength (void) {
    startTimeUsec = micros();
    while ( digitalRead(echo) == HIGH );
    endTimeUsec = micros();
}


/**
 * Uses the HC SR04 ultrasonic sensor to measure distance. The HC SR04
 * provides 2cm to 400cm range measurement.
 */
int main()
{
    double speedOfSoundMetersPerSecond = 340.29;

    wiringPiSetup();

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    while(1){	
    // Initialize the sensor's trigger pin to low. If we don't pause
    // after setting it to low, sometimes the sensor doesn't work right.
    digitalWrite(trigger, LOW);
    delay(500); // .5 seconds

    // Triggering the sensor for 10 microseconds will cause it to send out
    // 8 ultrasonic (40Khz) bursts and listen for the echos.
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    // The sensor will raise the echo pin high for the length of time that it took
    // the ultrasonic bursts to travel round trip.
    while ( digitalRead(echo) == LOW);
    recordPulseLength();

    long travelTimeUsec = endTimeUsec - startTimeUsec;
    double distanceMeters = ((travelTimeUsec/1000000.0)*speedOfSoundMetersPerSecond)/2;

    printf("Distance is %f cm.",(distanceMeters/100));
    delay(1000);
    }	
    return 0;
}
