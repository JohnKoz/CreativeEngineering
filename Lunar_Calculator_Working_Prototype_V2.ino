//arduino pins before the reshuffle:
////Stepper motor:
//A1 = digital pin 13
//A2 = digital pin 8
//A3 = digital pin 7
//A4 = digital pin 6
//Negative = GND
//Positive = +5V


////LCD Display Screen:
//VSS to GND Negative(-)
//VDD to to +5volts Positive (+)
//VO to digital pin 9
//RS to pin 12
//RW to GND Negative(-)
//E to pin 11
//skip the next 4
//D4 to pin 5
//D5 to pin 4
//D6 to pin 3
//D7 to pin 2
//A(anode) to digital pin 10
//K(cathode) to 220 ohm resistor to GND Negative(-)

////RTC - Real Time clock:
//GND to GND (-)
//VCC to +5V )+)
//SDA to Analog A4
//SCL to Analog A5
//--START HERE AND IN ONENOTE ON Saturday
//Just got the dates to correct themselves.
//Now need to integrate a time delay, maybe once a day in unix I recheck the days until full moon?


#include <LiquidCrystal.h> //Import the LCD library
#include "RTClib.h"

//Initialize
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); /*Initialize the LCD and
                                        tell it which pins is
                                        to be used for communicating*/
//Global Var
//Below it for stepper motor
#define A 13
#define B 8
#define C 7
#define D 6
//#define NUMBER_OF_STEPS_PER_REV 512
#define NUMBER_OF_STEPS_PER_REV 1
//above it for stepper motor
#define contra 9 //Define the pin that controls the contrast of the screen
#define bri 10  //Define the pin the controls the brightness of the screen
                //Both pins are PWM so you can analogWrite to them and have them output a variable value
                
int contrast = 220; //Contrast of 60 seems to be good for viewing text on LCD screen. LOW and HIGH are also available as well as 50-70 seems to work well for this.
int i = 0; //for stepper motor
unsigned long previousMillis = 0; //this will store the last time the moon date was check and the LCD was updated
const unsigned long interval = 60UL*60UL*1000UL; // Set the checking interval for once every 12 hours. 1000 ms in 1 second, 60 seconds per minute, 60 minutes per hour, 12 hours.
//const unsigned long interval = 3000UL; //For testing purposes use this for 3 second interval.
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib


RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  
  Serial.begin(9600);
  lcd.begin(16, 2); //Tell the LCD that it is a 16x2 LCD
  //For stepper Motor below
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  //For stepper motor above
  pinMode(contra, OUTPUT); //set pin 9 to OUTPUT
  pinMode(bri, OUTPUT); //Set pin 10 to OUTPUT...pinMode-ing OUTPUT makes the specified pin output power
  digitalWrite(contra, LOW); //outputs no power to the contrast pin. this lets you see the words
  analogWrite(bri, 255); //Outputs full power to the screen brightness LED
  analogWrite(contra, 60);

  //turn the LCD screen on and calculate the days
  

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // When time needs to be re-set on a previously configured device, the following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 

  //turn the LCD screen on and calculate the days
   calculateDaysUntilFullMoon();
}

void loop (){
  //Use the below for debugging: Especially to see how quickly the loop executes
  //Serial.println("-----------log 1.0 Start of Void Loop---------------");// log the start of the loop

  //Call delayByOneHour to see if it's been 1 hour since the last update.
  delayByOneHour(); 

  //Call onestep to run the stepper motors
  onestep();

  //For debugging purposes below:
  //Serial.println("Next part will determine the current Milliseconds and previous Milliseconds");
  //unsigned long currentMillis = millis();
  //Serial.print("currentMillis = ");
  //Serial.println(currentMillis);
  //Serial.print("previousMillis = ");
  //Serial.println(previousMillis);
  //Serial.print("interval = ");
  //Serial.println(interval);
}

//classes for stepper motor below:
void write(int a,int b,int c,int d){
digitalWrite(A,a);
digitalWrite(B,b);
digitalWrite(C,c);
digitalWrite(D,d);
}

void onestep(){
write(1,0,0,0);
delay(5);
write(1,1,0,0);
delay(5);
write(0,1,0,0);
delay(5);
write(0,1,1,0);
delay(5);
write(0,0,1,0);
delay(5);
write(0,0,1,1);
delay(5);
write(0,0,0,1);
delay(5);
write(1,0,0,1);
delay(5);
}

//classes for stepper motor above
void calculateDaysUntilFullMoon(){ //calcuate days until full moon, might need to not use void for this?

  Serial.println("We made it into the Calculate days until Full Moon Method!!!");
  
  DateTime now = rtc.now(); //get todays date/time
  long todayInUnix = now.unixtime(); //convert todays date/time to unix
  long lastFullmooninUnix = 1642438260; //This is 1/17/22 at 4:51PM MTN = 1642438260 in Unix.
                                        //TODO: I can update this in the future to get a more precise estimate.
                                        //Maybe I do this once a year? 
  long timeBetweenMoons = 2567100; //this is the unix time between lunar moons in seconds or 29.53058770576 days
  long nextFullMoonUnix = lastFullmooninUnix + timeBetweenMoons ; //Add 29.54 days to the last full moon (1/17/22 at 4:51PM MTN = 1642463460 in Unix
  
  //Below is to check for any restarts of the system. This could happen when it's unplugged. Want to make sure the nextFullMoonUnix is greater than todays date. Not sure what will happen when its the same date...
  //TODO: Test with the exact same time for todayinUnix
  while ((nextFullMoonUnix - todayInUnix) < 0){ //so todays date is greater than the next full moon, iterate 1 moon cycle and check again.
      nextFullMoonUnix = nextFullMoonUnix + timeBetweenMoons;
      Serial.println("We made it inside the while loop, has it been a while since we ran this program? I'm catching up to todays date. This could also cause problems when running on the day of the full moon...");
      Serial.println(nextFullMoonUnix);
  }
  //Calculate the time until the next full moon:
  long timeUntilNextMoon = (nextFullMoonUnix - todayInUnix);

  //LCD code below to display the Days until Full moon: 
  lcd.setCursor(0,0);
  lcd.print("Full Moon In:"); //Output " Full Moon In:" on the first line of the LCD
  lcd.setCursor(0, 1); /*Set the (invisible) cursor on the first place second row of the LCD.
                        Cursor values are 0-indexed, 0 would the be the first place.
                        The cursor coordinates are X,Y coordinates.*/
  lcd.print(timeUntilNextMoon /86400L); //86400L = long int and 86400 seconds in a day.
  lcd.print(" Days");
  lcd.setCursor(0,0);
 //LCD code above
 //For debugging below: 
//  Serial.print("Todays time in unix: ");
//  Serial.println(todayInUnix);
//  Serial.println("Next full moon in: ");
//  Serial.print(timeUntilNextMoon /86400L);
//  Serial.println(" Days");
  //For debugging above: 
}
void delayByOneHour(){
  /*This method below calculates using the system millis() method how long the system has been on for in milliseconds.  
   This unsigned long number SHOULD roll over, not causeing problems but this is where I could forsee 
   issues in the future with not rolling over gracefully. Seems to be 47 days until it rolls over. We shall see!*/
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis) >= interval){
  previousMillis = currentMillis;//set the previous millis variable to an iteration of the current interval.
   
   //call calculate days until full moon method which updates the lcd
   calculateDaysUntilFullMoon();
}
}
