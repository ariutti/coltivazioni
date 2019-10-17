#include "Block.h";
#define DELAY_TIME 50
int counter = 0;

Block block;

#define LED 9

bool prevStatus = false;


// SETUP /////////////////////////////////////////////////////////
void setup()
{
	Serial.begin(9660);
	//sine.init(1, 0.0);
  //line.init(0.0, 5000);
  pinMode(2, INPUT_PULLUP);
  block.init();
  pinMode(LED, OUTPUT);
}


// LOOP //////////////////////////////////////////////////////////
void loop()
{
	// temporary utility for debug
  if(!digitalRead(2) && !prevStatus)
	{
    counter = (++counter)%2;
    //line.reach(counter, 1000);
    block.touch();
    prevStatus = true;
  }
	else if (digitalRead(2) && prevStatus)
	{
    prevStatus = false;
  }


  block.update();
  //block.debug();
  block.printState();
	float y = block.getY();
  analogWrite( LED, y * 255);

  //float y;
  //line.update();
  //y = line.getY();
  //sine.update(y*(MAXVALUE+MINVALUE)*0.5, y*(MAXVALUE-MINVALUE)*0.5);
  //y = sine.getY();
  //Serial.print(y); Serial.print("\t");
  //Serial.print(MINVALUE); Serial.print("\t");
  //Serial.println(MAXVALUE);

	delay(DELAY_TIME);
}


/************************************************************************************
 * SERIAL EVENT
 ***********************************************************************************/
void serialEvent()
{
  byte c = Serial.read();
  if (c == 't') {
    block.touch();
  }
  else if (c == 'r') {
    block.release();
  }
  else if (c == 'h') {
		block.hush();
  }
	else if (c == 'u') {
		block.unhush();
  }
}
