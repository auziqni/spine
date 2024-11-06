
// library utama
#include <Arduino.h>

// library lcd_TFT
#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Global variable and declaration
//-----------------------------------------------------------------------------------------------
// Pin lcd_TFT
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TOUCH_MOSI 2
#define TOUCH_MISO 26
#define TOUCH_CLK 32
#define TFT_BACKLIGHT_PIN 25
TFT_eSPI tft = TFT_eSPI(); // Initialize TFT and Touch objects
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// Analog pins
#define ANALOG_PIN1 33
#define ANALOG_PIN2 34
#define ANALOG_PIN3 35
#define ANALOG_PIN4 36

// global variable Utama
unsigned int currentTimer = 0, timer = 0, timerinterval = 1000;

// global variable lcd_TFT
#define SCREEN_WIDTH 320 // Screen dimensions
#define SCREEN_HEIGHT 240

#define TERM_X 10 // Terminal dimensions
#define TERM_Y 10
#define TERM_W 300
#define TERM_H 160
#define TERM_HEADER_H 20

#define BTN_WIDTH 100 // Button dimensions
#define BTN_HEIGHT 35
#define BTN_Y 190

uint8_t currentScreen = 0; // Screen state  // 0 = main, 1 = terminal, 2 = result

uint16_t touchX = 0, touchY = 0; // Variables for touch handling
bool wasTouched = false;

// global variable analog
unsigned long lastAnalogRead = 0;                // Variables for analog reading
const unsigned long ANALOG_READ_INTERVAL = 1000; // Read every 1 second

// fungsi pendukung
//-----------------------------------------------------------------------------------------------
// class & struct begin --------------------------------
class Terminal // class terminal
{
private:
    static const int MAX_LINES = 8;
    String lines[MAX_LINES];
    int currentLine = 0;
    int lineHeight = 15;

public:
    void addLine(String text)
    {
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
        tft.fillRect(TERM_X, TERM_Y + TERM_HEADER_H, TERM_W, TERM_H - TERM_HEADER_H, TFT_BLACK);
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

struct AnalogData // Struct untuk menyimpan data analog
{
    int value1;
    int value2;
    int value3;
    int value4;
    long processResult;
} lastAnalogData;

struct Button // Button structure
{
    int x;
    int y;
    int width;
    int height;
    const char *label;
};
// class & struct end --------------------------------

// Define buttons begin --------------------------------
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

Button backButton = {
    (SCREEN_WIDTH - BTN_WIDTH) / 2,
    SCREEN_HEIGHT - BTN_HEIGHT - 10,
    BTN_WIDTH,
    BTN_HEIGHT,
    "BACK"};

// Define buttons end --------------------------------

// Function to draw button
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

// Draw terminal screen
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
    lastAnalogRead = millis();
}

// Draw result screen
void drawResultScreen()
{
    tft.fillScreen(TFT_BLACK);

    // Create result box
    int boxWidth = 280;
    int boxHeight = 120;
    int boxX = (SCREEN_WIDTH - boxWidth) / 2;
    int boxY = (SCREEN_HEIGHT - boxHeight) / 2;

    // Draw box
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, TFT_WHITE);

    // Header
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Process Result:", boxX + 10, boxY + 10);

    // Show input data
    tft.setTextSize(1);
    char buffer[50];
    sprintf(buffer, "Input: %d * %d * %d * %d",
            lastAnalogData.value1, lastAnalogData.value2,
            lastAnalogData.value3, lastAnalogData.value4);
    tft.drawString(buffer, boxX + 10, boxY + 40);

    // Show result
    tft.setTextSize(2);
    tft.setTextColor(TFT_GREEN);
    String result = String(lastAnalogData.processResult);
    int textX = boxX + (boxWidth - result.length() * 12) / 2;
    tft.drawString(result, textX, boxY + 70);

    // Draw back button
    drawButton(backButton, TFT_BLUE);
}

// fungsi komponen
//-----------------------------------------------------------------------------------------------
// Process the analog data
void processData()
{
    lastAnalogData.processResult = (long)lastAnalogData.value1 *
                                   (long)lastAnalogData.value2 *
                                   (long)lastAnalogData.value3 *
                                   (long)lastAnalogData.value4;

    Serial.println("Process Result: " + String(lastAnalogData.processResult));
}

// Function to check button press
bool isInButton(int x, int y, Button btn)
{
    return (x >= btn.x && x <= (btn.x + btn.width) &&
            y >= btn.y && y <= (btn.y + btn.height));
}
// Function to read and display analog values
void readAndDisplayAnalog()
{

    // Save to global variable
    lastAnalogData.value1 = analogRead(ANALOG_PIN1);
    lastAnalogData.value2 = analogRead(ANALOG_PIN2);
    lastAnalogData.value3 = analogRead(ANALOG_PIN3);
    lastAnalogData.value4 = analogRead(ANALOG_PIN4);
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
            terminal.addLine("Processing data...");
            processData();
            delay(500);
            currentScreen = 2;
            drawResultScreen();
        }
    }
    else if (currentScreen == 2)
    {
        if (isInButton(touchX, touchY, backButton))
        {
            drawButton(backButton, TFT_RED);
            delay(100);
            currentScreen = 0;
            tft.fillScreen(TFT_BLACK);
            drawButton(startButton, TFT_BLUE);
        }
    }
}

// fungsi monitoring
//-----------------------------------------------------------------------------------------------

void terminalWrite()
{
    char buffer[50];
    // Format current time
    unsigned long seconds = millis() / 1000;
    sprintf(buffer, "Time: %03lu s", seconds);
    terminal.addLine(buffer);

    // Display readings
    sprintf(buffer, "A1:%04d  A2:%04d", lastAnalogData.value1, lastAnalogData.value2);
    terminal.addLine(buffer);
    sprintf(buffer, "A3:%04d  A4:%04d", lastAnalogData.value3, lastAnalogData.value4);
    terminal.addLine(buffer);
    terminal.addLine("----------------");
}
void monitorSerial()
{
    Serial.println("Hello World");
}

// fungsi utama
//-----------------------------------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);

    // Initialize analog pins
    pinMode(ANALOG_PIN1, INPUT);
    pinMode(ANALOG_PIN2, INPUT);
    pinMode(ANALOG_PIN3, INPUT);
    pinMode(ANALOG_PIN4, INPUT);

    // Initialize SPI
    SPI.begin();

    // Initialize backlight
    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

    // Initialize TFT
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Initialize touch
    touchSPI.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS_PIN);

    if (!ts.begin(touchSPI))
    {
        Serial.println("Couldn't start touchscreen controller");
        tft.drawString("Touch Error!", 10, 10);
        while (1)
            ;
    }

    ts.setRotation(3);

    // Draw initial screen
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
            terminalWrite();
            lastAnalogRead = currentTime;
        }
    }

    delay(50);
}