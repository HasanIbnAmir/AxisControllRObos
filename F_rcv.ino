#include <ESP8266WiFi.h>
#include <espnow.h>

// Define motor control pins
#define IN1 D2  // Motor A
#define IN2 D4
#define IN3 D5  // Motor B
#define IN4 D18


// Struct to match the sender
struct Data {
  float accelX;
  float accelY;
} receivedData;

// Threshold in m/s² to detect significant left/right motion
const float THRESHOLD = 4.5;

// Callback when data is received
void onDataReceived(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  float ax = receivedData.accelX;
  float ay = receivedData.accelY;

  Serial.print("Received accelX: ");
  Serial.print(ax, 3);
  Serial.print(" accelY: ");
  Serial.println(receivedData.accelY, 3);

  // Logic for left or right tilt
  
  if (ay < -THRESHOLD) { //left
   moveForward();
  } else if (ay > THRESHOLD) {
    // Tilted Right
    moveBackward();
  } 
  
  // else if(ax > THRESHOLD){
  //   moveForward();
  // }

  // else if(ax < -THRESHOLD){
  //   moveBackward();
  // }
  else {
stopMotors();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceived);
}

void loop() {
  //  handled in onDataReceived
}
