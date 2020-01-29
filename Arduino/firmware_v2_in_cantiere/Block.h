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

#include "Adafruit_NeoPixel.h"

class Block
{
private:
	uint8_t idx;
	uint8_t NBLOCKS;
	Block* blocksRef;

	// LED stuff
  Adafruit_NeoPixel* strip;
	// indexes of first/last pixel of the block
	uint16_t head, tail;
	// this are vars to keep track of changing colors
  uint8_t r;
  uint8_t g;
  uint8_t b;

  // how many leds are dedicated to the ball
  uint8_t LPB;

  enum {
    TOUCHED = 0,
    SUSTAINED,
		RELEASED,
    TEASING,
    WAITING,
    HUSHED
  } state = TEASING;

  // Normalized brightness level
  float FLOOR= 0.0; // brigthness when hushed
  float MIN  = 0.1; // brightenss when sin is at his minimum 
  float HALF = 0.2; // brightenss when sin is at his maximum
  float MAX  = 1;   // maximum brightenss when we are in a touched state.

  Animator_Line* lineA; // Sine  envelope
  Animator_Line* lineB; // Full brightness envelope
  Animator_Sine* sine;
  
	// time variables
	int timeToRampA = 1000;
	int timeToRampB = 2000;
	int timeToWait  = 1000;
  int timeToSustain = 5000;
  long t; // a variable to keep track of the passing time

  // a value to keep track of the
  // animation movements
  float y = 0.0;
public:
	Block() {};
  // to each block we must pass:
  // 1. the number of blocks;
  // 2. a reference to the Blocks array;
  // 3. the block index;
  // 4. the # of LEDs per block;
  // 5. a reference to the LED strip objects;
	void init(uint8_t _NBLOCKS, Block* _blocksRef, uint8_t _idx, uint8_t _LPB, Adafruit_NeoPixel* _strip);
	void update();
  //void triggerTeaser();

  void touch();
  //void release(); //2019-11-10 - not used anymore
  // with hush and unHush we intend messages the block is sending
  // to its neighbors to make them fade out and fade in their light respectively.
	void hush();
	void unhush();

  float getY();
  void debug();
  void printState();

  void hushNeighbors();
  void unhushNeighbors();
};
#endif
