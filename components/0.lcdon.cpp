#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// Pin definitions
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TFT_BACKLIGHT_PIN 25 // Assuming GPIO 25 controls the backlight

// Create objects for the display and touch screen
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

void setup()
{
    Serial.begin(115200);

    // Initialize the TFT backlight pin
    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH); // Turn on the backlight

    // Initialize the TFT display
    tft.init();
    tft.setRotation(1); // Adjust rotation if needed
    tft.fillScreen(TFT_BLACK);

    // Initialize the touch screen
    ts.begin();
    ts.setRotation(1); // Adjust rotation to match TFT rotation

    // Draw some text
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Touch the screen", 10, 10);
}

void loop()
{
    if (ts.touched())
    {
        TS_Point p = ts.getPoint();

        // Scale touch coordinates to screen coordinates
        int16_t x = map(p.x, 0, 240, 0, tft.width());
        int16_t y = map(p.y, 0, 320, 0, tft.height());

        // Draw a dot where touched
        tft.fillCircle(x, y, 5, TFT_RED);

        // Print touch coordinates
        Serial.print("x = ");
        Serial.print(x);
        Serial.print(", y = ");
        Serial.println(y);

        delay(50); // Small delay to avoid multiple readings
    }
}