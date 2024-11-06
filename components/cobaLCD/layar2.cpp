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

// Analog pins
#define ANALOG_PIN1 33 // Contoh pin analog 1
#define ANALOG_PIN2 34 // Contoh pin analog 2
#define ANALOG_PIN3 35 // Contoh pin analog 3
#define ANALOG_PIN4 36 // Contoh pin analog 4

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Terminal dimensions
#define TERM_X 10
#define TERM_Y 10
#define TERM_W 300
#define TERM_H 160
#define TERM_HEADER_H 20

// Button dimensions
#define BTN_WIDTH 100
#define BTN_HEIGHT 35
#define BTN_Y 190

// Screen state
uint8_t currentScreen = 0; // 0 = main, 1 = terminal, 2 = next screen

// Initialize TFT and Touch objects
TFT_eSPI tft = TFT_eSPI();
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// Variables for touch handling
uint16_t touchX = 0, touchY = 0;
bool wasTouched = false;

// Variables for analog reading
unsigned long lastAnalogRead = 0;
const unsigned long ANALOG_READ_INTERVAL = 1000; // Read every 1 second

// Button structure
struct Button
{
    int x;
    int y;
    int width;
    int height;
    const char *label;
};

// Define buttons
Button startButton = {
    (SCREEN_WIDTH - BTN_WIDTH) / 2,
    (SCREEN_HEIGHT - BTN_HEIGHT) / 2,
    BTN_WIDTH,
    BTN_HEIGHT,
    "MULAI"};

Button nextButton = {
    (SCREEN_WIDTH - BTN_WIDTH) / 2,
    BTN_Y,
    BTN_WIDTH,
    BTN_HEIGHT,
    "NEXT >>"};

// Terminal class to handle text display
class Terminal
{
private:
    static const int MAX_LINES = 8; // Reduced to prevent overflow
    String lines[MAX_LINES];
    int currentLine = 0;
    int lineHeight = 15;

public:
    void addLine(String text)
    {
        // Scroll if needed
        if (currentLine >= MAX_LINES)
        {
            for (int i = 0; i < MAX_LINES - 1; i++)
            {
                lines[i] = lines[i + 1];
            }
            currentLine = MAX_LINES - 1;
        }

        lines[currentLine++] = text;
        updateDisplay();
    }

    void updateDisplay()
    {
        // Clear terminal area
        tft.fillRect(TERM_X, TERM_Y + TERM_HEADER_H, TERM_W, TERM_H - TERM_HEADER_H, TFT_BLACK);

        // Draw text
        tft.setTextColor(TFT_GREEN);
        tft.setTextSize(1);

        for (int i = 0; i < currentLine; i++)
        {
            tft.drawString(lines[i], TERM_X + 5, TERM_Y + TERM_HEADER_H + (i * lineHeight));
        }
    }

    void clear()
    {
        currentLine = 0;
        tft.fillRect(TERM_X, TERM_Y + TERM_HEADER_H, TERM_W, TERM_H - TERM_HEADER_H, TFT_BLACK);
    }
} terminal;

// Function to read analog values and update terminal
void readAndDisplayAnalog()
{
    char buffer[50];
    int analog1 = analogRead(ANALOG_PIN1);
    int analog2 = analogRead(ANALOG_PIN2);
    int analog3 = analogRead(ANALOG_PIN3);
    int analog4 = analogRead(ANALOG_PIN4);

    // Format current time (seconds since start)
    unsigned long seconds = millis() / 1000;
    sprintf(buffer, "Time: %03lu s", seconds);
    terminal.addLine(buffer);

    // Format and display analog readings
    sprintf(buffer, "A1:%04d  A2:%04d ininininininininininininininiin", analog1, analog2);
    terminal.addLine(buffer);
    sprintf(buffer, "A3:%04d  A4:%04d", analog3, analog4);
    terminal.addLine(buffer);
    terminal.addLine("----------------");
}

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

    int textWidth = strlen(btn.label) * 12;
    int textX = btn.x + (btn.width - textWidth) / 2;
    int textY = btn.y + (btn.height - 16) / 2;

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString(btn.label, textX, textY);
}

// Draw terminal screen (Screen 1)
void drawTerminalScreen()
{
    tft.fillScreen(TFT_BLACK);

    // Draw terminal border
    tft.drawRect(TERM_X, TERM_Y, TERM_W, TERM_H, TFT_WHITE);

    // Draw terminal header
    tft.fillRect(TERM_X, TERM_Y, TERM_W, TERM_HEADER_H, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(1);
    tft.drawString("Analog Monitor", TERM_X + 5, TERM_Y + 5);

    // Draw next button
    drawButton(nextButton, TFT_BLUE);

    // Initialize terminal
    terminal.clear();
    terminal.addLine("Monitoring started...");
    lastAnalogRead = millis(); // Reset timing
}

void processTouch()
{
    TS_Point p = ts.getPoint();

    touchX = map(p.x, 200, 3700, 0, SCREEN_WIDTH);
    touchY = map(p.y, 240, 3800, 0, SCREEN_HEIGHT);

    touchX = constrain(touchX, 0, SCREEN_WIDTH);
    touchY = constrain(touchY, 0, SCREEN_HEIGHT);

    if (currentScreen == 0)
    {
        if (isInButton(touchX, touchY, startButton))
        {
            drawButton(startButton, TFT_RED);
            delay(100);
            currentScreen = 1;
            drawTerminalScreen();
        }
    }
    else if (currentScreen == 1)
    {
        if (isInButton(touchX, touchY, nextButton))
        {
            drawButton(nextButton, TFT_RED);
            delay(100);
            terminal.addLine("Moving to next screen...");
            delay(500);
            currentScreen = 2;
            tft.fillScreen(TFT_BLACK);
            tft.drawString("Screen 3", 10, 10);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    // Initialize analog pins
    pinMode(ANALOG_PIN1, INPUT);
    pinMode(ANALOG_PIN2, INPUT);
    pinMode(ANALOG_PIN3, INPUT);
    pinMode(ANALOG_PIN4, INPUT);

    // Initialize main SPI for TFT
    SPI.begin();

    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS_PIN);

    if (!ts.begin(touchSPI))
    {
        Serial.println("Couldn't start touchscreen controller");
        tft.drawString("Touch Error!", 10, 10);
        while (1)
            ;
    }

    ts.setRotation(3);

    drawButton(startButton, TFT_BLUE);

    Serial.println("Setup completed!");
}

void loop()
{
    if (ts.touched())
    {
        if (!wasTouched)
        {
            wasTouched = true;
            processTouch();
        }
    }
    else
    {
        wasTouched = false;
    }

    // Update analog readings when in terminal screen
    if (currentScreen == 1)
    {
        unsigned long currentTime = millis();
        if (currentTime - lastAnalogRead >= ANALOG_READ_INTERVAL)
        {
            readAndDisplayAnalog();
            lastAnalogRead = currentTime;
        }
    }

    delay(50);
}