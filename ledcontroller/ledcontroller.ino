#include <Adafruit_NeoPixel.h>

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)

String sdata = "";
byte r = 0;
byte g = 0;
byte b = 0;

#define LEDPIN D3
#define NUMPIXELS 151

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup (void) {
  Serial.begin(921600);
  Sprintln("Command Interpreter");
  pixels.begin();
}

void loop() {
  byte ch;
  int i = 0;

  if (Serial.available()) {
    ch = Serial.read();
    sdata += (char)ch;
    if (ch == '\r') {
      sdata.trim();
      do {
        String got = getValue(sdata, ';', i);
        switch (i) {
          case 0:
            r = got.toInt();
            Sprint("r: ");
            Sprintln(r);
            break;
          case 1:
            g = got.toInt();
            Sprint("g: ");
            Sprintln(g);
            break;
          case 2:
            b = got.toInt();
            Sprint("b: ");
            Sprintln(b);
            break;
        }
        i++;
      } while (getValue(sdata, ';', i) != "");

      sdata = "";
    }
  }

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();

}

String getValue(String dat, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = dat.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (dat.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? dat.substring(strIndex[0], strIndex[1]) : "";
}
