#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Pin definitions for Touch
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TOUCH_MOSI 2
#define TOUCH_MISO 26
#define TOUCH_CLK 32
#define TFT_BACKLIGHT_PIN 25

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Button dimensions
#define BTN_WIDTH 120
#define BTN_HEIGHT 40

// Screen state
uint8_t currentScreen = 0; // 0 = main screen, 1 = second screen

// Initialize TFT and Touch objects
TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// Variables for touch handling
uint16_t touchX = 0, touchY = 0;
bool wasTouched = false;

// Button structure
struct Button
{
    int x;
    int y;
    int width;
    int height;
    const char *label;
};

// Define start button
Button startButton = {
    (SCREEN_WIDTH - BTN_WIDTH) / 2,   // x centered
    (SCREEN_HEIGHT - BTN_HEIGHT) / 2, // y centered
    BTN_WIDTH,
    BTN_HEIGHT,
    "MULAI"};

// Function to check if a point is inside a button
bool isInButton(int x, int y, Button btn)
{
    return (x >= btn.x && x <= (btn.x + btn.width) &&
            y >= btn.y && y <= (btn.y + btn.height));
}

// Function to draw a button
void drawButton(Button btn, uint16_t color)
{
    tft.fillRoundRect(btn.x, btn.y, btn.width, btn.height, 8, color);
    tft.drawRoundRect(btn.x, btn.y, btn.width, btn.height, 8, TFT_WHITE);

    // Calculate text position to center it in button
    int textWidth = strlen(btn.label) * 12; // Approximate width of text
    int textX = btn.x + (btn.width - textWidth) / 2;
    int textY = btn.y + (btn.height - 16) / 2; // 16 is approximate text height

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString(btn.label, textX, textY);
}

// Draw the main screen (Screen 0)
void drawMainScreen()
{
    tft.fillScreen(TFT_BLACK);
    drawButton(startButton, TFT_BLUE);
}

// Draw the second screen (Screen 1)
void drawSecondScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Layar Kedua", 10, 10);
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

    // Handle touch based on current screen
    if (currentScreen == 0)
    {
        // Check if start button was pressed
        if (isInButton(touchX, touchY, startButton))
        {
            // Visual feedback
            drawButton(startButton, TFT_RED);
            delay(100);

            // Change to second screen
            currentScreen = 1;
            drawSecondScreen();
        }
    }
}

void setup()
{
    Serial.begin(115200);

    // Initialize main SPI for TFT
    SPI.begin();

    // Initialize the TFT backlight pin
    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

    // Initialize TFT
    tft.init();
    tft.setRotation(1); // Landscape
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);

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

    // Draw initial screen
    drawMainScreen();

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