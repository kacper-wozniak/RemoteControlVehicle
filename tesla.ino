#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
/* ENGINE */
#define MODE D0
#define LDIR D1
#define LPWM D2
#define RDIR D3
#define RPWM D4
#define FRONT LOW
#define BACK HIGH
/* WHEEL SPEED */
#define FRONT_LEFT_WHEEL D5
#define FRONT_RIGHT_WHEEL D6
#define BACK_LEFT_WHEEL D7
#define BACK_RIGHT_WHEEL D8
/* BATTERY LEVEL */
#define R1 215.5
#define R2 119.5
#define POWER A0
/* NETWORK CONNECTION */
#define UDP_PORT 4210
WiFiUDP UDP;
char messageBuffer[64];

Ticker ticker;

int frontLeftWheelSpeed = 0;
int frontRightWheelSpeed = 0;
int backLeftWheelSpeed = 0;
int backRightWheelSpeed = 0;

void setup() {
  pinMode(A0, INPUT);
}

void sendStatus() {
  char buffer[4];
  sprintf(buffer, "%d", )
  send(message: 
}

void setupWheelSpeed() {
  pinMode(FRONT_LEFT_WHEEL, INPUT);
  pinMode(FRONT_RIGHT_WHEEL, INPUT);
  pinMode(BACK_LEFT_WHEEL, INPUT);
  pinMode(BACK_RIGHT_WHEEL, INPUT);
  attachInterrupt(FRONT_LEFT_WHEEL, frontLeftWheelCounter, RISING);
  attachInterrupt(FRONT_RIGHT_WHEEL, frontRightWheelCounter, RISING);
  attachInterrupt(BACK_LEFT_WHEEL, backLeftWheelCounter, RISING);
  attachInterrupt(BACK_RIGHT_WHEEL, backRightWheelCounter, RISING);
}

void frontLeftWheelCounter() {
  frontLeftWheelSpeed++;
}

void frontRightWheelCounter() {
  frontRightWheelSpeed++;
}

void backLeftWheelCounter() {
  backLeftWheelSpeed++;
}

void backRightWheelCounter() {
  backRightWheelSpeed++;
}

char* readWheelSpeed() {
  char string[32];
  sprintf(string, "%d %d %d %d", frontLeftWheelSpeed, frontRightWheelSpeed, backLeftWheelSpeed, backRightWheelSpeed);
  frontLeftWheelSpeed = 0;
  frontRightWheelSpeed = 0;
  backLeftWheelSpeed = 0;
  backRightWheelSpeed = 0;
  return string
}

void loop() {
//  updateState();
  Serial.println(change);
  delay(0);
}

void setupWiFiHotspot() {
  WiFi.mode(WIFI_AP);
  Serial.begin(9600);
  if (WiFi.softAP("Tesla Roadster", "intive")) {
    Serial.println(WiFi.softAPIP());
    UDP.begin(UDP_PORT);
  }
  else Serial.println("Hotspot failed.");
}

void setupEngine() {
  analogWriteFreq(20000);
  analogWriteRange(255);
  pinMode(LPWM, OUTPUT);
  pinMode(LDIR, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(RDIR, OUTPUT);
  pinMode(MODE, OUTPUT);
  digitalWrite(MODE, HIGH);
}

int batteryVoltage() {
  return analogRead(A0) * (R1 + R2) / R2 / 1023
}

void updateState() {
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    int len = UDP.read(messageBuffer, 64);
    if (len > 0) {
      messageBuffer[len] = 0;
      parseMessage(messageBuffer);
    }
  }
}

void parseMessage(char* message) {
  float leftEnginePower = 0;
  float rightEnginePower = 0;
  while (*message != '\0') {
    if (*message == 'L') {
      leftEnginePower = atof(++message);
    }
    if (*message == 'R') {
      rightEnginePower = atof(++message);
    }
    message++;
  }
  update(leftEnginePower, rightEnginePower);
}

void send(char* message, int len, IPAddress remote) {
  UDP.beginPacket(remote, UDP_PORT);
  UDP.write(message, len);
  UDP.endPacket();
}

void update(float leftEnginePower, float rightEnginePower) {
  digitalWrite(LDIR, (leftEnginePower > 0) ? FRONT : BACK);
  analogWrite(LPWM, abs(leftEnginePower) * 255);
  digitalWrite(RDIR, (rightEnginePower > 0) ? FRONT : BACK);
  analogWrite(RPWM, abs(rightEnginePower) * 255);
}

