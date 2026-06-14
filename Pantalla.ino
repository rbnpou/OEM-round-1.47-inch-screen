uint16_t escalarColor(uint16_t c, uint8_t factor) {
  uint8_t r = ((c >> 11) & 0x1F) * factor / 255;
  uint8_t g = ((c >>  5) & 0x3F) * factor / 255;
  uint8_t b = ( c        & 0x1F) * factor / 255;
  return (r << 11) | (g << 5) | b;
}


void mostrarRAWOptimizado(const String& path) {
  File f = LittleFS.open(path, "r"); if (!f) return;
  for (int y = 0; y < IMG_H; y += READ_LINES) {
    int ln = min(READ_LINES, IMG_H - y);
    if (f.read(bigBuffer, IMG_W * 2 * ln) != (size_t)(IMG_W * 2 * ln)) break;
    for (int l = 0; l < ln; l++) {
      uint8_t* src = bigBuffer + l * IMG_W * 2;
      for (int x = 0; x < IMG_W; x++) lineBuffer[x] = ((uint16_t)src[x * 2] << 8) | src[x * 2 + 1];
      gfx->draw16bitRGBBitmap(0, y + l, lineBuffer, IMG_W, 1);
    }
  }
  f.close();
}

void mostrarASCII(const String& path) {
  gfx->fillScreen(BLACK); gfx->setTextColor(WHITE, BLACK); gfx->setTextSize(1);
  File f = LittleFS.open(path, "r"); if (!f) return;
  int y = 0, x = 0;
  while (f.available() && y < IMG_H) {
    char c = f.read();
    if (c == '\n') { y += 8; x = 0; }
    else if (c != '\r' && x < IMG_W - 7) { gfx->setCursor(x, y); gfx->print(c); x += 6; }
  }
  f.close();
}

void displayColorPulse() {
  static float phase = 0.0f;
  uint16_t base = COLORES_BASE[colorIndex];
  uint8_t factor = (uint8_t)(40 + 215 * (0.5f + 0.5f * sin(phase)));
  uint16_t color = escalarColor(base, factor);
  gfx->fillScreen(color);

  if (wifiActivo) { dnsServer.processNextRequest(); server.handleClient(); }
}

void displayStress() {
  static uint16_t cols[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0x07FF, 0xF81F, 0xFFFF, 0x0000};
  static int col = 0, t = 0;
  if (stressMode == 0) {
    for (int dy = 0; dy < IMG_H; dy += 18)
      for (int dx = 0; dx < IMG_W; dx += 18)
        gfx->fillRect(dx, dy, 18, 18, ((dx / 18 + dy / 18 + t) & 1) ? cols[col] : cols[(col + 2) % 8]);
  } else {
    for (int i = 0; i < 6; i++) gfx->fillRect(i * 40, 0, 40, 240, cols[(col + i) % 8]);
  }
}

void displayAsciiArt() {
  static int t = 0; int m = (t / 110) % 4;
  gfx->fillScreen(m == 2 ? 0x0421 : BLACK);
  if (m == 0) {
    gfx->setTextColor(0xCE59, BLACK); gfx->setTextSize(2);
    for (int i = 0; i < 24; i++) { int y = 40 + 40 * sin((i * 0.5) + t * 0.12); gfx->setCursor(8 + 10 * i, y); gfx->print("#"); }
    gfx->setTextColor(WHITE, BLACK); gfx->setTextSize(2);
    int y0 = 90 + 7 * sin(t * 0.14);
    gfx->setCursor(55, y0);   gfx->println("  /\\_/\\  ");
    gfx->setCursor(55, y0+20); gfx->println(" ( o.o ) ");
    gfx->setCursor(55, y0+40); gfx->println("  > ^ <  ");
    gfx->setTextColor(0x07E0, BLACK); gfx->setCursor(70, 180); gfx->println("ASCII DEMO!");
  } else if (m == 1) {
    gfx->setTextColor(0x07FF, 0x0008); gfx->setTextSize(1);
    for (int x = 0; x < 32; x++) {
      int len = 2 + ((x + t / 10) % 8); int pos = ((t * 4 + x * 11) % (IMG_H + len * 11));
      for (int i = 0; i < len; i++) { int y = pos - i * 11; if (y > 0 && y < IMG_H - 8) { gfx->setCursor(x * 7, y + 2); gfx->print("0123456789ABCDEF"[(x + t + i) % 16]); } }
    }
    gfx->setCursor(70, 210); gfx->setTextColor(0xF8A4, BLACK); gfx->setTextSize(2); gfx->print("MATRIX");
  } else if (m == 2) {
    gfx->setTextSize(2); gfx->setTextColor(0xA7EF, 0x0421);
    String s = "ESP32 ASCII WAVE";
    for (int i = 0; i < (int)s.length(); i++) { int y = 90 + 16 * sin((i + t * 0.14)); gfx->setCursor(15 + 11 * i, y); gfx->print(s[i]); }
    gfx->setTextColor(0x07E0, 0x0421); gfx->setCursor(32, 150); gfx->setTextSize(2); gfx->print("TEST DE PANTALLA");
  } else {
    gfx->setTextColor(0x01EF, BLACK); gfx->setTextSize(1);
    for (int x = 0; x < 32; x++) { int ya = 195 + 14 * sin(x * 0.2 + t * 0.09); gfx->setCursor(x * 7, ya); gfx->print("~"); gfx->setCursor(x * 7, ya - 14); gfx->print("~"); }
    gfx->setTextColor(0xFFE0, BLACK); gfx->setTextSize(2);
    int px = 20 + abs(160 * sin(t * 0.03)), py = 140 + 10 * sin(t * 0.09 + 12); gfx->setCursor(px, py); gfx->print("<o)))><");
    gfx->setTextColor(WHITE, BLACK); gfx->setCursor(50, 30); gfx->setTextSize(2); gfx->println("MAR & PEZ");
  }
}

void visorAnimaciones() {
  loadFileList();
  if (imgCount == 0) {
    gfx->fillScreen(BLACK); gfx->setTextSize(2); gfx->setTextColor(WHITE, BLACK);
    gfx->setCursor(15, 100); gfx->println("Sin archivos");
  }
  int intentos = 0;
  while (!fileEnabled[imagenActual] && intentos < imgCount) { imagenActual = (imagenActual + 1) % imgCount; intentos++; }
  if (intentos >= imgCount) {
    gfx->fillScreen(BLACK); gfx->setTextSize(2); gfx->setTextColor(WHITE, BLACK);
    gfx->setCursor(15, 100); gfx->println("Ninguno habilitado");
  
  }
  String fname = fileList[imagenActual];
  Serial.println("[VISOR] " + fname);
  if (fname.endsWith(".txt")) mostrarASCII("/" + fname);
  else                       mostrarRAWOptimizado("/" + fname);
  
  unsigned long start = millis();
  while (millis() - start < tiempoImagenes) {
    if (wifiActivo) { dnsServer.processNextRequest(); server.handleClient(); }
    if (rotarImagen) { rotarImagen = false; imagenActual = (imagenActual + 1) % imgCount; break; }
    if (modoPantalla != NORMAL) return;
    delay(10);
  }
  if (!rotarImagen) imagenActual = (imagenActual + 1) % imgCount;
}