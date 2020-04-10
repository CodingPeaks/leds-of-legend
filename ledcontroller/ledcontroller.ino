#include <Adafruit_NeoPixel.h>

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)
#define fstep 10
String sdata = "";

byte target_r = 0;
byte target_g = 0;
byte target_b = 0;

unsigned long fade = 10;
byte r = 0;
byte g = 0;
byte b = 0;

unsigned long curb = 0;
unsigned long curtime = 0;

#define LEDPIN 22
#define NUMPIXELS 151

Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

void setup (void) {
  Serial.begin(115200);
  Sprintln("Command Interpreter");
  pixels.begin();
}

void loop() {
  byte ch;
  int i = 0;

  if (Serial.available()) {
    curtime = micros();
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
          case 3:
            fade = got.toInt();
            Sprint("f: ");
            Sprintln(fade);
            break;
        }
        i++;
      } while (getValue(sdata, ';', i) != "");

      sdata = "";
    }
  }

  fadeTo();

}

void fadeTo() {


  if (micros() - curtime > fade) {

    if (target_r < r && !(abs(target_r - r) < fstep)) {
      target_r = target_r + fstep;
    } else if (target_r > r && !(abs(target_r - r) < fstep)) {
      target_r = target_r - fstep;
    }

    if (target_g < g && !(abs(target_g - g) < fstep)) {
      target_g = target_g + fstep;
    } else if (target_g > g && !(abs(target_g - g) < fstep)) {
      target_g = target_g - fstep;
    }

    if (target_b < b && !(abs(target_b - b) < fstep)) {
      target_b = target_b + fstep;
    } else if (target_b > b && !(abs(target_b - b) < fstep)) {
      target_b = target_b - fstep;
    }

    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }

    Serial.print(target_r);
    Serial.print(" ");
    Serial.print(target_g);
    Serial.print(" ");
    Serial.println(target_b);

    pixels.show();

    curtime = micros();
  }
}

String getValue(String dat, char separator, int index)
{
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
