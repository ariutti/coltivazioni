#ifndef __BLOCK__
#define __BLOCK__
#include "Arduino.h"
#include "Animator_Line.h"
#include "Animator_Sine.h"

class Block
{
private:
	Animator_Line* line;
public:
	Block() {};
	void init();
	void update();
};
#endif
