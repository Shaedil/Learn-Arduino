/*
Arduino Morse Coder & Reader

Simulates a telegraph with a button switch, LED, and Serial Monitor.

The circuit:
        * Connect 1 LED to a digital pin with 220 Ohm resistor
        * Connect 2 button switches to digital pins with 1K Ohm resistors.

Created 17 Feb 2021
By Shaedil Dider

GPLv3.0 License
*/

unsigned long limit, startTime, endTime;
int decoderButton = 12;
int encoderButton = 11;
int led = 10;
int dotDelay = 100;
String translated = "";
bool encoderButtonIsActive = false;

static String letters[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",   // A-I
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"    // S-Z
};
static String numbers[] = {
    "-----", ".----", "..---", "...--", "....-",     //0-4
    ".....", "-....", "--...", "---..", "----.", "N" //5-9
};

void setup() {
    Serial.begin(9600);
    pinMode(decoderButton, INPUT);
    pinMode(encoderButton, INPUT);
    pinMode(led, OUTPUT);
}

void loop() {
NextDotDash:
    // if you press blue button, change state from inactive to active
    // read serial monitor and etc while state is active
    // otherwise, go ahead and decode your morsecode.
    if (digitalRead(encoderButton) == HIGH) {
        encoderButtonIsActive = true;
        while (encoderButtonIsActive) {
            // while (digitalRead(encoderButton) == HIGH) {
            digitalWrite(led, LOW);
            char readCharacter;
            if (Serial.available()) {
                readCharacter = Serial.read();
                Serial.print(readCharacter);
                if (readCharacter >= 'a' && readCharacter <= 'z') {
                    flash(letters[readCharacter - 'a']);
                } else if (readCharacter >= 'A' && readCharacter <= 'Z') {
                    flash(letters[readCharacter - 'A']);
                } else if (readCharacter >= '0' && readCharacter <= '9') {
                    flash(numbers[readCharacter - '0']);
                } else if (readCharacter == ' ') {
                    delay(dotDelay * 4);
                } else if (readCharacter == '\n') {
                    encoderButtonIsActive = false;
                }
            }
        }
        encoderButtonIsActive = false;
    }
    while (digitalRead(decoderButton) == HIGH) {}
    startTime = millis();
    digitalWrite(led, HIGH);
    while (digitalRead(decoderButton) == LOW) {}
    endTime = millis();
    digitalWrite(led, LOW);
    limit = endTime - startTime;
    if (limit > 50) {
        translated += dotOrDash();
    }
    while ((millis() - endTime) < 500) {
        if (digitalRead(decoderButton) == LOW) {
            goto NextDotDash;
        }
    }
    convertor();
}

char dotOrDash() {
    if (limit < 600 && limit > 50) {
        return '.';
    } else if (limit > 600) {
        return '-';
    }
}

void convertor() {
    bool numberFound = false;
    int i = 0;
    while (numbers[i] != "N") {
        if (numbers[i] == translated) {
            Serial.print(char('0' + i));
            numberFound = true;
            break;
        }
        i++;
    }
    i = 0;
    if (translated == ".-.-.-") {
        Serial.print(".");
    } else {
        while (letters[i] != "E") {
            if (letters[i] == translated) {
                Serial.print(char('A' + i));
                break;
            }
            i++;
        } 
        if (letters[i] == "E" && numberFound == false) {
            Serial.println("<Wrong input>");
        }
    }
    translated = "";
    numberFound = false;
}

void flash(String sequence) {
    int i = 0;
    while (sequence[i] != 0) {
        flashDotsAndDashes(sequence[i]);
        i++;
    }
    delay(dotDelay * 3);
}

void flashDotsAndDashes(char ch) {
    digitalWrite(led, HIGH);
    if (ch == '.') {
        delay(dotDelay);
    } else {
        delay(dotDelay * 3);
    }
    digitalWrite(led, LOW);
    delay(dotDelay);
}