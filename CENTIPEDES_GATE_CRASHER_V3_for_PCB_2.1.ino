//HOLMES MODULAR - CASEY HOLMES - "CENTIPEDES VOLTAGE CONTROLLED GATE CRASHER" 5-25-23

// pin definitions
const int enableLed = 5;                      //green input led
const int buttonPin = 2;                      //button is here
const int enablePin = 4;                      //clock input transistor pin
const int outputLed = 6;                      //red output LED
const int output = 3;                         //output transistor on pin 3

//variables
int toggleState;                              //variable to record button presses
int lastButtonState = 1;                      //variable to record previous button states
long unsigned int lastPress;                  //timestamp for working with button
volatile int buttonFlag;                      //global button flaggin variable for ISR
int debounceTime = 50;                        //variable for storing debounce time
unsigned long previoustime1 = 0;               //stores previous time stamps from millis
unsigned long previoustime2 = 0;               //stores previous time stamps from millis
unsigned long currenttime1 = 0;                //stores current time stamps from millis
unsigned long currenttime2 = 0;                //stores current time stamps from millis
volatile int enableState = 0;                 //variable to store current clock status
int ledState1 = 0;
int ledState2 = 0;
int outputState1 = 0;
int outputState2 = 0;
unsigned long interval1 = 1000;                //variable to adjust timing of flip flop  //add volatile iff needed for global variables
unsigned long interval2 = 500;                 //variable to adjust timing of flip flop

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);           // intilize button as input with internal pullup resistor
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, CHANGE); //trigger interrupt on change of button
  pinMode(output, OUTPUT);                    //initialize output transistor
  pinMode(outputLed, OUTPUT);                 //initialize output LED as output
  digitalWrite(outputLed, 0);                 //initialize output LED as off
  pinMode(enableLed, OUTPUT);                 //initialize the LED pin as an output:
  digitalWrite(enableLed, 0);                 //initialize cock LED as off
  pinMode(enablePin, INPUT);                  //initialize clock pin as input
}

void loop() {
  enableState = digitalRead(enablePin);      

  if((millis() - lastPress) > debounceTime && buttonFlag){
    lastPress = millis();   //update lastPress      
                                                   
    if(digitalRead(buttonPin) == 0 && lastButtonState == 1){    //if button is pressed and was released last change
      toggleState =! toggleState;                 //turn on master clock with button by flipping variable state
      lastButtonState = 0;    //record the lastButtonState
    }
    
    else if(digitalRead(buttonPin) == 1 && lastButtonState == 0)    //if button is not pressed, and was pressed last change
    {
      lastButtonState = 1;    //record the lastButtonState
    }
    buttonFlag = 0;
  }

if(toggleState|!enableState){
interval1 = map(analogRead(A0), 0, 1023, 1, 1000);//measure speed pot on A0 and call it interval1 for later calculations
interval2 = map(analogRead(A1), 0, 1023, 1, 1000);//measure length pot on A1 and call it interval2 for later calculations
currenttime1 = millis();                       //take a time stamp to prepare for blinking the red output LED
   if (currenttime1 - previoustime1 >= interval1){//if current time (x) minus previous time (0) reaches greater than or equal to interval1 (1000ms)
      previoustime1 = currenttime1;               //write current time into previous time to reset counter
   
      if (ledState1 == LOW) {                     //if the LED variable is equal to LOW (0) 
         ledState1 = HIGH;                        //turn it HIGH
         } else {                                 //or else if it is HIGH,
         ledState1 = LOW;                         //turn it LOW
      }

      if (outputState1 == LOW) {                  //if the LED is off 
         outputState1 = HIGH;                     //turn it on
         } else {                                 //or else if it is on,
         outputState1 = LOW;                      //turn it off
      }   
   digitalWrite(outputLed, ledState1);            //write the ledState1 variable to the red output LED once each time through the loop
   digitalWrite(output, outputState1);            //write the outputState1 varibale to the output once through each loop
   }
  
   currenttime2 = millis();                       //take a time stamp to prepare for blinking the green input LED
   if (currenttime2 - previoustime2 >= interval2){//if current time (x) minus previous time (0) reaches greater than or equal to interval2 (500ms)
      previoustime2 = currenttime2;               //write current time into previous time to reset counter
 
      if (ledState2 == LOW) {                     //if the LED is off 
         ledState2 = HIGH;                        //turn it on
         } else {                                 //or else if it is on,
         ledState2 = LOW;                         //turn it off
      }

      if (outputState2 == LOW) {                  //if the LED is off 
         outputState2 = HIGH;                     //turn it on
         } else {                                 //or else if it is on,
         outputState2 = LOW;                      //turn it off
      }
   digitalWrite(enableLed, ledState2);             //write the ledState2 variable to the green output LED once each time through the loop
   digitalWrite(output, outputState2);             //write the outputState2 varibale to the output once through each loop
   }
}else{
digitalWrite(outputLed, LOW);                     //turn off outputLed when not in use
digitalWrite(output, LOW);                        //turn off output when not in use
digitalWrite(enableLed, LOW);                      //turn off clockLed when not in use
}
}

void ISR_button()                                                 //interrupt service routine
{
  buttonFlag = 1;                                                 //let buttonFlag = 1
}
