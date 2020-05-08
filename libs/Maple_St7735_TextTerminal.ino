/***************************************************
 * 
 * Maple_St7735_TextTerminal
 * 
 * *************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  ILI9341 = ST7735-Controller
  
 ****************************************************/

/*
*/
#include "SPI.h"
#include <Adafruit_GFX_AS.h>
#include <Adafruit_ILI9341_STM.h>

#define ILI9341_VSCRDEF 0x33
#define ILI9341_VSCRSADD 0x37
#define ILI9341_GREY 0x5AEB

//--- ST7735 TFT, 1.8'' 128x160, using MAPLE-Mini-SPI-Interface #1,  
//--- these are the connections for the MapleMini to display

#define sclk  PA5
#define mosi  PA7  
#define cs    PA4   // If cs and dc pin allocations are changed then 
#define dc    PB0   // comment out #define F_AS_T line in "Adafruit_ILI9341_FAST.h" which is inside "Adafruit_ILI9341_AS" library.
#define rst   0     // Can alternatively connect this to the Arduino reset

Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(cs, dc, rst); // Invoke custom library

//-------------------------------------------
// The scrolling area must be a integral multiple of TEXT_HEIGHT
#define   TEXT_HEIGHT 		16 	// Height of text to be printed and scrolled
#define   BOT_FIXED_AREA 	0 	// Number of lines in bottom fixed area (lines counted from bottom of screen)
#define   TOP_FIXED_AREA 	16 	// Number of lines in top fixed area (lines counted from top of screen)
#define   MAX_X				    160 // for landscape mode 
#define   MAX_Y 			    128 // for landscape mode 

uint16_t  yStart      =   TOP_FIXED_AREA;                           // The initial y coordinate of the top of the scrolling area
uint16_t  yArea       =   MAX_Y - TOP_FIXED_AREA - BOT_FIXED_AREA;  // --- we are using our display in landscape mode (setrotation=0, Y=128 X=160) 
uint16_t  yDraw       =   MAX_Y - BOT_FIXED_AREA - TEXT_HEIGHT;     //--- initial y coordinate of the top of the bottom text line  
uint16_t  xPos        =   0;    //--- keep track of the drawing x coordinate
byte      data        =   0;    //--- for the byte we read from the serial port
bool      change_colour  = 1;    //---  test variables used during debugging
bool      selected  	   = 1;    //---  test variables used during debugging

//--- we have to blank the top line each time the display is scrolled, but this takes up to 13 milliseconds
//--- for a full width line, meanwhile the serial buffer may be filling... and overflowing  
//--- we can speed up scrolling of short text lines by just blanking the character we drew
// We keep all the strings pixel lengths to optimise the speed of the top line blanking
int blank[19]; // in der Schrift pro Zeile 19 Zeichen.

//---------------------------------------------------------------------------------
void setup() 
{
  delay(2000);  //--- wait for serial usb settle 
  
  //--- setup TFT 
  tft.begin();
  tft.setRotation(1);    //--- special modes in this library: 0..3 and 7, see setRotion in lib
  
  //tft.invertDisplay(true);  // nur Farben 
  
  //tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen2(ILI9341_BLACK);
  
  //--- define scroll area
  setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);
  
  //-- start serial 
  Serial.begin(115200);
  
  tft.setTextColor(ILI9341_WHITE, ILI9341_RED);  
  tft.fillRect(0,0,128,16, ILI9341_BLUE);
  

  delay(5000);

  //tribute to not _AX
  tft.drawCentreString("Serial Terminal 115200 ",64,0,2);

  // Change colour for scrolling zone
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

  //--- prefill the array
  for (byte i = 0; i<18; i++) 
	blank[i] = 0;
}
//---------------------------------------------------------------------------------
void loop(void) 
{
  //  These lines change the text colour when the serial buffer is emptied
  //  These are test lines to see if we may be losing characters
  //  Also uncomment the change_colour line below to try them
  //
  //  if (change_colour){
  //  change_colour = 0;
  //  if (selected == 1) {tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK); selected = 0;}
  //  else {tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK); selected = 1;}
  //}
  
  while (Serial.available()) 
  {
		//--- first byte of incoming serial data available (or -1 if no data is available). Data type: int.
    data = Serial.read();
    //Serial.print("E: ");    
    if (data == '\r' || xPos>(160-9) || byte(data) == 10 )     //231
    {
      xPos = 0;
      yDraw = scroll_line(); // It takes about 13ms to scroll 16 pixel lines
      Serial.println(yDraw);
    }
    if (data > 31 && data < 128) 
    {
      xPos += tft.drawChar(data, xPos, yDraw, 2);
      
      blank[(18+(yStart-TOP_FIXED_AREA)/TEXT_HEIGHT)%19] = xPos; // Keep a record of line lengths
    }
    //change_colour = 1; // Line to indicate buffer is being emptied
  }
}

//---------------------------------------------------------------------------------
// Call this function to scroll the display one text line
//---------------------------------------------------------------------------------
int scroll_line() 
{
  int yTemp = yStart; // Store the old yStart, this is where we draw the next line

  // Use the record of line lengths to optimise the rectangle size we need to erase the top line
  tft.fillRect(0, yStart, blank[(yStart-TOP_FIXED_AREA)/TEXT_HEIGHT], TEXT_HEIGHT, ILI9341_BLACK);

  // Change the top of the scroll area
  yStart += TEXT_HEIGHT;
  
  // The value must wrap around as the screen memory is a circular buffer
  //if (yStart >= 128 - BOT_FIXED_AREA) 
  if (yStart >= 128 - 16 - BOT_FIXED_AREA) 
    yStart = TOP_FIXED_AREA + (yStart - 128-16 + BOT_FIXED_AREA);
  
  // Now we can scroll the display
  scrollAddress(yStart);
  return  yTemp;
}

//---------------------------------------------------------------------------------
// Setup a portion of the screen for vertical scrolling
//---------------------------------------------------------------------------------
// We are using a hardware feature of the display, so we can only scroll in portrait orientation
void setupScrollArea(uint16_t TFA, uint16_t BFA) 
{
  tft.writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
  tft.writedata(TFA >> 8);
  tft.writedata(TFA);
  tft.writedata((128-TFA-BFA)>>8);
  tft.writedata(128-TFA-BFA);
  tft.writedata(BFA >> 8);
  tft.writedata(BFA);
}

//---------------------------------------------------------------------------------
// Setup the vertical scrolling start address
//---------------------------------------------------------------------------------
void scrollAddress(uint16_t VSP) 
{
  tft.writecommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  tft.writedata(VSP>>8);
  tft.writedata(VSP);
}
