
#ifndef __LINE_ANIMATOR__
#define __LINE_ANIMATOR__
#include "Arduino.h"

extern bool DEBUG;

class Animator_Line
{

private:

  // NOTE: that time variables must be
  // long in order to store great values.

  // x axis represents time
  int t;     // utility var: time to reach target value
  long x;    // utility var: keeps track of the passing time
  float y;   // utility var: interpolated value between y1 and y2

  long x1;    // start time
  long x2;    // end time
  float y1;   // start value
  float y2;   // end value

  // states
  enum {
    QUIET = -1,
    RAMPUP,
		RAMPDOWN
  } state;


public:
	Animator_Line() {};

	// pass an initial value to start moving from
	// and a time to get there
	void init( int _start, int _t );
  void update();
  void reach(float _target, long _rampTime);
  float getY();
	int getState();
  void printState();
};
#endif
