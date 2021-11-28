#include "Arduino.h"
#include "SoftwareSerial.h"

#define LED_PIN 3
#define RX_PIN 0 // D3, Pin 2 
#define TX_PIN 1 // D4, Pin 3

/* rx = 3, tx = 4 */
SoftwareSerial MySerial(RX_PIN, TX_PIN);

void setup()
{
    MySerial.begin(9600);
    // initialize LED digital pin as an output.
    pinMode(LED_PIN, OUTPUT);
    MySerial.println("setup.......");
}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_PIN, HIGH);
    MySerial.println("aaaaa");

    // wait for a second
    delay(1000);

    // turn the LED off by making the voltage LOW
    digitalWrite(LED_PIN, LOW);
    MySerial.println("cccc");

    // wait for a second
    delay(1000);
}