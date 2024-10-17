
// library utama
#include <Arduino.h>

// Global variable and declaration
//-----------------------------------------------------------------------------------------------
// declare <component>

// global variable Utama
unsigned int currentTimer = 0, timer = 0, timerinterval = 1000;

// global variable <component>

// fungsi dari komponen
//-----------------------------------------------------------------------------------------------

void initComponent()
{
}
void component()
{
}

// fungsi pendukung
//-----------------------------------------------------------------------------------------------

void monitorSerial()
{
    Serial.println("Hello World");
}
// fungsi utama
//-----------------------------------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    // function to read along
    currentTimer = millis();
    component();
    // fuction to run at interval
    if ((currentTimer - timer) > timerinterval)
    {
        timer = currentTimer;
        monitorSerial();
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
}
