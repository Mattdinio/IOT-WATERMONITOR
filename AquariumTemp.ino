/*
 * Required Libraries
 * Blynk
 * OneWire
 * DallasTemperature
 * adafruit_SSD1306
 * adafruit_GFX
 * Wire.h
 */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "tuT64tM5P_fj91Oq2ZJuuWfWfCyW1SLj";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "BTHub6-5CWF";
char pass[] = "Di7Wdf3grTXw";

#define ONE_WIRE_BUS 2 //D4 Pin
#define PowerLED 14 // D5 Pin
#define ProblemLED 12 // D6 Pin
#define OLED_RESET LED_BUILTIN // 
#define LCDHEIGHT 32
#define LCDWIDTH 128
OneWire oneWire (ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
Adafruit_SSD1306 display(LCDWIDTH, LCDHEIGHT, &Wire, OLED_RESET);
float temp;
float celcius=0;
int lowTempWarning = 5; //customisable low temperature warning
int highTempWarning = 28; //customisable high temperature warning
WidgetTerminal terminal(V9);
BlynkTimer timer;






void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(PowerLED, HIGH);
  pinMode(ProblemLED, HIGH);
  digitalWrite(PowerLED, HIGH);
  //Blynk.begin(auth, ssid, pass);
  DS18B20.begin();
  terminal.clear();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,214), 8080);
  Blynk.virtualWrite(V2, 7); // Sensor nor implemented, set to 7 for demo purposes
  Blynk.virtualWrite(V3, lowTempWarning);
  Blynk.virtualWrite(V4, highTempWarning);
  timer.setInterval(1000L, sendData);

}

void loop()
{
  Blynk.run();
  DS18B20.requestTemperatures();
  celcius=DS18B20.getTempCByIndex(0);
  screenData();
  timer.run();
     
 }

 void sendData()
 {
      celcius=DS18B20.getTempCByIndex(0);
      terminal.println(celcius);
      Blynk.virtualWrite(V1, celcius);
      if (celcius > highTempWarning) {
        terminal.println("Temp to high");
        Blynk.notify("Temperature to High");
        digitalWrite(ProblemLED, HIGH);
      } else if (celcius < lowTempWarning) { 
        terminal.println("Temp to low");
        digitalWrite(ProblemLED, HIGH);
        } else {
        digitalWrite(ProblemLED, LOW);
        }
 }


 void screenData()
 {
  celcius=DS18B20.getTempCByIndex(0);
  display.clearDisplay();
  display.drawRect(1,1,126,31,WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(2,2);
  display.print(celcius);
  display.display();
  Serial.println(celcius);
 }
