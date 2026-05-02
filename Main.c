#include <Wire.h>
#include <U8x8lib.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

// Pins
#define LED_PIN D13
#define BUTTON_PIN D2
#define POT_PIN A0

// MPU6050 address
#define MPU_ADDR 0x68

// --- Minimal MPU functions ---
void mpuWrite(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void mpuReadAccel(int16_t &ax, int16_t &ay, int16_t &az) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);

  ax = (Wire.read() << 8) | Wire.read();
  ay = (Wire.read() << 8) | Wire.read();
  az = (Wire.read() << 8) | Wire.read();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  Wire.begin();

  // Wake up MPU6050
  mpuWrite(0x6B, 0);

  // OLED init (tiny mode)
  display.begin();
  display.setFont(u8x8_font_chroma48medium8_r);
}

void loop() {
  // --- Potentiometer ---
  int pot = analogRead(POT_PIN);
  int pwm = pot >> 2; // faster than map()
  analogWrite(LED_PIN, pwm);

  // --- Button ---
  int btn = digitalRead(BUTTON_PIN);

  // --- MPU6050 ---
  int16_t ax, ay, az;
  mpuReadAccel(ax, ay, az);

  // --- OLED (text only, very light) ---
  display.clear();

  display.setCursor(0,0);
  display.print("P:");
  display.print(pot);

  display.setCursor(0,1);
  display.print("B:");
  display.print(btn);

  display.setCursor(0,2);
  display.print("X:");
  display.print(ax);

  display.setCursor(0,3);
  display.print("Y:");
  display.print(ay);

  display.setCursor(0,4);
  display.print("Z:");
  display.print(az);

  delay(150);
}
