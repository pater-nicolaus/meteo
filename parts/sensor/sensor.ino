#include <U8glib.h>
#include <BME280I2C.h>
#include <Wire.h>

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_FAST);	// Dev 0, Fast I2C / TWI

#define SERIAL_BAUD 115200

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure x1, temperature x1, humidity x1, filter off,

void u8g_prepare() {
  u8g.setFont(u8g_font_9x18);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void setup() {
  Serial.begin(SERIAL_BAUD);  
#if defined(ARDUINO)
  pinMode(13, OUTPUT);           
  digitalWrite(13, HIGH);  
#endif
}

void loop() {
  float temp, hum, pres;

  // Variable Declaration
  char pressstr[100];
  char humiditystr[100];
  char temperature[100];
   

  while(!bme.begin())
  {
    delay(1000);
  }
  bme.chipModel();

  // Value type declaration
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_inHg);

  // Read from microcontroller
  bme.read( pres, temp, hum, tempUnit, presUnit);

  // Pressure Conversion
  pres *= 25,4;

  // Value Prepairation
  sprintf(pressstr,    "Press:   %d ", (int)pres);
  sprintf(humiditystr, "Humid:   %d %%", (int)hum);
  sprintf(temperature, "Temp:    %d C", (int)temp);
   
  // Picture loop  
  u8g.firstPage();  
  do {
      u8g_prepare();
      u8g.drawStr( 0, 0, pressstr);
      u8g.drawStr( 0, 16, humiditystr);
      u8g.drawStr( 0, 32,temperature); 

      //u8g.drawFrame(5,10+30,20,10);

   } while( u8g.nextPage() );
  
  delay(1000);
} 

