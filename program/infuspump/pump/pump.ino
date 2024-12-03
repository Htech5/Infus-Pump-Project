z#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Stepper.h>

SoftwareSerial mySerial(12, 13);
#define I2CADDR 0x20
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //three columns
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = { 0, 1, 2, 3 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 5, 6, 7 };  //connect to the column pinouts of the keypad
Keypad_I2C kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR);

LiquidCrystal_I2C lcd(0x27, 16, 2);
int fase = 0;
int kondisi = 0;
int sensor = 10;
int A = 0;
int B = 0;
int state = 0;
String message;
int QTY, numMessages, endBytes;
byte inByte;
int flag = 0;
String kecepatan = "";
int speednya;

long curretntmillis = 0;
int Flowrate = 240;  // Enter callibrated flow rate here "4ml/minute"
int FlowfACTOR = 0;
int Volume = 0;
int Call = 0;

unsigned long timeNow;
unsigned long timeLast = 0;
unsigned long interval = 1000;

unsigned long tetesNow;
unsigned long tetesLast = 0;
float tetesan;
float laju;
float volume = 0;

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define MOTOR_STEPS 200
#define STEPS 200
Stepper stepper(STEPS, dirPin, stepPin);
void setup() {
  pinMode(sensor, INPUT);
  pinMode(7, OUTPUT);  //LED
  Wire.begin();
  kpd.begin(makeKeymap(keys));
  numMessages, endBytes = 0;
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("system start");
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Infus Pump");
  lcd.setCursor(4, 1);
  lcd.print("Arduino");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Input Volume:");
  lcd.setCursor(0, 1);
  FlowfACTOR = Flowrate * 2.5;
  delay(1000);
}

void loop() {
  timeNow = millis();
  if (kondisi == 0) {
    char key = kpd.getKey();
    if (key != NO_KEY) {
      Serial.println(key);
      if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') {
        lcd.setCursor(0, 1);
        kecepatan = kecepatan + key;
        lcd.print(kecepatan);
        Serial.println("oke");
      } else if (key == '*') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Input Volume:");
        lcd.setCursor(0, 1);
        kecepatan = "";
      } else if (key == '#') {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tetesan: ");
        lcd.print(speednya);
        lcd.print("ml/h");
        lcd.setCursor(0, 1);
        lcd.print("V: ");
        lcd.print(speednya);
        lcd.print("ml/h");
        lcd.print(" S: ");
        lcd.print(speednya);
        speednya = kecepatan.toInt();
        Serial.println(speednya);
        Serial.println(kecepatan.toInt());
        digitalWrite(7, HIGH);

        kondisi = 1;
      }
    }
  }


  else if (kondisi == 1) {
    tetesNow = millis();
    char key = kpd.getKey();
    Serial.println(key);
    int sensorValue = analogRead(A0);
    Volume = FlowfACTOR * speednya;

    for (int i = 0; i < Volume; i++) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("V :");
      lcd.print(speednya);
      lcd.print("ml ");
      lcd.setCursor(0, 1);
      lcd.print("S :");
      lcd.print(speednya);
      mySerial.print(String(laju) + "#" + String(Volume) + "#" + String(speednya) + "#");
      stepper.setSpeed(30000);
      stepper.step(100);
      delayMicroseconds(100);
    }
    speednya = 0;
    if (key == 'D') {
      lcd.clear();
      kondisi = 0;
      lcd.setCursor(0, 0);
      lcd.print("Input Volume:");
      lcd.setCursor(0, 1);
      kecepatan = "";
    }
  }
}
