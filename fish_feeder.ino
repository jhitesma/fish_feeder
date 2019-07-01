/*
====================================================================================================
==Created 5/3/2013 by Brian Tremblay and motified by Cory Oberdas
==Purpose: Run a Fish Feeder Motor every X Hours, and also provide a button to run anytime.
==Platform: Arduino Uno or Mega
==Version: 4.2
====================================================================================================
*/
#include <AccelStepper.h>

//--Setting Vars


   //Fish Feeder Vars.
int pinOutLed = 5;
int pinInButton = 2;
int MotorRunDuration = 1000;
float AutoWaitDurationHours = 0.;
//--Program Vars
unsigned long currentMsSinceStart = 0;
unsigned long msValueAtLastRun = 0;
float HoursSinceLastRun = 0;
int buttonValue = 0;

#define HALFSTEP 4

// Motor pin definitions
#define motorPin1  10     // IN1 on the ULN2003 driver 1
#define motorPin2  11     // IN2 on the ULN2003 driver 1
#define motorPin3  12     // IN3 on the ULN2003 driver 1
#define motorPin4  13     // IN4 on the ULN2003 driver 1

AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);


void setup() {
  Serial.begin(9600);             //Turns on serial monitor.
  pinMode(pinInButton, INPUT);    //Manual feed.
  pinMode(pinOutLed, OUTPUT);     //Shows motor is on when lite.
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(50);
  stepper1.setSpeed(300);
}

void loop() {
  HoursSinceLastRun = AutoWaitDurationHours - 0.05; //this will make it run 3 minutes after turned on.

//Fish Feeder
  //get miliseconds since program start and calc.
  currentMsSinceStart = millis();
  //HoursSinceLastRun = (currentMsSinceStart - msValueAtLastRun) / 43200000; //10000 millis= 10 seconds.  To change the time to vary feeding times set for 12hrs type in 43200000 or 6hrs tyoe in 21600000.
  //check for button press
  buttonValue = digitalRead(pinInButton);
  if(buttonValue == HIGH){  //had to change state because of button from LOW to HIGH.
    //run motor for button press.
    RunMotor();
    Serial.println("Feed Fish Manually");  //serial print funtion completed by button

  } else if(HoursSinceLastRun > AutoWaitDurationHours){
    RunMotor();
    msValueAtLastRun = currentMsSinceStart;
    Serial.println("Feed Fish Auto");      //serial print funtion completed by auto wait duration hours
  }
  //slow the loop down so it isn't running at full CPU Utilization for nothing.
  delay(50); //delay 1/20th of a second.
}

void RunMotor(){
    analogWrite(pinOutLed, 80);        // turn on green led to show motor is on.
    stepper1.setCurrentPosition(0);
    stepper1.moveTo(600);
    stepper1.runToPosition();
    stepper1.moveTo(-4000);
    stepper1.runToPosition();
    stepper1.disableOutputs();
    analogWrite(pinOutLed, 0);        // turn off green led to show motor is off.
}
