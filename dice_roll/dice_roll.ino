/*
Arduino Dice Roller

Simulates throwing a die with 6 LEDS

The circuit:
        * Connect 6 LEDS to consecutive digital pins with 220 Ohm resistors
        * Connect button switch to digital pin

Created 14 Feb 2021
By Shaedil Dider

GPLv3.0 License
*/

#define DEBUG 0

// 6 consecutive digital pins for the LEDs
int first = 2;
int second = 3;
int third = 4;
int fourth = 5;
int fifth = 6;
int sixth = 7;

int button = 12;

int pressed = 0;

void setup() {
        for (int i=first; i<=sixth; i++) {
                pinMode(i, OUTPUT);
        }
        pinMode(button, INPUT);
        // initialize random seed by noise from analog pin 0 (should be unconnected)
        randomSeed(analogRead(0));

#ifdef DEBUG
        Serial.begin(9600);
#endif

}

void lightHalfOfLEDS(int first, int second, int third) {
        digitalWrite(first, HIGH);
        digitalWrite(second, HIGH);
        digitalWrite(third, HIGH);
        delay(200);
        digitalWrite(first, LOW);
        digitalWrite(second, LOW);
        digitalWrite(third, LOW);
        delay(200);
}

void buildUpTension() {
        lightHalfOfLEDS(3, 5, 7);
        lightHalfOfLEDS(2, 4, 6);
        lightHalfOfLEDS(3, 5, 7);
}
void showNumber(int number) {
        digitalWrite(first, HIGH);
        if (number >= 2) { digitalWrite(second, HIGH); }
        if (number >= 3) { digitalWrite(third, HIGH); }
        if (number >= 4) { digitalWrite(fourth, HIGH); }
        if (number >= 5) { digitalWrite(fifth, HIGH); }
        if (number == 6) { digitalWrite(sixth, HIGH); }
}
int throwDice() {
        // get a random number in the range [1,6]
        int randNumber = random(1,7);

#ifdef DEBUG
        Serial.println(randNumber);
#endif

        return randNumber;
}

void setAllLEDs(int value) {
        for (int i=first; i<=sixth; i++) {
                digitalWrite(i, value);
        }
}
void loop() {
        // if button is pressed - throw the dice
        pressed = digitalRead(button);
        if (pressed == HIGH) {
                // remove previous number
                setAllLEDs(LOW);
                buildUpTension();
                int thrownNumber = throwDice();
                showNumber(thrownNumber);
        }
}
