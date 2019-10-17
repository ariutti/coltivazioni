#include "Animator_ASHR.h"


void Animator_ASHR::init( int _t1, int _t2, int _holdtime ) {
  t1 = _t_1>0.0?_t_1:10;
  t2 = _t_2>0.0?_t_2:500;
  HOLDTIME = _holdtime>0.0?_holdtime:5000;
  state = QUIET;
}

void Animator_ASHR::update()
{
  if(state == ATTACK) {
    dt = millis() - st;
    y =  (1.0*dt)/t1;
    if(y > 1.0) {
      y = 1.0;
      state = SUSTAIN;
    }
  } else if( state == SUSTAIN ) {
    // wait for a release event
  } else if( state == HOLD ) {
    dt = millis() - st;
    if(dt > HOLDTIME ) {
      state = RELEASE;
      st = millis();
    }
    
  } else if( state == RELEASE ) {
    dt = millis() - st;
    y =  1.0 - ( (1.0*dt)/t2 );
    if(y < 0.0) {
      y = 0.0;
      state = QUIET;
    }
  }
}

void Animator_ASHR::triggerAttack() {
  state = ATTACK;
  st = millis();
}

void Animator_ASHR::triggerRelease(){
  if(state == SUSTAIN) {
    st = millis();
    state = HOLD;
  }
  else if (state == ATTACK) {
    st = int( millis()*(1+((1.0*t2)/t1)) - t2*(1 + ((1.0*st)/t1)) );
    state = RELEASE;
  }
}

float Animator_ASHR::getY() {
  return y;
}
