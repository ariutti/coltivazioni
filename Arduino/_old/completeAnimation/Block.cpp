#include "Block.h"

void Block::init()
{
  line = new Animator_Line();
	line->init(0.0, 5000);
  sine = new Animator_Sine();
  sine->init(1.0, 0.0);
}

void Block::update()
{
  if( state == ATK )
  {
  
  }
  else if( state == SUSTAIN )
  {
    
  }
  else if( state == HOLD )
  {
    
  }
  else if( state == RLS )
  {
    
  }
  else if( state == REST )
  {
    y = 0.0;
  }
  else if( state == TEASERATK ) {
    line->update();
    y = line->getY();
    sine->update(y*(HALF+MIN)*0.5, y*(HALF-MIN)*0.5);
    y = sine->getY();
  }
  
  //line->printState();
	//Serial.println(y);
}

void Block::triggerTeaser()
{
  state = TEASERATK;
  line->reach(1, 1000);
}

void Block::touch() 
{
  state = ATK;
  line->resetStart(y);
}

void Block::release()
{
  
}

float Block::getY() 
{
  return y;
}

void Block::debug() {
  Serial.print(y); Serial.print("\t");
  Serial.print(HALF); Serial.print("\t");
  Serial.print(MIN); Serial.print("\t");
  Serial.print(MAX); //Serial.print("\t");
  Serial.println();
}
