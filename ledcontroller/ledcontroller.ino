//#define Sprintln(a) (Serial.println(a))
//#define Sprint(a) (Serial.print(a))
#define Sprintln(a)
#define Sprint(a)

String sdata = ""; // Initialised to nothing.
byte test, start;
int fo;
int s = 0;
int in = 300;
int out = 600;
int b = 255;
int fadeIn, fadeOut;

#define ledpin D3

void setup (void) {
  Serial.begin(115200);
  Sprintln("Command Interpreter");
  pinMode(ledpin, OUTPUT);
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
            s = got.toInt();
            Sprintln("s:" + got);
            break;
          case 1:
            in = got.toInt() * 10;
            Sprint("i:");
            Sprintln(got.toInt() * 10);
            break;
          case 2:
            out = got.toInt() * 10;
            Sprint("o:");
            Sprintln(got.toInt() * 10);
            break;
          case 3:
            b = got.toInt() * 10;
            Sprint("b:");
            Sprintln(got.toInt() * 10);
            break;
        }
        i++;
      } while (getValue(sdata, ';', i) != "");

      fo = lightLed();
      sdata = "";
    }
  }
  if (fo) {

    int timediff = millis() - fo;

    if (s) {
      fadeIn = map(timediff, 0, in, fadeOut, b);
      if (fadeIn > 255) {
        fadeIn = 255;
      }
      analogWrite(ledpin, fadeIn);
    } else {
      fadeOut = map(timediff, 0, out, fadeIn, 0);
      analogWrite(ledpin, fadeOut);
    }

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

int lightLed() {
  analogWrite(ledpin, s * b);
  return millis();
}
