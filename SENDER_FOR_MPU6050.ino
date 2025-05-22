#include <ESP8266WiFi.h>
#include <espnow.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;

// Replace with receiver's MAC address
uint8_t receiverMAC[] = {0x48, 0x55, 0x19, 0xC8, 0xAA, 0x0B};

// MPU6050 raw values
int16_t ax, ay, az, gx, gy, gz;
float ax_offset = 0, ay_offset = 0, az_offset = 0;

// Struct to send data
struct Data {
  float accelX;
  float accelY;
} dataToSend;

// Callback for send status
void onDataSent(uint8_t *macAddr, uint8_t sendStatus) {
  Serial.print("Send status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connected");
  delay(2000); // Wait for sensor to stabilize

  // Get initial offsets
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax_offset = (ax / 16384.0) * 9.80665;
  ay_offset = (ay / 16384.0) * 9.80665;
  az_offset = (az / 16384.0) * 9.80665;

  Serial.println("Calibration complete.");
  Serial.print("Offsets - X: "); Serial.print(ax_offset, 3);
  Serial.print(" Y: "); Serial.print(ay_offset, 3);
  Serial.print(" Z: "); Serial.println(az_offset, 3);

  // Initialize WiFi and ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_register_send_cb(onDataSent);
}

void loop() {
  // Read sensor data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float ax_ms2 = (ax / 16384.0) * 9.80665 - ax_offset;
  float ay_ms2 = (ay / 16384.0) * 9.80665 - ay_offset;

  // Fill struct
  dataToSend.accelX = ax_ms2;
  dataToSend.accelY = ay_ms2;

  // Send data
  esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));

  // Print for debugging
  Serial.print("X: "); Serial.print(ax_ms2, 3);
  Serial.print(" Y: "); Serial.println(ay_ms2, 3);

  delay(800); // Adjust rate as needed
}
