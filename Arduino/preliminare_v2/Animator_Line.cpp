#include "Animator_Line.h"

// Set ramp initial value
// and ramp time in millisecond
void Animator_Line::init( float _start, int _t ) {
	t = _t>0.0?_t:5000;
	state = QUIET;
  y1 = y = _start;
	y2 = 1.0;
}

void Animator_Line::setStart( float _start ) {
  y1 = y = _start;
}


void Animator_Line::update() {
	 if( state == RAMPUP ) {
		 x = millis();
		 y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
		 if( y > y2 ) {
			 y = y1 = y2;
			 state = QUIET;
		 }
	 } else if( state == RAMPDOWN ) {
		x = millis();
		y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
		if( y < y2 ) {
			y = y1 = y2;
			state = QUIET;
		}
	} else if( state == QUIET ) {
		// do nothing
	}
 }

// a function to say the animator to move to a new value
// in a specified amount of time (ms)
void Animator_Line::reach(float _target, long _rampTime) {
  //println("target: " + target + "; rampTime: " + rampTime + ";");
	y2 = _target;
	t = _rampTime;
  x1 = millis();
  x2 = x1 + t;
  y1 = y;
	if( y2 > y1 )
		state = RAMPUP;
	else if( y2 < y1 )
		state = RAMPDOWN;
  else
    state = QUIET;
}

float Animator_Line::getY()
{
	return y;
}

int Animator_Line::getState()
{
	return state;
}


void Animator_Line::printState()
{
  switch(state) {
    case QUIET:
      Serial.print("QUIET");
      break;
    case RAMPUP:
      Serial.print("RAMPUP");
      break;
		case RAMPDOWN:
	    Serial.print("RAMPDOWN");
	    break;
    default:
			// do nothing
      break;
  }
  //Serial.println();
}
