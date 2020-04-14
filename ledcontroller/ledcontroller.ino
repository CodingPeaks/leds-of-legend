#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)

AsyncWebServer server(80);

int ahtt = 0;
int rhtt, ghtt, bhtt;
String sdata = "";
float r = 0;
float g = 0;
float b = 0;
float r_target = 0;
float g_target = 0;
float b_target = 0;
float f = 30;
bool flag = true;
bool rcvhtt = true;

unsigned long pixelsInterval = 50;
unsigned long rainbowPreviousMillis = 0;
unsigned long rainbowCyclesPreviousMillis = 0;
int rainbowCycles = 0;
int rainbowCycleCycles = 0;

#define LEDPIN D3
#define NUMPIXELS 151
#define STASSID "contact1"
#define STAPSK  "0210111966"

const char* ssid = STASSID;
const char* password = STAPSK;

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup (void) {
  Serial.begin(115200);
  pixels.begin();
  initSPIFFS();
  startwifi();
  waitcon();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");

    rcvhtt = true;
    ahtt = 0;

    int paramsNr = request->params();

    for (int i = 0; i < paramsNr; i++) {

      AsyncWebParameter* p = request->getParam(i);

      if (p->name() == "a") {
        ahtt = p->value().toInt();
      }

      if (p->name() == "r") {
        rhtt = p->value().toInt();
      }

      if (p->name() == "g") {
        ghtt = p->value().toInt();
      }

      if (p->name() == "b") {
        bhtt = p->value().toInt();
      }

      if (p->name() == "d") {
        pixelsInterval = p->value().toInt();
      }

      if (!ahtt) {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(rhtt, ghtt, bhtt));
        }
        pixels.show();
      }
      Serial.println("------");
    }

  });

  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  byte ch;
  int i = 0;

  if (Serial.available()) {
    rcvhtt = false;
    ch = Serial.read();
    sdata += (char)ch;
    if (ch == '\r') {
      sdata.trim();
      flag = true;
      do {
        String got = getValue(sdata, ';', i);
        switch (i) {
          case 0:
            r_target = got.toFloat();
            Sprint("r_target: ");
            Sprintln(r_target);
            break;
          case 1:
            g_target = got.toFloat();
            Sprint("g_target: ");
            Sprintln(g_target);
            break;
          case 2:
            b_target = got.toFloat();
            Sprint("b_target: ");
            Sprintln(b_target);
            break;
          case 3:
            f = got.toFloat() / 10;
            Sprint("f: ");
            Sprintln(f);
            break;
        }
        i++;
      } while (getValue(sdata, ';', i) != "");

      sdata = "";
    }
  }

  if (!rcvhtt) {
    ahtt = 0; //stoppa animazione
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color((int) r, (int) g, (int) b));
    }
    pixels.show();
    fade();
  }

  switch (ahtt) { //in base all'animazione
    case 1: //animazione 1...
      if ((unsigned long)(millis() - rainbowPreviousMillis) >= pixelsInterval) {
        rainbowPreviousMillis = millis();
        rainbow();
      }
      break;

    default:
      break;
  }
}
