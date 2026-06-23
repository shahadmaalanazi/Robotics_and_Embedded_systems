#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int buzzerPin = 8;

// ±10 درجات = حركة طبيعية
const int stableRange = 10;

// قراءة كل 5 ثواني
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

    // ثابت = فرق بسيط
    if (abs(angle - lastAngle) <= stableRange) {
      stableTime++;
    } else {
      stableTime = 0;
    }

    lastAngle = angle;

    // دقيقة = 12 قراءة (5 ثواني × 12)
    if (stableTime >= 12) {

      //  صوت عالي وثابت لمدة 2 ثواني
      tone(buzzerPin, 2000);   // 2000 Hz = نغمة عالية
      delay(2000);             // مدة طويلة (2 ثواني)
      noTone(buzzerPin);       // إيقاف الصوت

      // إعادة العداد حتى ما يكرر كل 5 ثواني
      stableTime = 0;  
    }
  }
}
