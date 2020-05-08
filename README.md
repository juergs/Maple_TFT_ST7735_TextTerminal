# Maple_TFT_ST7735_TextTerminal
Have had some difficulties implementing this Terminall as debug solution using this serial-to-tft app.
Because theree are some various implemtations for TFT ST7735 models. But thes models may variy on their hardware properties.
So this is my solution with adapted Adafruit-Libs for this special case.

Implementation and setup for 128x160 Pixel Display with 1.8'' size with Maple-Mini using SPI_1 

    //--- these are the connections for the MapleMini to display

    #define sclk  PA5

    #define mosi  PA7  

    #define cs    PA4   // If cs and dc pin allocations are changed then 

    #define dc    PB0   // comment out #define F_AS_T line in "Adafruit_ILI9341_FAST.h" which is inside "Adafruit_ILI9341_AS" library.

    #define rst   0     // Can alternatively connect this to the Arduino reset

    Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(cs, dc, rst); // Invoke custom library



<img src="https://github.com/juergs/Maple_TFT_ST7735_TextTerminal/blob/master/ST7750_Terminal_Beta.png" alt="breadboard-prototype" style="width:800px;"/>
