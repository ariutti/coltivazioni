/*
* A block is a single element which will manage:
* - a 16 pixels LED strip
* - a single capacitive PAD of the MPR121
* - all the animation related to light;
* The block probably should have a knoledge of
* all/part of all others block sourrounding it
* in order to trigger particulare animation of them.
*/

#ifndef __BLOCK__
#define __BLOCK__

#include "Arduino.h"
#include "Animator_Line.h"
#include "Animator_Sine.h"

class Block
{
private:
  enum {
    TOUCHED = 0,
		RELEASED,
    TEASING,
    WAITING,
    HUSHED
  } state = TEASING;

	Animator_Line* lineA; // Sine  envelope
  Animator_Line* lineB; // Full brightness envelope
  Animator_Sine* sine;

  float FLOOR= 0.0;
  float MIN  = 0.3;
  float HALF = 0.7;
  float MAX  = 1;

	// time variables
	int timeToRampA = 1000;
	int timeToRampB = 3000;
	int timeToWait  = 5000;
  long t; // a variable to keep track of the passing time

  // a value to keep track of the
  // animation movements
  float y = 0.0;
public:
	Block() {};
	void init();
	void update();
  //void triggerTeaser();

  void touch();
  void release();
	void hush();
	void unhush();

  float getY();
  void debug();
  void printState();
};
#endif
