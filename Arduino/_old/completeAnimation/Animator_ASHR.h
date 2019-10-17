#ifndef __ANIMATOR_ASHR__
#define __ANIMATOR_ASHR__
#include "Arduino.h"

class Animator_ASHR
{

private:

  // NOTE: that time variables must be
  // long in order to store great values.

  // x axis represents time
  int t1, t2;  // t attack, t release
  int HOLDTIME; // hold time
  int dt;      // delta time
  int st;      // start time

  float y;

  // states
  enum {
    QUIET = -1,
    ATTACK,
    SUSTAIN,
    HOLD,
    RELEASE
  } state;


public:
  Animator_ASHR() {};

  // pass an initial value to start moving from
  // and a time to get there
  void init( int t_1, int t_2, int holdtime );
  void resetStart( float _start )
  void update();
  void reach(float _target, long _rampTime);
  float getY();
  int getState();
  void printState();
};
#endif
