String sdata = ""; // Initialised to nothing.
byte test, start;
String r, g, b, l, d, s;

void setup (void) {
  Serial.begin(115200);
  Serial.println("Command Interpreter");
  pinMode(D4, OUTPUT);
}

void loop(void ) {
  byte ch;
  String valStr;
  int val;

  if (Serial.available()) {
    ch = Serial.read();

    sdata += (char)ch;

    if (ch == '\r') {
      sdata.trim();
      //Serial.println("Data received");

      parseCom(sdata);

      sdata = "";
    }
  }
}

void parseCom(String cnf) {

  String parsed = "";
  int i = 0;
  //Serial.println("Parsing data -> " + cnf);

  do {
    parsed = getValue(cnf, '&', i);
    String param = getValue(parsed, '=', 0);
    String value = getValue(parsed, '=', 1);

    if (param == "d") {
      d = value;
    }

    if (param == "r") {
      r = value;
    }

    if (param == "g") {
      g = value;
    }

    if (param == "b") {
      b = value;
    }

    if (param == "l") {
      l = value;
    }

    if (param == "s") {
      s = value;
    }

    i++;
  } while (getValue(cnf, '&', i) != "");

  lightLed();

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

void lightLed() {
if(s.toInt()){
  
    digitalWrite(D4, s.toInt());


  //Serial.println("OPTIONS -> Duration: " + d + " - RGB: " + r + "," + g + "," + b + " - Brightness:" + l);
}else{
    analogWrite(D4, 0);
}
}
