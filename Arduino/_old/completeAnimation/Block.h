#ifndef __BLOCK__
#define __BLOCK__
#include "Arduino.h"
#include "Animator_Line.h"
#include "Animator_Sine.h"



class Block
{
private:
  enum {
    ATK = 0,
    SUSTAIN,
    HOLD,
    RLS,
    REST,
    TEASERATK,
    TEASER
  } state = REST;
  
	Animator_Line* line;
  Animator_Sine* sine;
  
  float FLOOR= 0.0;
  float MIN  = 4;
  float HALF = 8;
  float MAX  = 10;
  
  // a value to keep track of the
  // animation movements
  float y = 0.0;
public:
	Block() {};
	void init();
	void update();
  void triggerTeaser();
  void touch();
  void release();
  float getY();
  void debug();
};
#endif
