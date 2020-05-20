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

void meteorRain(byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay) {

  if (mrain > NUMPIXELS + NUMPIXELS) {
    mrain = 0;
  } else {
    mrain++;
  }

  // fade brightness all LEDs one step
  for (int j = 0; j < NUMPIXELS; j++) {
    if ( (!meteorRandomDecay) || (random(10) > 5) ) {
      fadeToBlack(j, meteorTrailDecay );
    }
  }

  // draw meteor
  for (int j = 0; j < meteorSize; j++) {
    if ( ( mrain - j < NUMPIXELS) && (mrain - j >= 0) ) {
      pixels.setPixelColor(mrain - j, rhtt, ghtt, bhtt);
    }
  }

  pixels.show();

}

void fadeToBlack(int ledNo, byte fadeValue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = pixels.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  pixels.setPixelColor(ledNo, r, g, b);
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[ledNo].fadeToBlackBy( fadeValue );
#endif
}
