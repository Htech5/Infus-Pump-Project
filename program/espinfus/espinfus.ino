#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6LYLsc1bi"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "vLGKKXedSsmD9ioHqgWgKHRW-13_vmVx"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "Projectkita_id";
char pass[] = "caritaudong";
SoftwareSerial serial(D1, D2);
String data;
char c;
String dt[10];
unsigned long waktunow;
unsigned long waktulast;

unsigned long waktunow1;
unsigned long waktulast1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}
void loop() {

  // put your main code here, to run repeatedly:
  Blynk.run();
  while (serial.available() > 0) {
    delay(10);
    c = serial.read();
    data += c;
  }
  if (data.length() > 0) {
    waktunow = millis();
    waktunow1 = millis();
    Serial.println(data);
    int j = 0;
    dt[j] = "";
    //proses parsing data
    for (int i = 0; i < data.length(); i++)
    {
      if ((data[i] == '#') || (data[i] == ','))
      {
        j++;
        dt[j] = "";
      }
      else
      {
        dt[j] = dt[j] + data[i];
      }
    }
    Serial.print("data 1 : ");
    Serial.print(dt[0]);
    Serial.print("\n");
    Serial.print("data 2 : ");
    Serial.print(dt[1]);
    Serial.print("\n");
    Serial.print("data 3 : ");
    Serial.println(dt[2]);

    Blynk.virtualWrite(V7, dt[0]);
    Blynk.virtualWrite(V1, dt[1]);
    Blynk.virtualWrite(V4, dt[2]);



    data = "";
  }
}
