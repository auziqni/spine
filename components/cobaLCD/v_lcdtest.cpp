#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Pin definitions for Touch
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TOUCH_MOSI 2
#define TOUCH_MISO 26
#define TOUCH_CLK 32
#define TFT_BACKLIGHT_PIN 25 // Assuming GPIO 25 controls the backlight

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Initialize TFT and Touch objects
TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// Variables for touch handling
uint16_t touchX = 0, touchY = 0;
bool wasTouched = false;
void updateDisplay()
{
    // Clear previous coordinates
    tft.fillRect(10, 30, 200, 20, TFT_BLACK);

    // Display new coordinates
    tft.setCursor(10, 30);
    tft.print("X:");
    tft.print(touchX);
    tft.print(" Y:");
    tft.print(touchY);

    // Draw touch point
    tft.fillCircle(touchX, touchY, 3, TFT_RED);

    // Optional: fade out previous touch point after a delay
    delay(50);
    tft.fillCircle(touchX, touchY, 3, TFT_DARKGREY);
}

void processTouch()
{
    TS_Point p = ts.getPoint();

    // Map touch coordinates to screen coordinates
    touchX = map(p.x, 200, 3700, 0, SCREEN_WIDTH);
    touchY = map(p.y, 240, 3800, 0, SCREEN_HEIGHT);

    // Constrain values to screen dimensions
    touchX = constrain(touchX, 0, SCREEN_WIDTH);
    touchY = constrain(touchY, 0, SCREEN_HEIGHT);

    // Print debug information
    Serial.print("Touch at x=");
    Serial.print(touchX);
    Serial.print(", y=");
    Serial.println(touchY);

    Serial.print("Raw values: x=");
    Serial.print(p.x);
    Serial.print(", y=");
    Serial.print(p.y);
    Serial.print(", z=");
    Serial.println(p.z);

    // Update display
    updateDisplay();
}

void drawInterface()
{
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Touch Screen Test", 10, 10);
    tft.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TFT_WHITE);

    // Draw touch area
    tft.drawRect(10, 50, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 60, TFT_BLUE);
}

void setup()
{
    Serial.begin(115200);

    // Initialize main SPI for TFT
    SPI.begin();

    // Initialize the TFT backlight pin
    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH); // Turn on the backlight

    // Initialize TFT
    tft.init();
    tft.setRotation(1); // Landscape
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Initialize touch SPI
    touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS_PIN);

    // Initialize touch screen
    if (!ts.begin(touchSPI))
    {
        Serial.println("Couldn't start touchscreen controller");
        tft.drawString("Touch Error!", 10, 10);
        while (1)
            ;
    }

    ts.setRotation(3); // Match with TFT rotation

    // Draw initial interface
    drawInterface();

    Serial.println("Setup completed!");
}

void loop()
{
    if (ts.touched())
    {
        if (!wasTouched)
        { // Just touched
            wasTouched = true;
            processTouch();
        }
    }
    else
    {
        wasTouched = false;
    }
    delay(50); // Debounce delay
}
