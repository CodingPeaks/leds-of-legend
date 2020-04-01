//#define Sprintln(a) (Serial.println(a))
//#define Sprint(a) (Serial.print(a))
#define Sprintln(a)
#define Sprint(a)

String sdata = ""; // Initialised to nothing.
byte test, start;
int fo;
int s = 0;
int d = 600;
int b = 255;

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
            d = got.toInt() * 10;
            Sprint("d:");
            Sprintln(got.toInt() * 10);
            break;
          case 2:
            b = got.toInt() * 10;
            Sprint("b:");
            Sprintln(got.toInt() * 10);
            break;
        }
        i++;
      } while (getValue(sdata, ';', i) != "");

      fo = lightLed(s, d, b);
      sdata = "";
    }
  }
  if (fo) {
    int timediff = millis() - fo;
    int fade = map(timediff, 0, d, b, 0);
    Serial.println(fade);
    analogWrite(ledpin, fade);
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

int lightLed(int s, int d, int b) {
  analogWrite(ledpin, s * b);
  if (!s) {
    return millis();
  } else {
    return 0;
  }
}
