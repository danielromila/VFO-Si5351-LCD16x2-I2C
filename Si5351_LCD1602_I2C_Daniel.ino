                               // Made by Daniel Romila, VE7LCG
                               // If you do not like the rotation sense of your digital encoder and want to change it, just switch among them the wires going into D2 and D3
#include <LiquidCrystal_I2C.h> // fmalpartida library, from https://github.com/fmalpartida/New-LiquidCrystal , updated November 24th 2020
                               // if you have already a library LiquidCrystal_I2C in your library folder you need to delete it (or move it), otherwise will interfere with the useful library
#include <si5351.h>            // Adafruit Si5351 library, version 1.2.0

static const long Fmin =       4000;            // minimumm VFO frequency
static const long Fmax =  225000000;            // maximum VFO frequency
volatile long freq = 10000000;                  // the starting frequency
volatile long oldfreq = 0;                      // the previous frequency
volatile long IntermediateF = 10700000;         // intermediate frequency value
volatile long radix = 1000;                     // the modifying step between consecutive frequency
volatile long oldradix = 0;                     // the previous radix
unsigned int encoderA, encoderB, encoderC = 1;  // rotary encoder variables

// Rotary encoder pins and other inputs
static const int rotAPin = 2;
static const int rotBPin = 3;
static const int rotSwch = 4;

int fMHz;
int fKHz;
int fHz;

// Instantiate the display and the signal generator board
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);    //I2C Address 0x27 (or 0x3F)
Si5351 si5351(0x60); //I2C Address 0x60

void setup()
{
  // Set up I/O pins
  pinMode(rotAPin, INPUT);
  digitalWrite(rotAPin, HIGH); // internal pull-up 
  pinMode(rotBPin, INPUT);
  digitalWrite(rotBPin, HIGH); // internal pull-up 
  pinMode(rotSwch, INPUT);
  digitalWrite(rotSwch, HIGH); // internal pull-up 

  // Initialize the display
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  // Initialize the Si5351 signal generator module 
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 122400);  // the correction factor for the given Si5351 board, obtained with the calibration utility from Etherkit_Si5351 library       

  si5351.output_enable(SI5351_CLK0, 1); // 1 - Enables / 0 - Disables CLK
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.output_enable(SI5351_CLK2, 1);
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);
}

void loop()
{
  CheckEncoder();
  CheckRadixSwitch();
}

void CheckEncoder()
{
  byte encoderA = digitalRead(rotAPin);
  byte encoderB = digitalRead(rotBPin);

  if ((encoderA == HIGH) && (encoderC == LOW))
  {
    if (encoderB == HIGH)
    {
      // Decrease frequency
      freq = (freq - radix);
      if (freq < Fmin) freq = Fmin;
    }
    else
      // Increase frequency
      freq = (freq + radix);
      if (freq > Fmax) freq = Fmax;
  }
  encoderC = encoderA;

  if (freq != oldfreq)
  {
    UpdateDisplay();
      si5351.set_freq(((freq) * 100ULL), SI5351_CLK0);     // VFO
      si5351.set_freq((IntermediateF * 100ULL), SI5351_CLK2);              // BFO
    oldfreq = freq;
  }
}

void CheckRadixSwitch()
{
  if (digitalRead(rotSwch) == 0)
  {
    radix = radix * 10;
    if (radix > 10000000)
      radix = 1;
    delay(400);
  }
  if (radix != oldradix)
  {
    UpdateRadixDisplay();
    oldradix = radix;
  }
}


void UpdateDisplay()
{
  // setup the frequency                   
                   lcd.setCursor(0, 0);
                   lcd.print("                ");
                                      
                   fMHz = int(freq/1000000); 
                   fKHz = int((freq-fMHz*1000000)/1000);
                   fHz = int (freq-fMHz*1000000-fKHz*1000);
                   
                   if (fMHz >=100) lcd.setCursor(0, 0);
                   if (fMHz <100 && fMHz >= 10) 
                   {
                    lcd.print(" ");
                    lcd.setCursor(1, 0);
                   }
                   if (fMHz <10 && fMHz >= 1) 
                   {
                    lcd.print("  ");
                    lcd.setCursor(2, 0);
                   }
                   lcd.print(fMHz);
                   if (fMHz >=1)
                          {
                            lcd.setCursor(3, 0);
                            lcd.print(".");
                          }
                          
                          else
                          {
                          lcd.setCursor(0, 0);
                          lcd.print("    "); 
                          }        
                   if (fKHz >=100)
                   {
                     lcd.setCursor(4, 0);
                     lcd.print(fKHz);
                   }
                                     
                   if (fKHz <100 && fKHz >= 10) 
                   {
                    lcd.setCursor(4, 0);
                    lcd.print("0");
                    lcd.setCursor(5, 0);
                    lcd.print(fKHz);
                   }
                   if (fKHz <10 && fKHz >= 1)
                   {
                    lcd.setCursor(4, 0);
                    lcd.print("00"); 
                    lcd.setCursor(6, 0);
                    lcd.print(fKHz);                  
                   }
                   if (fKHz <1)
                   {
                    lcd.setCursor(4, 0);
                    lcd.print("000"); 
                   }                              
                    lcd.setCursor(7, 0);
                    lcd.print(".");
                    if(fMHz <1 && fKHz < 100)
                   {                                        
                    lcd.setCursor(0, 0);
                    lcd.print("     "); 
                   }                   
                    if(fMHz <1 && fKHz < 10)
                   {                                        
                    lcd.setCursor(0, 0);
                    lcd.print("      "); 
                   }                                    
                    if (fHz >=100)
                   {
                   lcd.setCursor(8, 0);
                   lcd.print(fHz);
                   }
                                     
                   if (fHz <100 && fHz >= 10) 
                   {
                    lcd.setCursor(8, 0);
                    lcd.print("0");
                    lcd.setCursor(9, 0);
                    lcd.print(fHz);
                   }
                   if (fHz <10 && fHz >= 1)
                   {
                    lcd.setCursor(8, 0);
                    lcd.print("00"); 
                    lcd.setCursor(10, 0);
                    lcd.print(fHz);                  
                   }
                   if (fHz <1)
                   {
                    lcd.setCursor(8, 0);
                    lcd.print("000"); 
                   }
                   if(fMHz>=1)
                   {
                    lcd.setCursor(13, 0);
                    lcd.print("MHz");
                   }
                   else
                   {
                    lcd.setCursor(13, 0);
                    lcd.print("KHz");
                   }
}

void UpdateRadixDisplay()
{
  //Update and display radix, the step between two consecutive frequencies
  if (radix == 10000000)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step: 10 MHz");
  }
  if (radix == 1000000)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step:  1 MHz");
  }
  if (radix == 100000)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step:100 KHz");
  }
  if (radix == 10000)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step: 10 KHz");
  }
  if (radix == 1000)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step:  1 KHz");
  }
  if (radix == 100)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step: 100 Hz");
  }
  if (radix == 10)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step:  10 Hz");
  }
  if (radix == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("    Step:   1 Hz");
  }
}
