// library utama
#include <Arduino.h>

// global variable indicatorLed
int delayTime = 250;

void blink()
{
    digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
    Serial.println("hidup \tLED_BUILTIN ");
    delay(delayTime);               // wait for a second
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
    Serial.println("mati \tLED_BUILTIN ");
    delay(delayTime); // wait for a second
}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop()
{
    blink();
}
