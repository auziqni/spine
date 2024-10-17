#include <Wire.h>
#include <MPU6050_light.h>

#define TCA9548A_ADDR 0x70
const int n_sensor = 4;
const int n_ch = n_sensor + 1;
unsigned long timer = 0;
float angle_x[n_ch] = {0}, angle_y[n_ch] = {0}, angle_z[n_ch] = {0};

MPU6050 mpu[n_ch] = {MPU6050(Wire), MPU6050(Wire), MPU6050(Wire), MPU6050(Wire), MPU6050(Wire)}; // disesuaikan manual dengan jumlah sensor

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

  for (int ch = 1; ch < n_ch; ch++) // TODO change to NUM_SENSORS
  {
    tcaSelect(ch);
    byte status = mpu[ch].begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0)
    {
      Serial.println("MPU6050 at channel " + String(ch) + " not found");
      delay(1000);
    } // stop everything if could not connect to MPU6050
    Serial.println("set ofset for MPU6050 at channel " + String(ch));
    // mpu[i].setGyroOffsets(-8.53, 1.56, -0.80); // gyro only
    // mpu[i].setAccOffsets(0.04, 0.00, 0.14);    // accelero only
    delay(1000);
    mpu[ch].calcOffsets(true, true); // gyro and accelero

    // Serial.println(F("Calculating offsets, do not move MPU6050"));
    // delay(1000);
    // // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  }
  Serial.println("Done!\n");
}

void loop()
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

  if ((millis() - timer) > 1000)
  { // print data every second
    timer = millis();

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
}