#include <Wire.h>
#include <MPU6050_light.h>

#define TCA9548A_ADDR 0x70
int NUM_SENSORS = 3;
unsigned long timer = 0;

MPU6050 mpu[3] = {MPU6050(Wire), MPU6050(Wire), MPU6050(Wire)};

void tcaSelect(uint8_t i)
{
    if (i > 7)
        return;
    Wire.beginTransmission(TCA9548A_ADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    // for (int i = 0; i < 1; i++) // TODO change to NUM_SENSORS
    // {
    tcaSelect(1);
    byte status = mpu[1].begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0)
    {
    } // stop everything if could not connect to MPU6050
    Serial.println(F("set ofset"));
    // mpu[i].setGyroOffsets(-8.53, 1.56, -0.80); // gyro only
    // mpu[i].setAccOffsets(0.04, 0.00, 0.14);    // accelero only
    delay(2000);
    mpu[1].calcOffsets(true, true); // gyro and accelero
    delay(2000);

    // Serial.println(F("Calculating offsets, do not move MPU6050"));
    // delay(1000);
    // // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
    Serial.println("Done!\n");
    // }
}

void loop()
{

    tcaSelect(1);
    mpu[1].update();
    delay(10);

    if ((millis() - timer) > 100)
    { // print data every second
        Serial.print("MPU 0: angX=");
        Serial.print(mpu[1].getAngleX());
        Serial.print(" angY=");
        Serial.print(mpu[1].getAngleY());
        Serial.print(" angZ=");
        Serial.println(mpu[1].getAngleZ());
        timer = millis();
    }
}