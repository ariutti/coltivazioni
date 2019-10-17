#include "Block.h"

void Block::init(uint8_t _NBLOCKS, Block* _blocksRef, uint8_t _idx, uint8_t _LPB,
					/*Adafruit_DotStar* _strip*/
					Adafruit_NeoPixel* _strip)
{
  NBLOCKS = _NBLOCKS;
	blocksRef = _blocksRef;
	idx = _idx;
	strip = _strip;
	LPB = _LPB;
  tail = idx*LPB;
  head = tail + LPB - 1;

	//setColor(r, g, b);
  for(int j=0; j<LPB; j++) {
    strip->setPixelColor(tail+j, r, g, b);
  }
  //strip->show();

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

	// this are vars to keep track of changing colors
  r = 255.0 * y;
  g = 255.0 * y;
  b = 255.0 * y;

	for(int j=0; j<LPB; j++) {
    strip->setPixelColor(tail+j, r, g, b );
  }

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
    hushNeighbors();
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

void Block::hushNeighbors()
{
  // each block is also responsible to hush its neighbors
  if( idx == 0 )
  {
		blocksRef[1].hush();
  }
  else if (idx == NBLOCKS-1)
  {
		blocksRef[NBLOCKS-2].hush();
  }
  else
  {
		blocksRef[idx-1].hush();
		blocksRef[idx+1].hush();
  }
}

void Block::unhushNeighbors()
{
  // each block is also responsible to hush its neighbors
  if( idx == 0 )
  {
		blocksRef[1].unhush();
  }
  else if (idx == NBLOCKS-1)
  {
		blocksRef[NBLOCKS-2].unhush();
  }
  else
  {
		blocksRef[idx-1].unhush();
		blocksRef[idx+1].unhush();
  }
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
