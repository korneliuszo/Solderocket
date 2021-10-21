int buttonPin = 2;
int potPin = 0;
int mosfetPin = 5;
int potVal = 0;

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

bool state = false;

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


  attachInterrupt(digitalPinToInterrupt(buttonPin), fire, RISING);

  // initialize serial communication with computer:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

}

void loop() {

  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(potPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits

  average = map(average, 0, 1023, 50, 1000);


  showLeds(average,50,1000);
 
 
  delay(1);        // delay in between reads for stability

 Serial.println(average);

 if (state) {
    digitalWrite(mosfetPin, HIGH); // sets the digital pin 13 on
    delay(average);            // waits for a second
    digitalWrite(mosfetPin, LOW);
    state=false;
 }

}

void fire() {

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    state = true;
  }
  last_interrupt_time = interrupt_time;
 
 
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
