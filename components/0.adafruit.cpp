#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

Adafruit_MPU6050 mpu;

float yaw = 0;
unsigned long lastTime = 0;
float roll_offset = 0;
float pitch_offset = 0;

void calibrateSensor()
{
    float roll_sum = 0;
    float pitch_sum = 0;
    int num_readings = 100;

    for (int i = 0; i < num_readings; i++)
    {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        float roll = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
        float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;

        roll_sum += roll;
        pitch_sum += pitch;

        delay(10);
    }

    roll_offset = roll_sum / num_readings;
    pitch_offset = pitch_sum / num_readings;
}

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("Adafruit MPU6050 test!");

    // Try to initialize!
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("Calibrating...");
    calibrateSensor();
    Serial.println("Calibration complete!");

    Serial.println("");
    delay(100);
}

void loop()
{
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    // Calculate and print calibrated tilt angles
    float roll = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI - roll_offset;
    float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI - pitch_offset;

    // Estimate yaw using gyro data
    yaw += g.gyro.z * 180.0 / PI * deltaTime;

    Serial.print("Calibrated Roll angle: ");
    Serial.print(roll);
    Serial.println(" degrees");
    Serial.print("Calibrated Pitch angle: ");
    Serial.print(pitch);
    Serial.println(" degrees");
    Serial.print("Yaw angle: ");
    Serial.print(yaw);
    Serial.println(" degrees");

    Serial.println("");
    delay(500);
}