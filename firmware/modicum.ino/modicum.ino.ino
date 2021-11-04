int buttonPin = 15;
int potPin = 0;
int mosfetPin = 16;
int potVal = 0;

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int buttonState = 1;         // current state of the button, HIGH as we have active low with INPUT_PULLUP
int lastButtonState = 1;     // previous state of the button

const int ledCount = 10;    // the number of LEDs in the bar graph
int ledPins[] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};   

void setup() {
  
  // loop over the pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  }
  
  pinMode(mosfetPin, OUTPUT);  
  pinMode(buttonPin, INPUT);    
  pinMode(potPin, INPUT);  

  pinMode(2, OUTPUT); // Do not know why it did not set in the above for loop

}

void loop() {

  buttonState = digitalRead(buttonPin);
    // compare the buttonState to its previous state
    if (buttonState != lastButtonState) {
  
      if (buttonState == LOW) {  
        // wend from off to on:
          digitalWrite(mosfetPin, LOW);
       
      } else {
        // if the current state is LOW then the button
        // wend from on to off:

        digitalWrite(mosfetPin, HIGH); // sets the digital pin 13 on
        delay(average);            // waits for a second
        // if in full throtle do not make it of
        if (average < 999) {
          digitalWrite(mosfetPin, LOW);
        }
          
       
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    // save the current state as the last state,
    //for next time through the loop
    lastButtonState = buttonState;

  average = readPot();

  showLeds(average,50,1000);
 
}

int readPot() {
  
  return map(analogRead(potPin), 0, 1023, 50, 1000);
  
}

void showLeds(int avarage,int mymin,int mymax){

  int ledLevel = map(avarage, mymin, mymax, 0, ledCount);

  // loop over the LED array:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    }
    // turn off all pins higher than the ledLevel:
    else {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
  
}
