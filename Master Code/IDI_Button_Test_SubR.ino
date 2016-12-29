//Engine Monitor Alpha Testing code IDI
//Written by Christian Boots

int counter = 0; //value to store the amount of times the button is pressed for the cycle loop

//LCD Wiring Setup
#include <LCD5110_Basic.h>
LCD5110 myGLCD(8, 9, 10, 11, 12);

//Bitmaps used
extern uint8_t IH[];
extern uint8_t MENU1[];

// the setup function runs once when you press reset or power the board
void setup()
 {
	// initialize LCD 
	myGLCD.InitLCD(); 
	// initialize digital pin 13 as an output.
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, INPUT);
// Lets setup some default output Highs/Lows
	digitalWrite(5, HIGH); //Default High Pin 4 for GND active

	Serial.begin(9600); //Start some serial fun
}

// the loop function runs over and over again forever
void loop() //Mode Switching?
 {	
	int ModeButton1 = digitalRead(5);
	
	
	if (ModeButton1 == LOW) { //is the button pressed?
	    delay(500);
		counter ++;
		//Reset count if over 5
		if (counter == 5) {
		counter = 0;
		}
	}
	else
	//!mode Change!
	{
		Serial.print("number of button pushes:  ");
		Serial.println(counter);
		if (counter == 1)
		led1();
		if (counter == 2)
		led2();
		if (counter == 3)
		led3();
		if (counter == 4)
		led4();
		
	}
	
}
void led1()  //LED 1
{
	digitalWrite(4, LOW);
	digitalWrite(2, HIGH);
	myGLCD.drawBitmap(0, 0, IH, 84, 48);
	Serial.println("on");
	Serial.print("number of button pushes:  ");
	Serial.println(counter);
	
}

void led2()  //LED 2
{
	digitalWrite(2, LOW);
	digitalWrite(3, HIGH);
	myGLCD.drawBitmap(0, 0, MENU1, 84, 48);
	Serial.println("on");
	Serial.print("number of button pushes:  ");
	Serial.println(counter);
}

void led3()  //LED 3
{
	digitalWrite(3, LOW);
	digitalWrite(4, HIGH);
	Serial.println("on");
	Serial.print("number of button pushes:  ");
	Serial.println(counter);
	
}
void led4() //LED ALL
{
	digitalWrite(2, HIGH);
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	delay(500);
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	delay(100);
	Serial.print("number of button pushes:  ");
	Serial.println(counter);
	Serial.print("ALL LEDS");	
}


