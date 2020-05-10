# Maple_TFT_ST7735_TextTerminal
Have had some difficulties implementing this Terminall as debug solution using this serial-to-tft app.
Because theree are some various implemtations for TFT ST7735 models. But thes models may variy on their hardware properties.
So this is my solution with adapted Adafruit-Libs for this particular case (without SD-card-slot).

Implementation and setup for 128x160 Pixel Display with 1.8'' size with Maple-Mini using SPI_1 

    //--- these are the connections for the MapleMini to display

    #define sclk  PA5

    #define mosi  PA7  

    #define cs    PA4   // If cs and dc pin allocations are changed then 

    #define dc    PB0   // comment out #define F_AS_T line in "Adafruit_ILI9341_FAST.h" which is inside "Adafruit_ILI9341_AS" library.

    #define rst   0     // Can alternatively connect this to the Arduino reset

    Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(cs, dc, rst); // Invoke custom library



<img src="https://github.com/juergs/Maple_TFT_ST7735_TextTerminal/blob/master/ST7750_Terminal_Beta.png" alt="breadboard-prototype" style="width:800px;"/>


Todo:

Expand from  
 [Arduino_STM32](https://github.com/rogerclarkmelbourne/Arduino_STM32) to  [stm32duino](https://github.com/stm32duino/Arduino_Core_STM32]-Arduino_Core_STM32[/url]-version ...
 
 
`/*
 * Blink without delay and UART Test
 *
 * https://github.com/stm32duino/wiki/wiki/API#hardwareserial
*/
#include <HardwareSerial.h>

// Variables:
int previousMillis = 0;        // will store the last time the LED was updated
int interval = 500;            // interval at which to blink (in milliseconds)
uint32_t counter = 0;

  //                      RX    TX
  //HardwareSerial Serial2(PA3, PA2);
  
  HardwareSerial Serial2(USART2, SERIAL_8N1 );
  
  //HardwareSerial Serial3(PB11, PB10);
  
  HardwareSerial Serial3(USART3, SERIAL_8N1 );


void setup() {
  // Set up the built-in LED pin as output:
  pinMode(PC13, OUTPUT);

  Serial2.setTx(PA9);
  Serial2.setRx(PA10);
  Serial2.setTx(PA2);
  Serial2.setRx(PA3);
  Serial3.setTx(PB10);
  Serial3.setRx(PB11);
  
  Serial.begin(115200); 
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
}

void loop() 
{
  // Check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time we blinked
  // the LED is bigger than the interval at which we want to blink
  // the LED.
  if (millis() - previousMillis > interval) {
    // Save the last time you blinked the LED
    previousMillis = millis();

    Serial.print("Counter: ");
    Serial.print(counter);
    Serial.println(" - Hello USB-Serial 0!");

    
    Serial1.print("Counter: ");
    Serial1.print(counter);
    Serial1.println(" - Hello UART 1!");

    Serial2.print("Counter: ");
    Serial2.print(counter);
    Serial2.println(" - Hello UART 2!");

    Serial3.print("Counter: ");
    Serial3.print(counter);
    Serial3.println(" - Hello UART 3!");

    counter ++;
    // If the LED is off, turn it on, and vice-versa:
    digitalWrite(PC13,!digitalRead(PC13));
    }
}

// ===> will be working on all Maple Hardware-Serial-Ports!` 
 
