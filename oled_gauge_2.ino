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

//Initialise the sensor input and pressure variables from sensor inputs
int val;
int pressure = 0;
int push;

//To switch between displays
int programnumber = 1;

//Temperature sensor
const int numTemps = 40;       //Here the number of temperatures is set to 7
int tempsense = A1;            //Temperature sensor on analog pin 1
int temps[numTemps];           //Sets up the readings from the analog input
int index = 0;                 //Refers to the index of the current reading
float total = 0;               //Floats the running total
float average = 0;             //Floats the average

//Set up the following
void setup()   
{                
  Serial.begin(9600); //To comunicate with the PC
  display.begin(SSD1306_SWITCHCAPVCC); //To supply the 3.3V power supply
  pinMode(0, INPUT); //Set analog pin 0 as an input from the sensor
  pinMode(2, INPUT_PULLUP);
  
  //Initialixe all readings to 0
  for(int thisTemp = 0; thisTemp < numTemps; thisTemp++)
  temps[thisTemp] = 0;
  
  display.clearDisplay(); //Clear the display and ram
  display.display(); //Show the cleared display to prevent for a smooth transition
  delay(1000);
}

void loop() {
  push = digitalRead(2);

  if (push == LOW){
    delay(100);
    {
      programnumber++;
    if(programnumber > 7){
      programnumber = 1;
    }}}
    
  switch (programnumber) { //if the button is not let go within the menu time stated above then it does not loop back and then selects the subroutine from this switch case:
      case 1:
        drawWT();
        break;
      case 2:
        drawOP();
        break;
      case 3:
        drawAT();
        break;
      case 4:
        drawBC();
        break;
      case 5:
        drawTRIP();
        break;
      case 6:
        drawMPG();
        break;
      case 7:
        drawOFF();
        break;
      }
}

void drawOP(void)
{
  val = ((analogRead(0)/9.5) - 3);
  pressure = (analogRead(0)/17);
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,12);
  display.println("OIL"); 
  display.setCursor(83,12);
  display.println(pressure);
  
  display.setTextSize(1);
  display.setCursor(10,32);
  display.println("PRESS");
  display.setCursor(86,32);
  display.println("PSI");

  display.drawLine(10, 60, 10, 62, WHITE);
  display.drawLine(64, 60, 64, 62, WHITE);
  display.drawLine(117, 60, 117, 62, WHITE);
  display.drawRect(10, 50, 108, 10, WHITE); //Border of the bar chart
  display.fillRect(12, 52, val, 6, WHITE); //Draws the bar depending on the sensor value
  
  if (pressure < 10)
  {  display.setCursor(52,32);
     display.println("LOW!");
  }
  
  display.display();
  display.clearDisplay();
}

void drawWT(void)
{
  val = ((analogRead(0)/9.5) - 3);
  pressure = (analogRead(0)/10.2);
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,12);
  display.println("WATER"); 
  display.setCursor(91,12);
  display.println(pressure);
  
  display.setTextSize(1);
  display.setCursor(10,32);
  display.println("TEMP");
  display.setCursor(102,32);
  display.println("C");
  display.drawCircle(98, 33, 2, WHITE);
  
  display.drawLine(10, 60, 10, 62, WHITE);
  display.drawLine(64, 60, 64, 62, WHITE);
  display.drawLine(117, 60, 117, 62, WHITE);
  display.drawRect(10, 50, 108, 10, WHITE); //Border of the bar chart
  display.fillRect(12, 52, val, 6, WHITE);//Draws the bar depending on the sensor value
  
  if (pressure > 80)
  {  display.setCursor(54,32);
     display.println("HOT!");
  }
  
  display.display();
  display.clearDisplay();
}

void drawAT(void)
{
  
  total = total - temps[index];
  
  //Function to conver the analog reading to degrees celcius
  //0-5v is converted to 0-1023 integer values
  //TMP36 outputs at a rate of 10mV per unit temperature
  //The gradient for function of temperature against voltage is 100
  //For the analog reading operation, 1V = 204.6
  //Dividing the gradient by 204.6 we get 0.489
  //At 0v the function gives -50 degrees
  //Hence the function becaomes y=0.489x-50
  
  //Read the current temperature to index
  temps[index] = (((analogRead(tempsense))*0.4888)-50);
  
  //Add the newly read temperature to the total
  total = total + temps[index];
  
  //Advance to the next position in the array
  index = index + 1;
  
  //If we are at the end of the array, move to the start
  if (index >= numTemps)
  index = 0;
  
  //Calculate the average of the current array
  average = total / numTemps;
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,22);
  display.println("AIR"); 
  display.setCursor(75,22);
  display.println(average, 1);
  
  
  display.setTextSize(1);
  display.setCursor(10,42);
  display.println("TEMP");
  display.setCursor(98,42);
  display.println("C");
  display.drawCircle(93, 43, 2, WHITE);
  
      if (average < 3)
  {  display.setCursor(53,42);
     display.println("ICE!");
  }
  
  display.display();
  display.clearDisplay();
}

void drawBC(void)
{
  val = ((analogRead(0)/9.5) - 3);
  pressure = ((analogRead(0)/10.2));
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,12);
  display.println("BATT"); 
  display.setCursor(82,12);
  display.println(pressure);
  
  display.setTextSize(1);
  display.setCursor(10,32);
  display.println("CHARGE");
  display.setCursor(90,32);
  display.println(char(37));

  display.drawLine(10, 60, 10, 62, WHITE);
  display.drawLine(64, 60, 64, 62, WHITE);
  display.drawLine(117, 60, 117, 62, WHITE);
  display.drawRect(10, 50, 108, 10, WHITE);
  display.fillRect(12, 52, val, 6, WHITE);
  
   //Draws the bar depending on the sensor value
  
  if (pressure < 50)
  {  display.setCursor(56,32);
     display.println("LOW!");
  }
  
  display.display();
  display.clearDisplay();
  
}

void drawTRIP(void)
{
  val = (analogRead(0)/9.5);
  pressure = (analogRead(0)/2.5);
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,22);
  display.println("TRIP"); 
  display.setCursor(80,22);
  display.println(pressure);
  
  display.setTextSize(1);
  display.setCursor(10,42);
  display.println("DISTANCE");
  display.setCursor(80,42);
  display.println("MILES");
  
  
  display.display();
  display.clearDisplay();
}

void drawMPG(void)
{
  val = (analogRead(0)/9.5);
  pressure = (analogRead(0)/22.7);
  
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10,22);
  display.println("MPG"); 
  display.setTextSize(3);
  display.setCursor(80,22);
  display.println(pressure);
  
  display.setTextSize(1);
 
  
    if (pressure < 15)
  {   display.setCursor(10,42);
     display.println("SPORT");
  }
    if (pressure > 35)
  {   display.setCursor(10,42);
     display.println("ECON");
  }
  
  if (15 <= pressure && pressure <= 35)
  {
  display.setCursor(10,42);
  display.println("NORMAL");
  }
  
  
  display.display();
  display.clearDisplay();
}

void drawOFF(void)
{  
  display.display();
  display.clearDisplay();
  delay(100);
}
