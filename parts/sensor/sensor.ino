#include <U8glib.h>.....
#include <BME280I2C.h>
#include <Wire.h>
#include <math.h>
#include <stdio.h>
#include <string.h> 


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

// Counts the amount of needed zeros
String zero_adder(int source, int accuracy_var){
  String zeros_var = "";
  int digits = 0;
  while (source >= 10){
    source /= 10;
    digits++;
  }

  if(digits < accuracy_var){
    for(int i = 1; i<(accuracy_var - digits); i++){ zeros_var = zeros_var + "0"; }
  }

  return zeros_var;
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

  // Decimal
  int press_int;
  int press_remainder;
  int temp_int;
  int temp_remainder;
  // SETUP
  int accuracy = 1; // Must be => 1
  int multiplier = pow(10, accuracy);
  String zeros = ""; // Zeros is decimal output

  // BME Failcheck
  int fail = 0;
  while(!bme.begin())
  {
    fail++;
    if(fail >= 3){
      fail = 0;
      u8g.firstPage();
      do {
        u8g_prepare();
        u8g.drawStr( 0, 0, "BME Failure"); 
        u8g.drawStr( 0, 16, "Retrying..."); 
      } while( u8g.nextPage() );
    }
    delay(1000);
  }
  fail = 0;
  // BME Setup
  bme.chipModel();

  // Unit type declaration
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_inHg);

  // Read data from BME280
  bme.read( pres, temp, hum, tempUnit, presUnit);

  // Pressure Conversion
  pres *= 25,4;
  pres *= multiplier;
  press_int = (long)pres/multiplier;
  press_remainder = (long)pres % multiplier;

  // Temp Conversion
  if(temp != 0){
      temp *= multiplier;
      temp_int = (long)temp/multiplier;
      temp_remainder = (long)temp % multiplier;       
    } else {
      temp_int = 0;
      temp_remainder = 0;
  }

  
  // Value Prepairation
  zeros = zero_adder(press_remainder, accuracy);
  String pressureST = "Press: " + String(press_int, DEC) + '.' + zeros + String(press_remainder, DEC);
  zeros = zero_adder(temp_remainder, accuracy);
  String tempST = "Temp: " + String(temp_int, DEC) + "." + zeros + String(temp_remainder, DEC) + " C�";
  pressureST.toCharArray(pressstr, sizeof(pressstr));
  tempST.toCharArray(temperature, sizeof(temperature));
  sprintf(humiditystr, "Hum:  %d %%", (int)hum);

  //sprintf(pressstr,    "Press: %d,%d ", (int)press_int, (int)press_remainder);
  //sprintf(humiditystr, "Humid: %d %%", (int)hum);

  // Display output
  u8g.firstPage();  
  do {
      u8g_prepare();
      u8g.drawStr( 0, 0, pressstr);
      u8g.drawStr( 0, 16, humiditystr);
      u8g.drawStr( 0, 32,temperature); 
      
    
    } while( u8g.nextPage() );
   
  delay(1000);
} 

// Reciever version 2 ___________________________________________________________
//int reciever_counter = 0;
int payload_size = 0;
int recieved_beggining = 0;
int data_counter = 0;

void bt_pkg_reciever(uint8_t* location, uint8_t size){
  uint8_t *p = location;
  uint8_t recieved_data[]; //FIXME size is undefined
  for (i = 0, i< (int)size, i++){

    if (( p[i] == 0xa0) || (p[i] == 0x20) && (recieved_beggining = 0)) {recieved_beggining = 1; } // Finds responce beggining

    if (data_counter == 1) { payload_size = p[i]; } // Gets Payload size

    // Writes recieved data to array
    if(recieved_beggining = 1){ 
      recieved_data[data_counter] = p[i];
      data_counter++;

    // Sends loaded responce to decoder
    if ((data_counter >= payload_size) && (data_counter > 1)){
      data_counter = 0;
      payload_size = 0;
      recieved_beggining = 0;
      decoder(received_data);
      }
    }
  }
}

void decoder(bt_data){
//EMPTY, for now......
}
