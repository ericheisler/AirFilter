/*
 Test code for reading values from a 
 Sharp GP2Y1010AU0F dust sensor
 
 sensor pin 1 (V-LED)   => 5V (through 150ohm resister)
 sensor pin 2 (LED-GND) => GND
 sensor pin 3 (LED)     => pin 2
 sensor pin 4 (S-GND)   => GND
 sensor pin 5 (Vo)      => pin A0
 sensor pin 6 (Vcc)     => 5V
 
 */

// the pins
#define sensorPin A0
#define ledPin 2

// timing numbers
#define delayOnToSense 280
#define delaySenseToOff 40
#define delayOff 9680

int sensorVal;
int aveCounter;
unsigned long aveVal, millivolts;
float dustDensity;

void setup(){
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin, HIGH);
  
  aveCounter = 0;
  aveVal = 0;
}

void loop(){
  if(aveCounter == 16){
    // send data and restart average
    aveCounter = 0;
    sendData();
    aveVal = 0;
  }
  
  digitalWrite(ledPin,LOW); // power on the LED
  delayMicroseconds(delayOnToSense);
  sensorVal = analogRead(sensorPin); // read the dust value
  aveVal += sensorVal;
  delayMicroseconds(delaySenseToOff);
  digitalWrite(ledPin,HIGH); // turn the LED off
  delayMicroseconds(delayOff);
  
  aveCounter++;
}

void sendData(){
  // calculate the stuff
  millivolts = aveVal*625/2048;
  dustDensity = aveVal/16*(5.0/6138) - 0.1;
  aveVal = aveVal/16;
  
  Serial.print("aveVal =  ");
  Serial.print(aveVal);
  Serial.print('\t');
  Serial.print("mV =  ");
  Serial.print(millivolts);
  Serial.print('\t');
  Serial.print("dust = ");
  Serial.println(dustDensity, 4);
  
}

