#include "Block.h"

void Block::init()
{
  line = new Animator_Line();
	line->init(0.0, 5000);
}

void Block::update()
{
	line->update();
	float y = line->getY();
  line->printState();
	//Serial.println(y);
}
