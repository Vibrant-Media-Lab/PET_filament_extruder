/*
pullstruder heater control
thermistor&resistor joined on A0
3.3V joined to resistor
GND joined to thermistor
relay signal joined to D13 (built in LED)

using excerpts or concepts from: 
thermistor stuff: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
a4988 driver stuff: https://www.youtube.com/watch?v=0qwrnUeSpYQ
potentiometer stuff: https://docs.arduino.cc/learn/electronics/potentiometer-basics
*/

//pin for thermistor read
#define ThermistorPin A0

float setT = 116.3; //the set temperature for nozzle
int samples = 30; //num temp samples to take

int i;
//int V[6];
float R1 = 10000;
float logR2, R2, T, Vo;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int potPin = A1;  // where the potentiometer is hooked up
int potVal = 0;  // variable to store its value

const int dirPin = 2;  // stepper direction pin D2
const int stepPin = 3;  // step pin D3
const int STEPS_PER_REV = 200;  // motor steps per revolution
int x = 0;  //loop counter for motor


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(ThermistorPin, INPUT);
  pinMode(potPin, INPUT);

  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);

  pinMode(4, OUTPUT);

  Serial.begin(9600);
  // connect AREF to 3.3V as it is less noisy than 5V:
  // analogReference(INTERNAL);
}



void loop() {
    digitalWrite(dirPin,HIGH);  // set direction clockwise
    digitalWrite(4,HIGH);  //set MS2 on a4988 to high, enabling quarter steps
    // begin thermistor code
    Vo = 0;
    // for (i=0; i<samples; i++)  
    // {
    //   Vo = Vo + analogRead(ThermistorPin);
    //   //delay(2);
    //   digitalWrite(stepPin,HIGH); //step
    //   delay((potVal/100)+1);      //step delay
    //   digitalWrite(stepPin,LOW);  //step
    //   delay((potVal/100)+1);      //step delay
    // }

    // Vo = Vo / samples;
    Vo = analogRead(ThermistorPin);
    Serial.println(Vo);
    R2 = R1 * (1023 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    T = T - 273.15; 
    T = (T * 9.0)/ 5.0 + 32.0; 
  
    Serial.print("Thermistor: ");
    Serial.print(R2);
    Serial.println(" ohms");
    Serial.print("Vo: ");
    Serial.println(Vo);
    Serial.print("Temperature: ");
    Serial.print(T);
    Serial.println(" F");

    if ((T < setT))  
    {
      digitalWrite(LED_BUILTIN, LOW);  //turn on builtin LED and close heater relay
    }
    else if (T > setT)  
    {
      digitalWrite(LED_BUILTIN, HIGH);   //turn off builtin LED and open heater relay
    }
    // delay(200);
    // end thermistor code

    // begin potentiometer read code
    potVal = analogRead(potPin);
    Serial.print("potVal: ");
    Serial.print(potVal);
    Serial.print("\r\n");
    // end potentiometer read code

    // begin a4988 stepper driver code
    digitalWrite(dirPin,HIGH);  // set direction clockwise

    
    while(x < 30)  {
      digitalWrite(stepPin,HIGH);
      delay((potVal/100)+1);
      digitalWrite(stepPin,LOW);
      delay((potVal/100)+1);
      x++;
    }
    x=0;
    // end a4988 stepper driver code 
}
