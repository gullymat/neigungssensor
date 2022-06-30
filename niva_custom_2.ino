
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);



int neig; //x
int kipp; //y
int limit_neig_gelb = 30;
int limit_neig_gelb_neg = -30;
int limit_neig_rot = 60;
int limit_neig_rot_neg = -60;
int limit_kipp_gelb = 30;
int limit_kipp_gelb_neg = -30;
int limit_kipp_rot = 60;
int limit_kipp_rot_neg = -60;

#include <EEPROM.h>

#include <Adafruit_MPU6050.h>
Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;

#include <FastLED.h>
#define NUM_LEDS  24
#define LED_PIN   2
CRGB leds[NUM_LEDS];


byte top[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte down[8] = {
  B10000,
  B01000,
  B01000,
  B00100,
  B00100,
  B00010,
  B00010,
  B00001
};

byte face[8] = {
  B10010,
  B01001,
  B00000,
  B00000,
  B00001,
  B00010,
  B01100,
  B00000
};

byte up[8] = {
  B00001,
  B00010,
  B00010,
  B00100,
  B00100,
  B01000,
  B01000,
  B10000
};

byte ladaf[8] = {
  B00000,
  B00001,
  B00010,
  B11100,
  B10000,
  B11111,
  B00110,
  B00110
};

byte ladar[8] = {
  B00000,
  B11110,
  B00001,
  B00001,
  B00001,
  B11111,
  B00110,
  B00110
};

byte ladaf1[8] = {
  B00000,
  B01111,
  B01000,
  B10100,
  B10000,
  B11111,
  B01100,
  B01100
};

byte ladaf2[8] = {
  B00000,
  B11110,
  B00010,
  B00101,
  B00001,
  B11111,
  B00110,
  B00110
};

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  Serial.begin(9600);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);

  mpu_accel = mpu.getAccelerometerSensor();
  mpu_accel->printSensorDetails();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(" I'm a Mr. Lada ");

  lcd.setCursor(0, 1);
  lcd.print("   Lover, hmmm  ");
  delay(2000);
  lcd.clear();

  lcd.createChar(0, top);
  lcd.createChar(1, down);
  lcd.createChar(2, face);
  lcd.createChar(3, up);
  lcd.createChar(4, ladaf);
  lcd.createChar(5, ladar);
  lcd.createChar(6, ladaf1);
  lcd.createChar(7, ladaf2);
  lcd.setCursor(0, 0);
  lcd.print("     Sensor     ");
  lcd.setCursor(0, 1);
  lcd.print("  KE-librierung ");

  sensors_event_t accel;
  mpu_accel->getEvent(&accel);
  int8_t x_offset_write = accel.acceleration.x * 9;
  int8_t y_offset_write = accel.acceleration.y * 9;
  delay(1000);
  EEPROM.write(0, x_offset_write);
  EEPROM.write(1, y_offset_write);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
    //  leds[i] = CRGB::Black;
  }

  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;
  }




  Serial.print("Offset: ");
  Serial.print(x_offset_write);
  Serial.print("\t");
  Serial.println(y_offset_write);



}
void loop() {

  sensors_event_t accel;
  mpu_accel->getEvent(&accel);



  int8_t x_offset_read = EEPROM.read(0);
  int8_t y_offset_read = EEPROM.read(1);



  //  neig = accel.acceleration.x * 9.2;
  //  kipp = accel.acceleration.y * 9.2;

  if (x_offset_read < 0)
    neig = (accel.acceleration.x * 9) - x_offset_read;
  if (x_offset_read > 0)
    neig = (accel.acceleration.x * 9) - x_offset_read;
  if (x_offset_read = 0)
    neig = (x_offset_read * 9.20);
  if (y_offset_read < 0)
    kipp = (accel.acceleration.y * 9) - y_offset_read;
  if (y_offset_read > 0)
    kipp = (accel.acceleration.y * 9) - y_offset_read;
  if (y_offset_read = 0)
    neig = (accel.acceleration.y * 9);



  lcd.setCursor(0, 0);
  lcd.print(neig);
  lcd.print((char) 223);
  lcd.print("      ");
  lcd.setCursor(8, 0);
  if (kipp < 0 && kipp > -10)
    lcd.print("     ");
  if (kipp >= 0 && kipp < 10)
    lcd.print("      ");
  if (kipp >= 10)
    lcd.print("     ");
  if (kipp <= -10)
    lcd.print("    ");
  lcd.print(kipp);
  lcd.print((char) 223);
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.write(byte(4));
  lcd.write(byte(5));
  lcd.print("          ");
  lcd.write(byte(6));
  lcd.write(byte(7));
  lcd.print(" ");


  FastLED.show();
  if (kipp < limit_kipp_gelb && kipp >= 0) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Green;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;
    FastLED.show();
  }
  if (kipp > limit_kipp_gelb && kipp <= limit_kipp_rot) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Orange;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Orange;
    leds[11] = CRGB::Black;
    FastLED.show();
  }
  if (kipp > limit_kipp_rot) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Red;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Red;
    FastLED.show();
  }
  if (kipp > limit_kipp_gelb_neg && kipp <= 0 ) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Green;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Green;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;
    FastLED.show();
  }
  if (kipp < limit_kipp_gelb_neg && kipp >= limit_kipp_rot_neg) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
    leds[2] = CRGB::Orange;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Black;
    leds[8] = CRGB::Orange;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;
    FastLED.show();
  }
  if (kipp < limit_kipp_rot_neg) {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Red;
    leds[2] = CRGB::Black;
    leds[3] = CRGB::Black;
    leds[4] = CRGB::Black;
    leds[5] = CRGB::Black;
    leds[6] = CRGB::Black;
    leds[7] = CRGB::Red;
    leds[8] = CRGB::Black;
    leds[9] = CRGB::Black;
    leds[10] = CRGB::Black;
    leds[11] = CRGB::Black;
    FastLED.show();
  }

  if (neig < limit_neig_gelb && neig >= 0) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Green;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Black;
    leds[20] = CRGB::Black;
    leds[21] = CRGB::Green;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Black;
    FastLED.show();
  }
  if (neig > limit_neig_gelb && neig <= limit_neig_rot) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Orange;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Black;
    leds[20] = CRGB::Orange;
    leds[21] = CRGB::Black;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Black;
    FastLED.show();
  }
  if (neig > limit_neig_rot) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Red;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Red;
    leds[20] = CRGB::Black;
    leds[21] = CRGB::Black;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Black;
    FastLED.show();
  }
  if (neig > limit_neig_gelb_neg && neig <= 0 ) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Green;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Black;
    leds[20] = CRGB::Black;
    leds[21] = CRGB::Green;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Black;
    FastLED.show();
  }
  if (neig < limit_neig_gelb_neg && neig >= limit_neig_rot_neg) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Orange;
    leds[17] = CRGB::Black;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Black;
    leds[20] = CRGB::Black;
    leds[21] = CRGB::Black;
    leds[22] = CRGB::Orange;
    leds[23] = CRGB::Black;
    FastLED.show();
  }
  if (neig < limit_neig_rot_neg) {
    leds[12] = CRGB::Black;
    leds[13] = CRGB::Black;
    leds[14] = CRGB::Black;
    leds[15] = CRGB::Black;
    leds[16] = CRGB::Black;
    leds[17] = CRGB::Red;
    leds[18] = CRGB::Black;
    leds[19] = CRGB::Black;
    leds[20] = CRGB::Black;
    leds[21] = CRGB::Black;
    leds[22] = CRGB::Black;
    leds[23] = CRGB::Red;
    FastLED.show();
  }


  if (kipp > limit_kipp_gelb || kipp < limit_kipp_gelb_neg || neig > limit_neig_gelb || neig < limit_neig_gelb_neg) {
    lcd.setCursor(0, 1);
    lcd.print("ALARM      ALARM");
  }
  if (kipp > limit_kipp_rot || kipp < -limit_kipp_rot || neig > limit_neig_rot || neig < limit_neig_rot_neg) {

    lcd.setCursor(0, 1);
    lcd.print("TJA  ");
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.print("_(");
    lcd.write(byte(2));
    lcd.print(")_");
    lcd.write(byte(3));
    lcd.write(byte(0));
    lcd.print("  ");



  }

  /*

    Serial.print("Neigung:");
    Serial.print(neig);
      Serial.print(",");
       Serial.print("X:");
      Serial.print(accel.acceleration.x);
      Serial.print(",");
    Serial.print("Kippung:");
    Serial.print(kipp);
        Serial.print(",");
       Serial.print("Y:");
      Serial.print(accel.acceleration.x);
    Serial.println("");
    delay(200);

  */



}
