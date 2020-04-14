uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow() {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel((i + rainbowCycles) & 255));
  }
  pixels.show();
  rainbowCycles++;
  if (rainbowCycles >= 256) rainbowCycles = 0;
}
