#include "Arduino.h"
#include "SoftwareSerial.h"
#include <EasyButton.h>

static const byte CAL_BUTTON_PIN = 5;
static const byte LED_PIN1 = 2;
static const byte LED_PIN2 = 3;
static const byte LED_PIN3 = 4;
#define RX_PIN 0 // D3, Pin 2 
#define TX_PIN 1 // D4, Pin 3

/* rx = 3, tx = 4 */
SoftwareSerial MySerial(RX_PIN, TX_PIN);

void setup()
{
    MySerial.begin(9600);
    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);    
    MySerial.println("setup.......");
}

void loop()
{
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_PIN1, HIGH);
    digitalWrite(LED_PIN2, HIGH);
    digitalWrite(LED_PIN3, HIGH);    
    MySerial.println("aaaaa");

    // wait for a second
    delay(1000);

    // turn the LED off by making the voltage LOW
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);        
    MySerial.println("cccc");

    // wait for a second
    delay(1000);
}