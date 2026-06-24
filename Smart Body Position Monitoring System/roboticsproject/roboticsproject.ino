#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int buzzerPin = 8;

// ±10 degrees = normal movement
const int stableRange = 10;

// Read posture every 5 seconds
unsigned long readInterval = 5000;
unsigned long lastRead = 0;

float lastAngle = 0;
unsigned long stableTime = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.println("Posture System - 1 Minute Alarm");
}

void loop() {

  if (millis() - lastRead >= readInterval) {
    lastRead = millis();

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float angle = atan2(ax, az) * 57.2958;

    Serial.print("Angle (5s): ");
    Serial.println(angle);

    // Check if posture remains stable
    if (abs(angle - lastAngle) <= stableRange) {
      stableTime++;
    } else {
      stableTime = 0;
    }

    lastAngle = angle;

    // 1 minute = 12 readings (5 seconds × 12)
    if (stableTime >= 12) {

      // Activate buzzer for 2 seconds
      tone(buzzerPin, 2000);   // High-frequency alert tone
      delay(2000);             // Alert duration
      noTone(buzzerPin);       // Stop buzzer

      // Reset counter to avoid repeated alerts
      stableTime = 0;
    }
  }
}