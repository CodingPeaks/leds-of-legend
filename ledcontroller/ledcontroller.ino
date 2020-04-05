#define Sprintln(a) (Serial.println(a))
#define Sprint(a) (Serial.print(a))
//#define Sprintln(a)
//#define Sprint(a)

String sdata = "";
int fade = 10;
int b = 0;
int curb = 0;
int curtime = 0;
int test = 0;
int state = 0;
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
            b = map(got.toInt(), 0, 100, 0, 255);
            Sprint("b: (" + got + "%) ");
            Sprintln(b);
            curtime = micros();
            break;
          case 1:
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
    if (state < b) {
      state++;
    } else if (state > b) {
      state--;
    }
    analogWrite(ledpin, state);
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
