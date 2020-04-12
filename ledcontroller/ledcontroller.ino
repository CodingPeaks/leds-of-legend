#include <Adafruit_NeoPixel.h>

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)

String sdata = "";
byte r = 0;
byte g = 0;
byte b = 0;
byte r_target = 0;
byte g_target = 0;
byte b_target = 0;
int f = 30;

#define LEDPIN D3
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
    ch = Serial.read();
    sdata += (char)ch;
    if (ch == '\r') {
      sdata.trim();
      do {
        String got = getValue(sdata, ';', i);
        switch (i) {
          case 0:
            r_target = got.toInt();
            Sprint("r_target: ");
            Sprintln(r_target);
            break;
          case 1:
            g_target = got.toInt();
            Sprint("g_target: ");
            Sprintln(g_target);
            break;
          case 2:
            b_target = got.toInt();
            Sprint("b_target: ");
            Sprintln(b_target);
            break;
          case 3:
            f = got.toInt();
            Sprint("f: ");
            Sprintln(f);
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

  fade();
}

void fade() {

  int stp = map(r, 1, 255, 1, f);

  if (r_target > r + stp) {
    r = r + stp;
  } else if (r_target <= r - stp) {
    r = r - stp;
  } else {
    r = r_target;
  }

  if (g_target > g + stp) {
    g = g + stp;
  } else if (g_target <= g - stp) {
    g = g - stp;
  } else {
    g = g_target;
  }

  if (b_target > b + stp) {
    b = b + stp;
  } else if (b_target <= b - stp) {
    b = b - stp;
  } else {
    b = b_target;
  }

  Serial.println("Fading in with step -> " + String(stp) + " | r -> " + String(r));

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
