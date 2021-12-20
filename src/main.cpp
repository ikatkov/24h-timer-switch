#include "Arduino.h"
#include "SoftwareSerial.h"
#include <EasyButton.h>
#include <EEPROM.h>
#include "Countimer.h"

static const byte SLEEP_INTERVAL = 40;
static const byte CAL_BUTTON_PIN = 0; //pin 5
static const byte LED_PIN1 = 2;       //pin 7

//TODO change to LED_PIN2 = 3;
static const byte LED_PIN2 = 2;   //pin 2
static const byte LED_PIN3 = 4;   //pin 3
static const byte MOSFET_PIN = 1; //pin 6

static const byte TX_PIN = 3; //Pin 1
SoftwareSerial MySerial(-1, TX_PIN);

EasyButton calButton(CAL_BUTTON_PIN);
Countimer t24cycle;
Countimer tTurnOff;
byte onTime = 1;

void onTurnOff()
{
    MySerial.println(F("ontTurnOff"));
    digitalWrite(MOSFET_PIN, LOW);
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
}

void configureTurnOffDelay()
{
    MySerial.println(F("configureTurnOffDelay"));
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    switch (onTime)
    {
    case 0:
    {
        MySerial.println(F("case 0"));
        tTurnOff.setCounter(0, 0, 15, CountType::COUNT_DOWN, onTurnOff);
        digitalWrite(LED_PIN1, HIGH);
        break;
    }
    case 1:
    {
        MySerial.println(F("case 1"));
        tTurnOff.setCounter(0, 0, 30, CountType::COUNT_DOWN, onTurnOff);
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        break;
    }
    case 2:
    {
        MySerial.println(F("case 2"));
        tTurnOff.setCounter(0, 0, 45, CountType::COUNT_DOWN, onTurnOff);
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        digitalWrite(LED_PIN3, HIGH);
        break;
    }
    }
    tTurnOff.restart();
}

void onNewCycle()
{
    MySerial.println(F("onNewCycle"));
    digitalWrite(MOSFET_PIN, HIGH);
    t24cycle.restart();
    configureTurnOffDelay();
}

void onCalButtonPressed()
{
    MySerial.println(F("onCalButtonPressed"));
    if (tTurnOff.isCounterRunning())
    {
        onTime = (onTime + 1) % 3;
        EEPROM.write(0, onTime);
        configureTurnOffDelay();
    }
    else
    {
        onNewCycle();
    }
}

void setup()
{
    //this is specific to the ATTiny85 instance
    // see http://becomingmaker.com/tuning-attiny-oscillator/
    OSCCAL -= 3;

    pinMode(TX_PIN, OUTPUT);
    MySerial.begin(9600);
    MySerial.println(F("setup..."));

    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(LED_PIN3, OUTPUT);
    pinMode(MOSFET_PIN, OUTPUT);
    pinMode(CAL_BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(LED_PIN1, HIGH);
    delay(200);
    digitalWrite(LED_PIN2, HIGH);
    delay(200);
    digitalWrite(LED_PIN3, HIGH);
    delay(200);
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    //read from eeprom here
    byte onTime = EEPROM.read(0);
    if (onTime < 0 || onTime > 2)
    {
        onTime = 0;
    }
    configureTurnOffDelay();
    calButton.begin();
    calButton.onPressed(onCalButtonPressed);

    t24cycle.setCounter(0, 1, 0, CountType::COUNT_DOWN, onNewCycle);
    onNewCycle();
}

void loop()
{
    t24cycle.run();
    tTurnOff.run();
    calButton.read();
}