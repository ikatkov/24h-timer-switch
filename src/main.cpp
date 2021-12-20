#include "Arduino.h"
#include "SoftwareSerial.h"
#include <EasyButton.h>
#include <EEPROM.h>

//#define serialON

static const byte SLEEP_INTERVAL = 40;
static const byte CAL_BUTTON_PIN = 0; //pin 5
static const byte LED_PIN1 = 2;       //pin 7

#ifdef serialON
static const byte LED_PIN2 = 2; //fake pint
#else
static const byte LED_PIN2 = 3;   //pin 2
#endif

static const byte LED_PIN3 = 4;   //pin 3
static const byte MOSFET_PIN = 1; //pin 6


static const byte ON = 0; 
static const byte OFF = 1;
byte state = ON;

#ifdef serialON
static const byte TX_PIN = 3; //Pin 1
SoftwareSerial MySerial(-1, TX_PIN);
#endif

EasyButton calButton(CAL_BUTTON_PIN);
byte onTime = 0;
unsigned long turnOnTime;
unsigned long turnOffTime;

void onTurnOff()
{
    #ifdef serialON
    MySerial.print(millis());
    MySerial.print(F("\t"));
    MySerial.println(F("ontTurnOff"));
    #endif
    digitalWrite(MOSFET_PIN, LOW);
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    state = OFF;
}

void configureTurnOffDelay()
{
    #ifdef serialON
    MySerial.println(F("configureTurnOffDelay"));
    #endif
    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    digitalWrite(LED_PIN3, LOW);
    switch (onTime)
    {
    case 0:
    {
        #ifdef serialON
        MySerial.println(F("case 0"));
        #endif
        turnOffTime = millis() + (unsigned long)15 * 1000;
        digitalWrite(LED_PIN1, HIGH);
        break;
    }
    case 1:
    {
        #ifdef serialON
        MySerial.println(F("case 1"));
        #endif
        turnOffTime = millis() + (unsigned long)30 * 1000;
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        break;
    }
    case 2:
    {
        #ifdef serialON
        MySerial.println(F("case 2"));
        #endif
        turnOffTime = millis() + (unsigned long)45 * 1000;
        digitalWrite(LED_PIN1, HIGH);
        digitalWrite(LED_PIN2, HIGH);
        digitalWrite(LED_PIN3, HIGH);
        break;
    }
    }
}

void onNewCycle()
{
    #ifdef serialON
    MySerial.print(millis());
    MySerial.print(F("\t"));
    MySerial.println(F("onNewCycle"));
    #endif
    digitalWrite(MOSFET_PIN, HIGH);
    turnOnTime = millis() + (unsigned long)60*1000;
    configureTurnOffDelay();
    state = ON;
}

void onCalButtonPressed()
{
    #ifdef serialON
    MySerial.print(millis());
    MySerial.print(F("\t"));
    MySerial.println(F("onCalButtonPressed"));
    #endif
    if (state == ON)
    {
        onTime = (onTime + 1) % 3;
        EEPROM.write(1, onTime);
    }
    onNewCycle();
}

void setup()
{
    //this is specific to the ATTiny85 instance
    // see http://becomingmaker.com/tuning-attiny-oscillator/
    OSCCAL -= 3;

    #ifdef serialON
    pinMode(TX_PIN, OUTPUT);
    MySerial.begin(9600);
    MySerial.println(F("setup..."));
    #endif

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
    byte onTime = EEPROM.read(1);
    if (onTime < 0 || onTime > 2)
    {
        onTime = 0;
    }
    configureTurnOffDelay();
    calButton.begin();
    calButton.onPressed(onCalButtonPressed);
    onNewCycle();
}

void loop()
{
    if(state == ON && millis() > turnOffTime){
        onTurnOff();
    }
    if(millis() > turnOnTime){
        onNewCycle();
    }
    calButton.read();
}