#include <U8glib.h>
#include <BME280I2C.h>
#include <Wire.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);	// Dev 0, Fast I2C / TWI

#define SERIAL_BAUD 115200

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure x1, temperature x1, humidity x1, filter off,

void setup(void) {
  Serial.begin(SERIAL_BAUD);  
#if defined(ARDUINO)
  pinMode(13, OUTPUT);           
  digitalWrite(13, HIGH);  
#endif
}

void loop(void) {
   float temp(NAN), hum(NAN), pres(NAN);
   char pressstr[100];

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_inHg);

   bme.read(pres, temp, hum, tempUnit, presUnit);
   sprintf(pressstr, "pressure %f", pres);
   u8g.drawStr( 0, 0, pressstr); 
  
   delay(1000);
}
