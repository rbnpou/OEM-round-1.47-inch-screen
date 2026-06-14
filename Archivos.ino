void guardarConfiguracion() {
  File f = LittleFS.open(CONFIG_FILE, "w");
  if (!f) return;
  for (int i = 0; i < imgCount; i++) {
    f.print(fileList[i]); f.print("="); f.println(fileEnabled[i] ? "1" : "0");
  }
  f.close();
}

void cargarConfiguracion() {
  if (!LittleFS.exists(CONFIG_FILE)) return;
  File f = LittleFS.open(CONFIG_FILE, "r");
  if (!f) return;
  while (f.available()) {
    String linea = f.readStringUntil('\n'); linea.trim();
    if (!linea.length()) continue;
    int eq = linea.indexOf('=');
    if (eq == -1) continue;
    String arch   = linea.substring(0, eq);
    String estado = linea.substring(eq + 1); estado.trim();
    for (int i = 0; i < imgCount; i++) {
      if (fileList[i] == arch) { fileEnabled[i] = (estado == "1"); break; }
    }
  }
  f.close();
}

void cargarEstadoArchivos() {
  imgCount = 0;
  File root = LittleFS.open("/");
  File e    = root.openNextFile();
  while (e && imgCount < 50) {
    String fn = e.name();
    if ((fn.endsWith(".raw") || fn.endsWith(".txt")) && fn != "config.txt") {
      fileList[imgCount]    = fn;
      fileEnabled[imgCount] = true;
      imgCount++;
    }
    e = root.openNextFile();
  }
  root.close();
  cargarConfiguracion();
}

void loadFileList() {
  imgCount = 0;
  File root = LittleFS.open("/"); File e = root.openNextFile();
  while (e && imgCount < 50) {
    String fn = e.name();
    if ((fn.endsWith(".raw") || fn.endsWith(".txt")) && fn != "config.txt") {
      fileList[imgCount] = fn; fileEnabled[imgCount] = true; imgCount++;
    }
    e = root.openNextFile();
  }
  root.close(); cargarConfiguracion();
  if (imagenActual >= imgCount) imagenActual = 0;
}