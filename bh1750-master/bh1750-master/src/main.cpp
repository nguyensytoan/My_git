#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print(F("Light: "));
  Serial.print(lux);
  Serial.println(F(" lx"));
  delay(1000);
}