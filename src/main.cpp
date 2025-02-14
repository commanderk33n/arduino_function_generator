#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <PWM.h> //PWM librarey for controlling freq. of PWM signal


//initialize the liquid crystal library
//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27,  16, 2);

const int Encoder_OuputA  = 11;
const int Encoder_OuputB  = 12;
const int Encoder_Switch = 10;
const int signal_pin = 9; 
const int Sine_pin = 5;
const int POT_pin = A2;

int Previous_Output;
int multiplier = 1;
double angle = 0;
double increment = 0.2;
int32_t frequency; //frequency to be set
int32_t lower_level_freq = 1; //Lowest possible freq value is 1Hz
int32_t upper_level_freq = 100000; //Maximum possible freq is 100KHz


void generate_sine()

{

   double sineValue = sin(angle);

   sineValue *= 255;

   int plot = map(sineValue, -255, +255, 0, 255);

   Serial.println(plot);

   analogWrite(Sine_pin,plot);

   angle += increment; 


   if (angle > 180)

   angle =0;

}




void setup() {
  
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();

  lcd.print("Signal Generator"); //Intro Message line 1
  lcd.setCursor(0, 1);
  lcd.print("-CircuitDigest "); //Intro Message line 2
  delay(2000);
  lcd.clear();
  lcd.print("Freq:00000Hz");
  lcd.setCursor(0, 1);
  lcd.print("Inc. by: 1 ");
  Serial.begin(9600); //Serial for debugging
  InitTimersSafe(lower_level_freq); //Initialize timers without disturbing timer 0

  //pin Mode declaration
  pinMode (Encoder_OuputA, INPUT);
  pinMode (Encoder_OuputB, INPUT);
  pinMode (Encoder_Switch, INPUT);
  Previous_Output = digitalRead(Encoder_OuputA); //Read the inital value of Output A


 attachInterrupt(0,generate_sine,CHANGE);

}


void loop()

{

  if (digitalRead(Encoder_OuputA) != Previous_Output)

   { 

     if (digitalRead(Encoder_OuputB) != Previous_Output) 

     { 

       frequency = frequency + multiplier;

      // Serial.println(frequency);

       pwmWriteHR(signal_pin, 32768); //Set duty cycle to 50% by default -> for 16-bit 65536/2 = 32768

       SetPinFrequencySafe(signal_pin, frequency);

       lcd.setCursor(0, 0);

       lcd.print("Freq:     Hz");

       lcd.setCursor(5, 0);

       lcd.print(frequency);

     } 

     else 

     {

       frequency = frequency -   multiplier;

      // Serial.println(frequency);

       pwmWriteHR(signal_pin, 32768); //Set duty cycle to 50% by default -> for 16-bit 65536/2 = 32768

       SetPinFrequencySafe(signal_pin, frequency);

       lcd.setCursor(0, 0);

       lcd.print("Freq:     Hz");

       lcd.setCursor(5, 0);

       lcd.print(frequency);

     }

   }


      if (digitalRead(Encoder_Switch) == 0)

   {

   multiplier = multiplier * 10;


   if (multiplier>1000)

   multiplier=1;

   

  // Serial.println(multiplier);

   lcd.setCursor(0, 1);

   lcd.print("Cng. by:     ");

   lcd.setCursor(8, 1);

   lcd.print(multiplier);

   

   delay(500);

   while(digitalRead(Encoder_Switch) == 0);

   }


   Previous_Output = digitalRead(Encoder_OuputA);  

 

}



