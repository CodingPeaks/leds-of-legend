void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void startwifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
}

void waitcon() {
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
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

  if (r == r_target && g == g_target && b == b_target && flag) {
    Serial.println("Fine");
    flag = false;
  }

  //Serial.println("Fading in with step -> " + String(stp) + " | r -> " + String(r) + " | g -> " + String(g) + " | b -> " + String(b));

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
