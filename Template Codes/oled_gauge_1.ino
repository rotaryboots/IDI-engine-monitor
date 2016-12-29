//To use the OLED we need to include the following libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//We then need to define the pins we are using on the Arduino
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13

//Initialize the display
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//Initialise the sensor input and pressure variable in PSI for the display
int val = 0;
int pressure = 0;

//Set up the following
void setup()   
{                
  Serial.begin(9600); //To comunicate with the PC
  display.begin(SSD1306_SWITCHCAPVCC); //To supply the 3.3V power supply
  pinMode(0, INPUT); //Set analog pin 0 as an input from the sensor
  display.clearDisplay(); //Clear the display and ram
  delay(100);
  display.display(); //Show the cleared display to prevent for a smooth transition
  delay(100);
}

void loop() 
{
  val = (analogRead(0)/9.5);
  pressure = (analogRead(0)/17);
  drawOILPRESS(); //This runs the command specified below inside the loop
}

void drawOILPRESS(void)
{
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,12);
  display.println("OIL"); 
  display.setCursor(82,12);
  display.println(pressure);
  
  display.setTextSize(1);
  display.setCursor(10,32);
  display.println("PRESSURE");
  display.setCursor(85,32);
  display.println("PSI");

  display.drawRect(10, 50, 108, 10, WHITE); //Border of the bar chart
  display.fillRect(11, 51, val, 8, WHITE); //Draws the bar depending on the sensor value
  
  display.display();
  display.clearDisplay();
}


