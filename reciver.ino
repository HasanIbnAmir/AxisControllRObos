#include <ESP8266WiFi.h>
#include <espnow.h>

int output=D4;
// Struct to hold the data
struct Data {
  int intValue;
  // char charValue;
  // char stringValue[32];  // Match the sender's char array size
} receivedData;

// Callback when data is received
void onDataReceived(uint8_t *senderMac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));  // Copy received data into struct

  // Display the received data
  Serial.print("Received int: ");
  Serial.println(receivedData.intValue);

  // Serial.print("Received char: ");
  // Serial.println(receivedData.charValue);

  // Serial.print("Received string: ");
  // Serial.println(receivedData.stringValue);  // Properly print the received string
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  pinMode(output, OUTPUT);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceived);
}

void loop() {
  if (receivedData.intValue==LOW){
    digitalWrite(output,HIGH);
  }
  else {
  digitalWrite(output,LOW);
  }
  delay(1000);

}