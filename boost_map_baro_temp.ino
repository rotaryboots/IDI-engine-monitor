#include <Adafruit_BMP085.h> // the barometric sensor library
#include <Wire.h> // the I2C library
#include "U8glib.h" // the display library

int mapsen = 0; // Set MAP sensor input on Analog port 0
float boost = 0; // Set boost value to 0
float mapval = 0; // Set raw map value to 0
float atm = 0; //initial air pressure compensation
float raw = 0;
float kpa = 0;

Adafruit_BMP085 bmp; //alias for the baraomteric sensor

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(4, 5, 6, 7);  // SW SPI Com: SCK = 4, MOSI = 5, CS = 6, A0 = 7 (new white HalTec OLED)
//U8GLIB_SSD1306_128X64 u8g(10, 9);   // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(13, 11, 10, 9);  // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_ADAFRUIT_128X64 u8g(10, 9);    // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont); // sets the font used
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 12, "BOOST"); // displays the Word BOOST at the top left of the screen.
  u8g.drawStr( 105, 55, "BAR");  // displays the word BAR at the bottom right of the screen, level with the boost value.
  //u8g.setPrintPos(19, 40);
  //u8g.print(atm, 2);   
  u8g.setFont(u8g_font_fub30n); // sets the font to a 30 pixel size for the boost value.
  //this will print the boost value in Bar. I also allow for the - sign so that the display doesn't jump right when present. 
  if (boost >= 0) {
  u8g.setPrintPos(19, 55); // starts the display 19 pixels from the left and bottom of the line at 55 pixels. The font size is 30 pixels high.
  u8g.print(boost, 2); // displays the Bar value to 2 decimal places.
  }
  else if (boost < 0) {  // different start position to accomodate the - symbol.
  u8g.setPrintPos(5, 55);
  u8g.print(boost, 2);
  } 
 
}

void setup(void) {
   Serial.begin(9600);  // sets up the serial output.
  if (!bmp.begin()) {   // starts the barometric library to use for the atmospheric pressure compensation.
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");  // prints the start message to the serial port, seems to need this.
  while (1) {}
  } 
  // flip screen, if required
   u8g.setRot180();  // I have enabled this to flip the screen, as I want to fit it upside down.
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loop()
{
atm = (float)(bmp.readPressure());  // gets the value from the barometric sensor as the atm variable in mBar
mapval= analogRead(mapsen); // Reads the MAP sensor raw value on analog port 0
kpa = (mapval*(.00488)/(.022)+20); // Converts raw MAP value to KPa. I need to validate the equation here as I don't know how it was derived.
boost = (((mapval*(4.88)/(.022)+20000))-(atm))/100000; // Converts raw MAP value to Bar with the atmospheric compensation.
raw = (mapval*(4.88)/(.022)+20000); // the MAP value in mBar with the 20 KPa min value from the sensor added.
// picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(50);
  // here I am outputting the values from the sensors to the serial console for checking.
  Serial.println();
  Serial.print(" BOOST ");
  Serial.print(boost);
  Serial.println();
  Serial.print(" RAW ");
  Serial.print(raw);
  Serial.println();
  Serial.print(" ATM ");
  Serial.print(atm);
  Serial.println();
  Serial.print(" MAPVAL ");
  Serial.print(mapval);
  Serial.println();
  Serial.print(" KPA ");
  Serial.print(kpa);
}

