#include "Block.h"

void Block::init()
{
  lineA = new Animator_Line();
	lineA->init(0.0, timeToRampA);
	lineB = new Animator_Line();
	lineB->init(0.0, timeToRampB);
  sine = new Animator_Sine();
  sine->init(1.0, 0.0);
	state = TEASING;
	lineA->reach(1.0, timeToRampA);
}

void Block::update()
{
  if( state == RELEASED )
  {
		if(lineB->getState() == Animator_Line::QUIET) {
			state = WAITING;
			t = millis();
		}

  }
  else if( state == WAITING )
  {
		if( millis() - t > timeToWait)
		{
      Serial.print("**  TIME PASSED **\n");
			state = TEASING;
			sine->setFreq(1.0); // set a random frequency
			lineA->reach(1.0, timeToRampA);
		}
  }

	// update the enveloped sine
	lineA->update();
	y = lineA->getY();
  // first a ADD argulment then a MUL argument.
	sine->update( y*(HALF+MIN)*0.5, y*(HALF-MIN)*0.5);
	// update the full brightness animator
	lineB->update();
	y = sine->getY() + lineB->getY();

  //line->printState();
	//Serial.println(y);
}

void Block::touch()
{
	if( state != TOUCHED)
	{
	  state = TOUCHED;
		lineA->reach(0.0, timeToRampA);
	  lineB->reach(1.0, timeToRampB);
	}
}

void Block::release()
{
	if(state == TOUCHED )
	{
		state = RELEASED;
		lineB->reach(FLOOR, timeToRampB);
	}
}

void Block::hush()
{
	if( state == TEASING )
	{
		lineA->reach(0.0, timeToRampA);
		state = HUSHED;
	}
	else if( state == TOUCHED )
	{
		// do nothing
	}
	else if( state == WAITING )
	{
		// do nothing
	}
	else if( state == HUSHED )
	{
		// do nothing
	}
}

void Block::unhush()
{
	if( state == HUSHED )
	{
		lineA->reach(1.0, timeToRampA);
		state = TEASING;
	}
}

/*
void Block::triggerTeaser()
{
  state = TEASERATK;
  line->reach(1, 1000);
}
*/

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

void Block::printState()
{
  Serial.print("[BLOCK: ");

  switch( state ) {
    case TOUCHED:
      Serial.print("TOUCHED");
      break;
    case RELEASED:
      Serial.print("RELEASED");
      break;
    case TEASING:
      Serial.print("TEASING");
      break;
    case WAITING:
      Serial.print("WAITING");
      break;
    case HUSHED:
      Serial.print("HUSHED");
      break;
    default:
      break;
  }
  Serial.print(" ]\t");
  
	Serial.print("[A: ");
	lineA->printState();
	Serial.print(" - ");
	Serial.print(lineA->getY() );
	Serial.print(" ]\t");
	Serial.print("[B: ");
	lineB->printState();
	Serial.print(" - ");
	Serial.print(lineB->getY() );
	Serial.print(" ]\t");
	Serial.print("y (sum): ");
	Serial.print( y );
	Serial.println();
}
