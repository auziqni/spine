#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Pin definitions for Touch
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TOUCH_MOSI 2
#define TOUCH_MISO 26
#define TOUCH_CLK 32

// Initialize touch screen with custom SPI settings
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// Screen dimensions (adjust these to match your TFT screen)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

void setup()
{
    Serial.begin(115200);

    // Initialize touch SPI
    touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS_PIN);

    // Initialize touch screen
    if (!ts.begin(touchSPI))
    {
        Serial.println("Couldn't start touchscreen controller");
        while (1)
            ;
    }

    ts.setRotation(1); // Adjust this based on your screen orientation

    Serial.println("Touchscreen initialized");
}

void loop()
{
    if (ts.touched())
    {
        TS_Point p = ts.getPoint();

        // Map touch coordinates to screen coordinates
        int16_t x = map(p.x, 200, 3700, 0, SCREEN_WIDTH);
        int16_t y = map(p.y, 200, 3700, 0, SCREEN_HEIGHT);

        // Print touch location
        Serial.print("Touch at x=");
        Serial.print(x);
        Serial.print(", y=");
        Serial.println(y);

        // Print raw values
        Serial.print("Raw values: x=");
        Serial.print(p.x);
        Serial.print(", y=");
        Serial.print(p.y);
        Serial.print(", z=");
        Serial.println(p.z);

        delay(50); // Debounce delay
    }
}
