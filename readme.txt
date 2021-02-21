You can see it in a youtube video at:
https://youtu.be/53N_Wv3248g

If you do not like the rotation sense of your digital encoder and want to change it, just switch among them the wires going into D2 and D3

Libraries:
#include <LiquidCrystal_I2C.h> // fmalpartida library, from https://github.com/fmalpartida/New-LiquidCrystal , updated November 24th 2020
                               // if you have already a library LiquidCrystal_I2C in your library folder you need to delete it (or move it), 
                                  otherwise will interfere with the useful library
#include <si5351.h>            // Adafruit Si5351 library, version 1.2.0

The Si5351 signal generator board has 3 clocks. I use only CLK0 and CLK2.
- CLK 0 generates a variable frequency, between 4 KHz and 225 MHz (the board is version A, Si5351A)
- CLK 2 generates a fixed frequency, which I called in the program IntermediateF, and has a fixed value of 10.7 MHz,
  which can be changed in the program.
