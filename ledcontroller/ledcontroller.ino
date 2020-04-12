#include <Adafruit_NeoPixel.h>

#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)

String sdata = "";
float r = 0;
float g = 0;
float b = 0;
float r_target = 0;
float g_target = 0;
float b_target = 0;
float f = 30;
bool flag = true;

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

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color((int) r, (int) g, (int) b));
  }
  pixels.show();

  fade();
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



void fade() {

  float stp = mapfloat(max(r, max(g, b)), 0.0, 255.0, 0.01, f);

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

  if(r == r_target && g == g_target && b == b_target && flag){
    Serial.println("Fine");
    flag = false;
  }

  //Serial.println("Fading in with step -> " + String(stp) + " | r -> " + String(r) + " | g -> " + String(g) + " | b -> " + String(b));

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
