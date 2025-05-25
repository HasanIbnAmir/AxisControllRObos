
/*
  MPU6050 Raw

  A code for obtaining raw data from the MPU6050 module with the option to
  modify the data output format.

  Find the full MPU6050 library documentation here:
  https://github.com/ElectronicCats/mpu6050/wiki
*/
#include "I2Cdev.h"
#include "MPU6050.h"

/* MPU6050 default I2C address is 0x68*/
MPU6050 mpu;
//MPU6050 mpu(0x69);         //Use for AD0 high
//MPU6050 mpu(0x68, &Wire1); //Use for AD0 low, but 2nd Wire (TWI/I2C) object.

/* OUTPUT FORMAT DEFINITION----------------------------------------------------------------------------------
- Use "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated list of the accel 
X/Y/Z and gyro X/Y/Z values in decimal. Easy to read, but not so easy to parse, and slower over UART.

- Use "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit binary, one right after the other. 
As fast as possible without compression or data loss, easy to parse, but impossible to read for a human. 
This output format is used as an output.
--------------------------------------------------------------------------------------------------------------*/ 
// #define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO

// #define D5 5 //Right led
// #define D6 6 //Left led
// #define D7 7 // forward led
// #define D8 8 // backward led

int16_t ax, ay, az;
int16_t gx, gy, gz;
bool blinkState;

float ax_offset = 0;
float ay_offset = 0;
float az_offset = 0;
void setup() {


  // Usual initialization
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connection successful");

  // Give sensor time to stabilize
  delay(2000);

  // Read initial accel values to use as offsets
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax_offset = (ax / 16384.0) * 9.80665;
  ay_offset = (ay / 16384.0) * 9.80665;
  az_offset = (az / 16384.0) * 9.80665;

  Serial.println("Calibration complete.");
  Serial.print("Offsets - X: "); Serial.print(ax_offset, 3);
  Serial.print(" Y: "); Serial.print(ay_offset, 3);
  Serial.print(" Z: "); Serial.println(az_offset, 3);

  
}


void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float ax_ms2 = (ax / 16384.0) * 9.80665 - ax_offset;
  float ay_ms2 = (ay / 16384.0) * 9.80665 - ay_offset;
  // float az_ms2 = (az / 16384.0) * 9.80665 - az_offset;

  Serial.print("Xis: "); Serial.print(ax_ms2, 3);
  Serial.print(" m/s², Yis: "); Serial.print(ay_ms2, 3);
  // Serial.print(" m/s², Zis: "); Serial.print(az_ms2, 3);
  Serial.println(" m/s²");
//for x axis movement
  // if (ax_ms2<0 && ax_ms2<-4.5 ){
  //   digitalWrite(D5,HIGH);
  // }
  // else if (ax_ms2>0 && ax_ms2>4.5) {
  // digitalWrite(D6,HIGH);
  // }
// for y axis movement

  // if (ay_ms2<0 && ay_ms2<-4.5 ){
  //   digitalWrite(D8,HIGH);
  // }
  // else if (ay_ms2>0 && ay_ms2>4.5) {
  // digitalWrite(D7,HIGH);
  // }
  
}
