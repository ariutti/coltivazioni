#include "Block.h";
#define DELAY_TIME 50
int counter = 0;

Block block;

#define MINVALUE 4
#define MAXVALUE 8

bool prevStatus = false;



void setup() {
	Serial.begin(9660);
	//sine.init(1, 0.0);
  //line.init(0.0, 5000);
  pinMode(2, INPUT_PULLUP);
  block.init();

}

void loop() {
  if(!digitalRead(2) && !prevStatus) {
    counter = (++counter)%2;
    //line.reach(counter, 1000);
    block.triggerTouch();
    prevStatus = true;
  } else if (digitalRead(2) && prevStatus) {
    prevStatus = false;
  }
  block.update();
  block.debug();
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
