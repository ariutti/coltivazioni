/*
La console prevede che ogni blocco sia sempre in uno stato di pulsazione luminosa (stato _teaser_).
Quando un blocco subisce un tocco, la luminosità relativa sale al 100% e rimane nello stato _sustain_ per un tempo limite prima di avviare la fase di _release_.
Opzionalmente i blocchi che si trovano ai lati di quello selezionato, si spengono per accentuare l'attenzione sul blocco appena toccato.

La console è' multiutente? Sì. Tutti i blocchi possono essere toccati simultaneamente (21).
Il tocco non viene disabilitato dai pulsanti che hanno luminosità pari a zero.
in altri termini, non disabilitiamo il tocco di quelli che si spengono, i quali restano comunque attivi. 
*/

// BLOCKs ////////////////////////////////////////////////////////
#include "Block.h";
#define NBLOCKS 2 //21
Block blocks[NBLOCKS];

// LEDs //////////////////////////////////////////////////////////
// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Leds Per Block: how many LEDs ar dedicated to each block
const uint8_t LPB = 16;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define DATAPIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT (LPB*NBLOCKS)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, DATAPIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

// CAPACITIVE STUFF //////////////////////////////////////////////
#include "Limulo_MPR121.h"
const uint8_t NMPR = 1;
const uint8_t NPADS[] = {2, 5, 5, 6}; // NPALLINE {5, 5, 5, 6}
#define FIRST_MPR_ADDR 0x5A

struct mpr121
{
  Limulo_MPR121 cap;
  uint8_t addr;
  // Save an history of the pads that have been touched/released
  uint16_t lasttouched = 0;
  uint16_t currtouched = 0;
  uint16_t oor=0;
  boolean bPadState[12];
};
byte b;
int filt;
int base;

// an array of mpr121! You can have up to 4 on the same i2c bus
// mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D
mpr121 mpr[NMPR];

// UTILITIES /////////////////////////////////////////////////////
bool bVVVV = true;
bool DEBUG = false;
bool bToPlotter = false;
#define DELAY_TIME 10

// SETUP /////////////////////////////////////////////////////////
void setup()
{
	Serial.begin(9600);

	// PIN STUFF **************************************************/
	pinMode(6, OUTPUT);

	// LED STUFF **************************************************/
  strip.begin(); // Initialize pins for output

	// CAPACITIVE STUFF *******************************************/
  if(DEBUG) Serial.println("Looking for MPRs!");
  // cycle through all the MPR
  for(uint8_t i=0; i<NMPR; i++)
  {
    mpr[i].cap = Limulo_MPR121();
    mpr[i].addr = FIRST_MPR_ADDR + i;

    // Look for the MPR121 chip on the I2C bus
    if ( !mpr[i].cap.begin( mpr[i].addr ) )
    {
      if( DEBUG ) Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    if( DEBUG ) Serial.print("MPR ");
    if( DEBUG ) Serial.print(i);
    if( DEBUG ) Serial.println(" found!");

    // initialize the array of booleans
    for(int j=0; j<12; j++) {
      mpr[i].bPadState[j] = false;
    }

    // possibly initialize the mpr with some initial settings
    mpr[i].cap.setUSL(201);
    mpr[i].cap.setTL(180);
    mpr[i].cap.setLSL(130);

    // First Filter Iteration
    // Second Filter Iteration
    // Electrode Sample Interval
    // NOTE: the system seems to behave
    // better if these value are more than 0
    mpr[i].cap.setFFI_SFI_ESI(1, 1, 1);  // See AN3890

    // MHD, NHD, NCL, FDL
    mpr[i].cap.setFalling( 1, 1, 2, 1 );
    mpr[i].cap.setRising( 1, 8, 1, 1 );
    mpr[i].cap.setTouched( 1, 1, 1 );
    mpr[i].cap.setThresholds( 24, 10 );
    mpr[i].cap.setDebounces(2, 2);
  }

	// BLOCK STUFF ************************************************/
  for(uint8_t i=0; i<NBLOCKS; i++)
  {
    if(DEBUG) {
      Serial.print("Inizializzazione blocco ");
      Serial.print(i);
      Serial.println(";");
    }
    // to each block we are passing 
    // 1. the number of blocks;
    // 2. a reference to the Blocks array;
    // 3. the block index;
    // 4. the # of LEDs per block;
    // 5. a reference to the LED strip objects;
    blocks[i].init(NBLOCKS, blocks, i, LPB, &strip);
  }

  //strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
	// Turn all LEDs off ASAP
	strip.show();
}


// LOOP //////////////////////////////////////////////////////////
void loop()
{

	// CAPACITIVE STUFF *******************************************/
  // cycle through all the MPRs
  for(uint8_t i=0; i<NMPR; i++)
  {
    // Get the currently touched pads
    mpr[i].currtouched = mpr[i].cap.touched();

    // cycle through all the electrodes
    for(uint8_t j=0; j<NPADS[i]; j++)
    {
      if (( mpr[i].currtouched & _BV(j)) && !(mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'j' has been touched
        mpr[i].bPadState[j] = true;
        
        uint8_t index = composeIndex(i, j);
        blocks[ index ].touch();

        if( bVVVV ) printAllSensors(); // Send serial data to VVVV
      }
      if (!(mpr[i].currtouched & _BV(j)) && (mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'i' has been released
        mpr[i].bPadState[j] = false;
        /*
        uint8_t index = composeIndex(i, j);
        blocks[ index ].release();
        */

        if( bVVVV ) printAllSensors(); // Send serial data to VVVV
      }
    }
    // reset our state
    mpr[i].lasttouched = mpr[i].currtouched;

    mpr[i].oor = mpr[i].cap.getOOR();

		if(bToPlotter)
    {
      /*
      // ### NEW COMMUNICATION PROTOCOL (19-02-2018) ###
      //
      // Send data via serial:
      // 1. 'Status Byte': first we send a byte containing the address of the mpr.
      //    The most significant bit of the byte is 1 (value of the byte is > 127).
      //    This is a convention in order for the receiver program to be able to recognize it;
      // 2. Then we send 'Data Bytes'. The most significant bit of these bytes is
      //    always 0 in order to differenciate them from the status byte.
      //    We can send as many data bytes as we want. The only thing to keep in mind
      //    is that we must be coherent the receiver side in order not to create confusion
      //    in receiving the data.
      //
      //    For instance we can send pais of byte containing the 'baseline' and 'filtered'
      //    data for each mpr pad.
      //
      //    We can also use data bytes for sending information as:
      //    * 'touched' register;
      //    * 'oor' register;


      // 1. write the status byte containing the mpr addr
      b = (1<<7) | i;
      Serial.write(b);
      // 2. write 'touched' register
      b = mpr[i].currtouched & 0x7F;
      Serial.write(b); //touch status: pad 0 - 6
      b = (mpr[i].currtouched>>7) & 0x7F;
      Serial.write(b); //touch status: pad 7 - 12 (eleprox)
      // 3. write 'oor' register
      b = mpr[i].oor & 0x7F;
      Serial.write(b); //oor status: pad 0 - 6
      b = (mpr[i].oor>>7) & 0x7F;
      Serial.write(b); //oor status: pad 7 - 12 (eleprox)

      // Cycle all the electrodes and send pairs of
      // 'baseline' and 'filtered' data. Mind the bit shifting!
      for(uint8_t j=0; j<NPADS; j++)
      {
        base = mpr[i].cap.baselineData(j);
        filt = mpr[i].cap.filteredData(j);
        Serial.write(base>>3); // baseline is a 10 bit value
        Serial.write(filt>>3); // sfiltered is a 10 bit value
      }
      */
      // Send data via serial:
      // 1. First send a byte containing the address of the mpr + the address of the pad +
      //    the 'touched' status of the pad; This byte has a value greater than 127 by convention;
      // 2. Then send two more bytes for 'baseline' and 'filtered' respectively.
      //    Because these values are 10bit values and we must send them
      //    inside a 7bit packet, we must made a 3 times bit shift to the right (/8).
      //    This way we will loose some precision but this is not important.
      //    This two other bytes have values lesser than 127 by convention.

      // cycle all the electrodes
      for(int j=0; j<NPADS[i]; j++)
      {
        filt = mpr[i].cap.filteredData(j);
        base = mpr[i].cap.baselineData(j);
        b = (1<<7) | (i<<5) | (j<<1) | mpr[i].bPadState[j];
        Serial.write(b); // send address & touched
        Serial.write(base / 8); // send base value
        Serial.write(filt / 8); // send filtered value
      }

    } // if(bToPlotter)
    //mpr[i].cap.printOOR(); // added for debug purposes
  } //   for(int i=0; i<NMPR; i++)


	// BLOCK STUFF ************************************************/
	for(uint8_t i=0; i<NBLOCKS; i++) {
		blocks[i].update();
	}

	// LED STUFF **************************************************/
	strip.show();

	delay(DELAY_TIME);
}


/************************************************************************************
 * COMPOSE INDEX
 ***********************************************************************************/
int composeIndex(int mprIndex, int padIndex) {
  int acc = 0;
  for(int i=0; i<mprIndex; i++) {
    acc += NPADS[i];
  }
  return acc + padIndex;  
}



/************************************************************************************
 * SERIAL EVENT
 ***********************************************************************************/
void serialEvent()
{
  byte c = Serial.read();
	/*
  if (c == 't') {
    block.touch();
  }
  else if (c == 'r') {
    block.release();
  }
  else if (c == 'h') {
		block.hush();
  }
	else if (c == 'u') {
		block.unhush();
  }
	*/
}

/************************************************************************************
 * PRINT ALL SENSORS
 ***********************************************************************************/
void printAllSensors()
{
  // cycle through all the mpr
  for(uint8_t i=0; i<NMPR; i++)
  {
    // cycle through all the electrodes
    for(uint8_t j=0; j<NPADS[i]; j++)
    {
      int state = (mpr[i].currtouched & _BV(j)) >> j;
      Serial.print( state );
    }
  }
  Serial.println(";");
}
