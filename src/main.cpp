
// library utama
#include <Arduino.h>
#include <Fuzzy.h>

// libeary mpu6050
#include <Wire.h>
#include <MPU6050_light.h>

// library lcd_TFT
#include <TFT_eSPI.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Global variable and declaration
//-----------------------------------------------------------------------------------------------
// mpu6050
#define TCA9548A_ADDR 0x70

// lcd_TFT
#define TOUCH_CS_PIN 15
#define TOUCH_IRQ_PIN 27
#define TOUCH_MOSI 2
#define TOUCH_MISO 26
#define TOUCH_CLK 32
#define TFT_BACKLIGHT_PIN 25
TFT_eSPI tft = TFT_eSPI(); // Initialize TFT and Touch objects
SPIClass touchSPI = SPIClass(VSPI);
XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

// global variable Utama
unsigned long timerColdstart = 0, intervalTimerColdstart = 10000;
unsigned long timerSerialMOnitor = 0;
unsigned long timerReadMpu = 0, intervalTimerReadMpu = 100;
int hasilCompareFuzzy = 0;
float nilai_normalis, nilai_lordosis, nilai_kifosis, nilai_skoliosis;
String hasilAnalisis[4] = {"normal", "lordosis", "kifosis", "skoliosis"};
float cervival_x = 0;
float thoracal_x = 0, thoracal_z = 0;
float lumbal_x = 0, lumbal_z = 0;
float sacrum_x = 0;

Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput1 ccx
FuzzySet *normal1 = new FuzzySet(73, 82, 90, 90);
FuzzySet *kelainan1 = new FuzzySet(50, 50, 73, 82);

// FuzzyInput2 tcx
FuzzySet *normal2 = new FuzzySet(63, 74.5, 90, 90);
FuzzySet *kelainan2 = new FuzzySet(40, 40, 63, 74.5);

// FuzzyInput3 tcz
FuzzySet *normal3 = new FuzzySet(-17, -5.5, -5.5, 17);
FuzzySet *kelainankanan3 = new FuzzySet(5.5, 17, 40, 40);
FuzzySet *kelainankiri3 = new FuzzySet(-40, -40, -17, -5.5);

// FuzzyInput4 lcx
FuzzySet *normal4 = new FuzzySet(73, 82, 90, 90);
FuzzySet *kelainan4 = new FuzzySet(50, 50, 73, 82);

// FuzzyInput5 lcz
FuzzySet *normal5 = new FuzzySet(-17, -5.5, -5.5, 17);
FuzzySet *kelainankanan5 = new FuzzySet(5.5, 17, 40, 40);
FuzzySet *kelainankiri5 = new FuzzySet(-40, -40, -17, -5.5);

// FuzzyInput6 tcx
FuzzySet *normal6 = new FuzzySet(63, 74.5, 90, 90);
FuzzySet *kelainan6 = new FuzzySet(40, 40, 63, 74.5);

// FuzzyOutput
FuzzySet *normalis = new FuzzySet(0, 0, 8, 12.75);
FuzzySet *lordosis = new FuzzySet(8, 12.75, 18.25, 22.75);
FuzzySet *kifosis = new FuzzySet(18.25, 22.75, 28.25, 32.75);
FuzzySet *skoliosis = new FuzzySet(28.25, 32.75, 40, 40);

// global variable mp6050
const int n_sensor = 4;
const int n_ch = n_sensor + 1;
float angle_x[n_ch] = {0}, angle_y[n_ch] = {0}, angle_z[n_ch] = {0};
MPU6050 mpu[n_ch] = {MPU6050(Wire), MPU6050(Wire), MPU6050(Wire), MPU6050(Wire), MPU6050(Wire)}; // disesuaikan manual dengan jumlah sensor
// String hasilAnalisis = "Amnesaia";

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
void fuzzyRule()
{
  // Building FuzzyRule 1
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

  FuzzyRuleConsequent *thenHasilNormal = new FuzzyRuleConsequent();
  thenHasilNormal->addOutput(normalis);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilNormal);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Building FuzzyRule 2
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

  FuzzyRuleConsequent *thenHasilKifosis = new FuzzyRuleConsequent();
  thenHasilKifosis->addOutput(kifosis);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
  fuzzy->addFuzzyRule(fuzzyRule2);

  // Building FuzzyRule 3
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilKifosis= new FuzzyRuleConsequent();
  thenHasilKifosis->addOutput(kifosis);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
  fuzzy->addFuzzyRule(fuzzyRule3);

  // Building FuzzyRule 4
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilKifosis= new FuzzyRuleConsequent();
  thenHasilKifosis->addOutput(kifosis);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
  fuzzy->addFuzzyRule(fuzzyRule4);

  // Building FuzzyRule 5
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5, kelainan6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilKifosis->addOutput(kifosis);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6, thenHasilKifosis);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Building FuzzyRule 6
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5, normal6);

  FuzzyRuleConsequent *thenHasilLordosis = new FuzzyRuleConsequent();
  thenHasilLordosis->addOutput(lordosis);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilLordosis);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Building FuzzyRule 7
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5, kelainan6);

  // FuzzyRuleConsequent *thenHasilLordosis= new FuzzyRuleConsequent();
  thenHasilLordosis->addOutput(lordosis);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilLordosis);
  fuzzy->addFuzzyRule(fuzzyRule7);

  // Building FuzzyRule 8
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, kelainan6);

  // FuzzyRuleConsequent *thenHasilLordosis= new FuzzyRuleConsequent();
  thenHasilLordosis->addOutput(lordosis);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6, thenHasilLordosis);
  fuzzy->addFuzzyRule(fuzzyRule8);

  // Building FuzzyRule 9
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

  FuzzyRuleConsequent *thenHasilSkoliosis = new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule9);

  // Building FuzzyRule 10
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule10 = new FuzzyRule(10, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule10);

  // Building FuzzyRule 11
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule11 = new FuzzyRule(11, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule11);

  // Building FuzzyRule 12
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis = new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule12 = new FuzzyRule(12, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule12);

  // Building FuzzyRule 13
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule13 = new FuzzyRule(13, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule13);

  // Building FuzzyRule 14
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule14 = new FuzzyRule(14, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule14);

  // Building FuzzyRule 15
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule15 = new FuzzyRule(15, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule15);

  // Building FuzzyRule 16
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule16 = new FuzzyRule(16, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule16);

  // Building FuzzyRule 17
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule17 = new FuzzyRule(17, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule17);

  // Building FuzzyRule 18
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule18 = new FuzzyRule(18, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule18);

  // Building FuzzyRule 19
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule19 = new FuzzyRule(19, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule19);

  // Building FuzzyRule 20
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule20 = new FuzzyRule(20, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule20);

  // Building FuzzyRule 21
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule21 = new FuzzyRule(21, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule21);

  // Building FuzzyRule 22
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule22 = new FuzzyRule(22, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule22);

  // Building FuzzyRule 23
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule23 = new FuzzyRule(23, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule23);

  // Building FuzzyRule 24
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule24 = new FuzzyRule(24, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule24);

  // Building FuzzyRule 25
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule25 = new FuzzyRule(25, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule25);

  // Building FuzzyRule 26
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule26 = new FuzzyRule(26, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule26);

  // Building FuzzyRule 27
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule27 = new FuzzyRule(27, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule27);

  // Building FuzzyRule 28
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule28 = new FuzzyRule(28, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule28);

  // Building FuzzyRule 29
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule29 = new FuzzyRule(29, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule29);

  // Building FuzzyRule 30
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule30 = new FuzzyRule(30, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule30);

  // Building FuzzyRule 31
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule31 = new FuzzyRule(31, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule31);

  // Building FuzzyRule 32
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule32 = new FuzzyRule(32, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule32);

  // Building FuzzyRule 33
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule33 = new FuzzyRule(33, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule33);

  // Building FuzzyRule 34
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule34 = new FuzzyRule(34, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule34);

  // Building FuzzyRule 35
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule35 = new FuzzyRule(35, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule35);

  // Building FuzzyRule 36
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule36 = new FuzzyRule(36, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule36);

  // Building FuzzyRule 37
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule37 = new FuzzyRule(37, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule37);

  // Building FuzzyRule 38
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule38 = new FuzzyRule(38, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule38);

  // Building FuzzyRule 39
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule39 = new FuzzyRule(39, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule39);

  // Building FuzzyRule 40
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule40 = new FuzzyRule(40, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule40);

  // Building FuzzyRule 41
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule41 = new FuzzyRule(41, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule41);

  // Building FuzzyRule 42
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule42 = new FuzzyRule(42, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule42);

  // Building FuzzyRule 43
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule43 = new FuzzyRule(43, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule43);

  // Building FuzzyRule 44
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule44 = new FuzzyRule(44, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule44);

  // Building FuzzyRule 45
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule45 = new FuzzyRule(45, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule45);

  // Building FuzzyRule 46
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule46 = new FuzzyRule(46, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule46);

  // Building FuzzyRule 47
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule47 = new FuzzyRule(47, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule47);

  // Building FuzzyRule 48
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule48 = new FuzzyRule(48, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule48);

  // Building FuzzyRule 49
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule49 = new FuzzyRule(49, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule49);

  // Building FuzzyRule 50
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule50 = new FuzzyRule(50, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule50);

  // Building FuzzyRule 51
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule51 = new FuzzyRule(51, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule51);

  // Building FuzzyRule 52
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule52 = new FuzzyRule(52, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule52);

  // Building FuzzyRule 53
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule53 = new FuzzyRule(53, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule53);

  // Building FuzzyRule 54
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule54 = new FuzzyRule(54, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule54);

  // Building FuzzyRule 55
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule55 = new FuzzyRule(55, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule55);

  // Building FuzzyRule 56
  // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule56 = new FuzzyRule(56, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule56);

  // Building FuzzyRule 57
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule57 = new FuzzyRule(57, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule57);

  // Building FuzzyRule 58
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule58 = new FuzzyRule(58, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule58);

  // Building FuzzyRule 59
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule59 = new FuzzyRule(59, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule59);

  // Building FuzzyRule 60
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule60 = new FuzzyRule(60, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule60);

  // Building FuzzyRule 61
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule61 = new FuzzyRule(61, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule61);

  // Building FuzzyRule 62
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule62 = new FuzzyRule(62, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule62);

  // Building FuzzyRule 63
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule63 = new FuzzyRule(63, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule63);

  // Building FuzzyRule 64
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule64 = new FuzzyRule(64, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule64);

  // Building FuzzyRule 65
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule65 = new FuzzyRule(65, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule65);

  // Building FuzzyRule 66
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, normal5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule66 = new FuzzyRule(66, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule66);

  // Building FuzzyRule 67
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule67 = new FuzzyRule(67, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule67);

  // Building FuzzyRule 68
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule68 = new FuzzyRule(68, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule68);

  // Building FuzzyRule 69
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule69 = new FuzzyRule(69, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule69);

  // Building FuzzyRule 70
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule70 = new FuzzyRule(70, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule70);

  // Building FuzzyRule 71
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule71 = new FuzzyRule(71, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule71);

  // Building FuzzyRule 72
  // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
  //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
  ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, normal6);

  // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
  thenHasilSkoliosis->addOutput(skoliosis);
  FuzzyRule *fuzzyRule72 = new FuzzyRule(72, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
  fuzzy->addFuzzyRule(fuzzyRule72);
}

// class & struct begin --------------------------------
class Terminal
{
private:
  static const int BOX_WIDTH = 140;
  static const int BOX_HEIGHT = 70;
  static const int MARGIN = 10;

public:
  void drawBoxes()
  {
    // Clear previous display
    tft.fillRect(TERM_X, TERM_Y + TERM_HEADER_H, TERM_W, TERM_H - TERM_HEADER_H, TFT_WHITE);

    // Draw main title
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("DATA MONITORING", TERM_X + 60, TERM_Y + 2);

    // Calculate positions for boxes
    int startX = TERM_X + MARGIN;
    int startY = TERM_Y + TERM_HEADER_H + MARGIN;

    // Draw four boxes
    drawDataBox("CERVICAL", startX, startY, 1);
    drawDataBox("THORACAL", startX + BOX_WIDTH + MARGIN, startY, 2);
    drawDataBox("LUMBAL", startX, startY + BOX_HEIGHT + MARGIN, 3);
    drawDataBox("SACRUM", startX + BOX_WIDTH + MARGIN, startY + BOX_HEIGHT + MARGIN, 4);
  }

  void updateData()
  {
    // Calculate positions for boxes
    int startX = TERM_X + MARGIN;
    int startY = TERM_Y + TERM_HEADER_H + MARGIN;

    // Update data in each box
    updateBoxData(startX, startY, 1);
    updateBoxData(startX + BOX_WIDTH + MARGIN, startY, 2);
    updateBoxData(startX, startY + BOX_HEIGHT + MARGIN, 3);
    updateBoxData(startX + BOX_WIDTH + MARGIN, startY + BOX_HEIGHT + MARGIN, 4);
  }

private:
  void drawDataBox(const char *title, int x, int y, int boxNum)
  {
    // // Draw box outline
    // tft.drawRect(x, y, BOX_WIDTH, BOX_HEIGHT, TFT_BLACK);

    // // Draw title
    // tft.setTextColor(TFT_NAVY);
    // tft.setTextSize(1);
    // tft.drawString(title, x + 5, y + 5);

    // // Initial data display
    // updateBoxData(x, y, boxNum);

    // Draw box outline
    tft.drawRect(x, y, BOX_WIDTH, BOX_HEIGHT, TFT_BLACK);

    // Draw centered title
    tft.setTextColor(TFT_NAVY);
    tft.setTextSize(1);

    // Calculate text width and center position
    int titleWidth = strlen(title) * 6; // Approximate width for size 1 text
    int titleX = x + (BOX_WIDTH - titleWidth) / 2;

    tft.drawString(title, titleX, y + 5);

    // Draw border bottom (separator line) below title
    tft.drawLine(x, y + 18, x + BOX_WIDTH, y + 18, TFT_BLACK);

    // Initial data display
    updateBoxData(x, y, boxNum);
  }

  void updateBoxData(int x, int y, int boxNum)
  {
    char buffer[20];
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);

    // Clear previous data area
    tft.fillRect(x + 5, y + 24, BOX_WIDTH - 10, BOX_HEIGHT - 29, TFT_WHITE);

    // Format and display X angle
    snprintf(buffer, sizeof(buffer), "X: %.2f", fabs(angle_x[boxNum]));
    tft.drawString(buffer, x + 5, y + 28);

    // Format and display Y angle
    snprintf(buffer, sizeof(buffer), "Y: %.2f", fabs(angle_y[boxNum]));
    tft.drawString(buffer, x + 5, y + 41);

    // Format and display Z angle
    snprintf(buffer, sizeof(buffer), "Z: %.2f", fabs(angle_z[boxNum]));
    tft.drawString(buffer, x + 5, y + 54);
  }

  void clear()
  {
    tft.fillRect(TERM_X, TERM_Y + TERM_HEADER_H, TERM_W, TERM_H - TERM_HEADER_H, TFT_WHITE);
  }
} terminal;

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
    (SCREEN_HEIGHT / 2) + 55,
    BTN_WIDTH,
    BTN_HEIGHT,
    "START"};

Button nextButton = {
    (SCREEN_WIDTH - BTN_WIDTH) / 2,
    BTN_Y + 8,
    BTN_WIDTH + 20,
    BTN_HEIGHT,
    "CLASSIFY"};

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

void drawMainScreen()
{
  tft.fillScreen(TFT_WHITE);

  // Draw "Selamat Datang"
  tft.setTextColor(TFT_NAVY);
  tft.setTextSize(2); // Medium size
  const char *welcomeText = "SELAMAT DATANG";
  int welcomeWidth = strlen(welcomeText) * 12; // Approximate width of text
  int welcomeX = (SCREEN_WIDTH - welcomeWidth) / 2;
  tft.drawString(welcomeText, welcomeX, SCREEN_HEIGHT / 5);

  // Draw "SPINE" on first line
  tft.setTextSize(3); // Larger size
  const char *titleText1 = "SPINE";
  int titleWidth1 = strlen(titleText1) * 18; // Approximate width of larger text
  int titleX1 = (SCREEN_WIDTH - titleWidth1) / 2;
  tft.drawString(titleText1, titleX1, SCREEN_HEIGHT / 2 - 30);

  // Draw "ASSESSMENT" on second line
  const char *titleText2 = "ASSESSMENT";
  int titleWidth2 = strlen(titleText2) * 18; // Approximate width of larger text
  int titleX2 = (SCREEN_WIDTH - titleWidth2) / 2;
  tft.drawString(titleText2, titleX2, SCREEN_HEIGHT / 2 + 10); // Positioned below SPINE

  // Draw start button
  drawButton(startButton, TFT_BLUE);
}
// Draw terminal screen
void drawTerminalScreen()
{
  tft.fillScreen(TFT_WHITE);

  // Draw terminal border
  tft.drawRect(TERM_X, TERM_Y, TERM_W, TERM_H, TFT_BLACK);

  // Draw terminal header
  tft.fillRect(TERM_X, TERM_Y, TERM_W, TERM_HEADER_H, TFT_BLACK);

  // Initialize terminal display
  terminal.drawBoxes();

  // Draw next button
  drawButton(nextButton, TFT_BLUE);

  lastAnalogRead = millis();
}

// draw result screen
void drawResultScreen()
{
  tft.fillScreen(TFT_WHITE);

  // Header with black background
  // tft.fillRect(0, 0, SCREEN_WIDTH, 40, TFT_BLACK);
  // tft.setTextColor(TFT_WHITE);
  // tft.setTextSize(2);
  // tft.drawString("HASIL", (SCREEN_WIDTH - 50) / 2, 10);
  tft.fillRect(TERM_X, TERM_Y, TERM_W, TERM_HEADER_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("HASIL KLASIFIKASI", TERM_X - 75 + (TERM_W - 50) / 2, TERM_Y + 2);

  // Create result box
  int boxWidth = 280;
  int boxHeight = 120;
  int boxX = (SCREEN_WIDTH - boxWidth) / 2;
  int boxY = 60; // Position below header

  // Draw box with white background and black border
  tft.fillRect(boxX, boxY, boxWidth, boxHeight, TFT_WHITE);
  tft.drawRect(boxX, boxY, boxWidth, boxHeight, TFT_BLACK);

  // Show result with black text
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLACK);
  String result = String(hasilAnalisis[hasilCompareFuzzy]);
  int textX = boxX + (boxWidth - result.length() * 18) / 2; // Adjusted for text size
  int textY = boxY + (boxHeight - 24) / 2;                  // Centered vertically
  tft.drawString(result, textX, textY);

  // Draw back button (unchanged)
  drawButton(backButton, TFT_BLUE);
}

void tcaSelect(uint8_t i)
{
  if (i > 7)
    return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// fungsi komponen
//-----------------------------------------------------------------------------------------------
// Process
int compareValues(float nilai_normalis, float nilai_lordosis, float nilai_kifosis, float nilai_skoliosis)
{
  // Print input values for debugging
  Serial.println("Input nilai:");
  Serial.print("Normalis: ");
  Serial.println(nilai_normalis);
  Serial.print("Lordosis: ");
  Serial.println(nilai_lordosis);
  Serial.print("Kifosis: ");
  Serial.println(nilai_kifosis);
  Serial.print("Skoliosis: ");
  Serial.println(nilai_skoliosis);

  int maxValue = nilai_normalis;
  int maxIndex = 0;

  if (nilai_lordosis > maxValue)
  {
    maxValue = nilai_lordosis;
    maxIndex = 1;
    Serial.println("Lordosis lebih besar");
  }

  if (nilai_kifosis > maxValue)
  {
    maxValue = nilai_kifosis;
    maxIndex = 2;
    Serial.println("Kifosis lebih besar");
  }

  if (nilai_skoliosis > maxValue)
  {
    maxValue = nilai_skoliosis;
    maxIndex = 3;
    Serial.println("Skoliosis lebih besar");
  }

  // Serial.print("Index terbesar: ");
  // Serial.println(maxIndex);
  // Serial.print("Nilai terbesar: ");
  // Serial.println(maxValue);

  return maxIndex;
}

void processData()
{
  // Print input values for debugging
  Serial.println("Fuzzify nilai:");
  Serial.print("cervical_x: ");
  Serial.println(cervival_x);
  Serial.print("thoracal_x: ");
  Serial.println(thoracal_x);
  Serial.print("thoracal_z: ");
  Serial.println(thoracal_z);
  Serial.print("lumbal_x: ");
  Serial.println(lumbal_x);
  Serial.print("lumbal_z: ");
  Serial.println(lumbal_z);
  Serial.print("sacrum_x: ");
  Serial.println(sacrum_x);

  fuzzy->setInput(1, cervival_x);
  fuzzy->setInput(2, thoracal_x);
  fuzzy->setInput(3, thoracal_z);
  fuzzy->setInput(4, lumbal_x);
  fuzzy->setInput(5, lumbal_z);
  fuzzy->setInput(6, sacrum_x);
  fuzzy->fuzzify();

  float output1 = fuzzy->defuzzify(1);

  nilai_normalis = normalis->getPertinence();
  nilai_lordosis = lordosis->getPertinence();
  nilai_kifosis = kifosis->getPertinence();
  nilai_skoliosis = skoliosis->getPertinence();

  hasilCompareFuzzy = compareValues(nilai_normalis, nilai_lordosis, nilai_kifosis, nilai_skoliosis);
  Serial.print(hasilCompareFuzzy);
  Serial.println(hasilAnalisis[hasilCompareFuzzy]);
  Serial.println("Hasil: " + String(hasilAnalisis[hasilCompareFuzzy]));
}

// Function to check button press
bool isInButton(int x, int y, Button btn)
{
  return (x >= btn.x && x <= (btn.x + btn.width) &&
          y >= btn.y && y <= (btn.y + btn.height));
}

void readMpu()
{
  for (int ch = 1; ch < n_ch; ch++)
  {
    tcaSelect(ch);
    mpu[ch].update();

    angle_x[ch] = mpu[ch].getAngleX();
    angle_y[ch] = mpu[ch].getAngleY();
    angle_z[ch] = mpu[ch].getAngleZ();
    delay(10);
  }

  cervival_x = angle_x[1];
  thoracal_x = angle_x[2];
  thoracal_z = angle_z[2];
  lumbal_x = angle_x[3];
  lumbal_z = angle_z[3];
  sacrum_x = angle_x[4];
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
      drawMainScreen();
    }
  }
}

// fungsi monitoring
//-----------------------------------------------------------------------------------------------
void monitorSerial()
{

  for (int ch = 1; ch < n_ch; ch++)
  {
    // Serial.print("angX=");
    Serial.print(angle_x[ch]);
    Serial.print(",");
    // Serial.print("angY=");
    Serial.print(angle_y[ch]);
    Serial.print(",");
    // Serial.print("angZ=");
    Serial.print(angle_z[ch]);
    Serial.print(",");
  }
  Serial.println();
}

// fungsi utama
//-----------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  SPI.begin();

  // FuzzyInput ccx
  FuzzyInput *ccx = new FuzzyInput(1);
  ccx->addFuzzySet(normal1);
  ccx->addFuzzySet(kelainan1);
  fuzzy->addFuzzyInput(ccx);

  // FuzzyInput tcx
  FuzzyInput *tcx = new FuzzyInput(2);
  tcx->addFuzzySet(normal2);
  tcx->addFuzzySet(kelainan2);
  fuzzy->addFuzzyInput(tcx);

  // FuzzyInput tcz
  FuzzyInput *tcz = new FuzzyInput(3);
  tcz->addFuzzySet(normal3);
  tcz->addFuzzySet(kelainankanan3);
  tcz->addFuzzySet(kelainankiri3);
  fuzzy->addFuzzyInput(tcz);

  // FuzzyInput lcx
  FuzzyInput *lcx = new FuzzyInput(4);
  lcx->addFuzzySet(normal4);
  lcx->addFuzzySet(kelainan4);
  fuzzy->addFuzzyInput(lcx);

  // FuzzyInput lcz
  FuzzyInput *lcz = new FuzzyInput(5);
  lcz->addFuzzySet(normal5);
  lcz->addFuzzySet(kelainankanan5);
  lcz->addFuzzySet(kelainankiri5);
  fuzzy->addFuzzyInput(lcz);

  // FuzzyInput scx
  FuzzyInput *scx = new FuzzyInput(6);
  scx->addFuzzySet(normal6);
  scx->addFuzzySet(kelainan6);
  fuzzy->addFuzzyInput(scx);

  // FuzzyOutput
  FuzzyOutput *hasil = new FuzzyOutput(1);

  hasil->addFuzzySet(normalis);
  hasil->addFuzzySet(lordosis);
  hasil->addFuzzySet(kifosis);
  hasil->addFuzzySet(skoliosis);
  fuzzy->addFuzzyOutput(hasil);

  fuzzyRule();

  // Initialize backlight tft
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

  // Display calibration message
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Menyiapkan Alat", (SCREEN_WIDTH - 160) / 2, SCREEN_HEIGHT / 2 - 40);

  // Add loading animation dots
  tft.setTextSize(2);
  const char *dots[] = {".", "..", "..."};
  int dotIndex = 0;

  // Initialize and calibrate each MPU6050
  for (int ch = 1; ch < n_ch; ch++)
  {
    // Show which sensor is being calibrated
    tft.fillRect(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, 30, TFT_BLACK);
    tft.drawString("Sensor " + String(ch) + dots[dotIndex], (SCREEN_WIDTH - 100) / 2, SCREEN_HEIGHT / 2);
    dotIndex = (dotIndex + 1) % 3;

    tcaSelect(ch);
    byte status = mpu[ch].begin();

    while (status != 0)
    {
      Serial.println("MPU6050 at channel " + String(ch) + " not found");
      // Show error on screen
      tft.fillRect(0, SCREEN_HEIGHT / 2 + 30, SCREEN_WIDTH, 30, TFT_BLACK);
      tft.setTextColor(TFT_RED);
      tft.drawString("Error Sensor " + String(ch), (SCREEN_WIDTH - 140) / 2, SCREEN_HEIGHT / 2 + 30);
      delay(1000);
    }

    Serial.println("set ofset for MPU6050 at channel " + String(ch));
    delay(500);

    // Show calibration message
    tft.fillRect(0, SCREEN_HEIGHT / 2 + 30, SCREEN_WIDTH, 30, TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("Kalibrasi...", (SCREEN_WIDTH - 100) / 2, SCREEN_HEIGHT / 2 + 30);

    mpu[ch].calcOffsets(true, true); // gyro and accelero

    // Show completion message for this sensor
    tft.fillRect(0, SCREEN_HEIGHT / 2 + 30, SCREEN_WIDTH, 30, TFT_BLACK);
    tft.drawString("OK!", (SCREEN_WIDTH - 30) / 2, SCREEN_HEIGHT / 2 + 30);
    delay(500);
  }

  // Clear screen and show completion message
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BROWN);
  tft.drawString("Cold Start", (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT / 2 - 20);
  Serial.println("Cold Start");

  // Wait for cold start
  timerColdstart = millis();
  while (millis() - timerColdstart < intervalTimerColdstart)
  {
    readMpu();
  }

  // Clear screen and show completion message
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("Siap Digunakan!", (SCREEN_WIDTH - 150) / 2, SCREEN_HEIGHT / 2 - 20);
  delay(2000);

  // Return to main screen
  // tft.fillScreen(TFT_BLACK);
  // drawButton(startButton, TFT_BLUE);
  drawMainScreen(); // Draw the main welcome screen

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
    readMpu();
    if ((millis() - timerSerialMOnitor) > 1000)
    { // print data every second
      timerSerialMOnitor = millis();

      // terminalWrite();
      terminal.updateData(); // Update the display
      monitorSerial();
    }
  }
}