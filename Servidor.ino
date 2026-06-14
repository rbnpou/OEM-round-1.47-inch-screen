bool authOK() {
  if (!server.authenticate("admin", password)) {
    server.requestAuthentication();
    return false;
  }
  return true;
}

String listFiles() {
  String html = "";
  for (int i = 0; i < imgCount; i++) {
    String name = fileList[i];
    String checked = fileEnabled[i] ? "checked" : "";
    String badge = "";

    if (name.endsWith(".raw")) badge = "<span style='color:#7fff00;font-size:.75em;margin-left:6px;'>[IMG]</span>";
    if (name.endsWith(".txt")) badge = "<span style='color:#00d4ff;font-size:.75em;margin-left:6px;'>[ASCII]</span>";

    String displayName = name;
    int dotIndex = displayName.lastIndexOf('.');
    if (dotIndex > 0) displayName = displayName.substring(0, dotIndex);

    html += "<li>";
    html += "<input type='checkbox' data-file='" + name + "' " + checked + " onchange='toggleFile(this);'>";
    html += "<a href='/download?file=" + name + "'>" + displayName + "</a>" + badge;
    html += " <a href='/delete?file=" + name + "' onclick='return confirm(\"Borrar?\");' style='color:red;margin-left:8px;'>[X]</a>";
    html += "</li>";
  }
  return html;
}

String htmlIndex() {
  String html = String(HTML_HEADER);
  html += listFiles();
  html += String(HTML_FOOTER);
  return html;
}

String htmlDeleted(String name) {
  return "<!DOCTYPE html><html><head><meta charset='UTF-8'></head><body><h2>Borrado: " + name + "</h2><a href='/admin'>Volver</a></body></html>";
}

void configurarServidorRutas() {

server.on("/", HTTP_GET, []() {

    server.sendHeader("Content-Type",
                      "text/html; charset=utf-8");

    server.send(200,
                "text/html; charset=utf-8",
                htmlLanding());
});

  server.on("/admin", HTTP_GET, []() {
    if (!authOK()) return;
    server.send(200, "text/html; charset=utf-8", htmlIndex());
  });

  server.on("/landing.json", HTTP_GET, []() {
    String json = "{";
    json += "\"titulo\":\"" + landing.titulo + "\",";
    json += "\"descripcion\":\"" + landing.descripcion + "\",";
    json += "\"instagram\":\"" + landing.instagram + "\",";
    json += "\"facebook\":\"" + landing.facebook + "\",";
    json += "\"tiktok\":\"" + landing.tiktok + "\",";
    json += "\"youtube\":\"" + landing.youtube + "\",";
    json += "\"whatsapp\":\"" + landing.whatsapp + "\",";
    json += "\"website\":\"" + landing.website + "\"}";
    server.send(200, "application/json", json);
  });

  server.on("/saveLanding", HTTP_POST, []() {
    if (!authOK()) return;

    landing.titulo = server.arg("titulo");
    landing.descripcion = server.arg("descripcion");
    landing.instagram = server.arg("instagram");
    landing.facebook = server.arg("facebook");
    landing.tiktok = server.arg("tiktok");
    landing.youtube = server.arg("youtube");
    landing.whatsapp = server.arg("whatsapp");
    landing.website = server.arg("website");

    guardarLanding();
    server.send(200, "text/plain", "OK");
  });

  server.on("/logo.jpg", HTTP_GET, []() {
    File f = LittleFS.open("/logo.jpg", "r");
    if (!f) {
      server.send(404, "text/plain", "No logo");
      return;
    }
    server.streamFile(f, "image/jpeg");
    f.close();
  });

// =====================================================
// SUBIR LOGO
// =====================================================

server.on(
  "/uploadLogo",
  HTTP_POST,

  []() {
    server.send(200,
                "text/plain",
                "OK");
  },

  []() {

    HTTPUpload& up = server.upload();

    static File uf;

    if (up.status == UPLOAD_FILE_START) {

      if (LittleFS.exists("/logo.jpg"))
        LittleFS.remove("/logo.jpg");

      uf = LittleFS.open("/logo.jpg", "w");
    }

    else if (up.status == UPLOAD_FILE_WRITE) {

      if (uf)
        uf.write(up.buf,
                 up.currentSize);
    }

    else if (up.status == UPLOAD_FILE_END) {

      if (uf)
        uf.close();
    }
  }
);

// =====================================================
// BORRAR LOGO
// =====================================================

server.on("/deleteLogo", HTTP_GET, []() {

  if (LittleFS.exists("/logo.jpg"))
    LittleFS.remove("/logo.jpg");

  server.send(200,
              "text/plain",
              "OK");
});

// =====================================================
// DESCARGAR ARCHIVO
// =====================================================

server.on("/download", HTTP_GET, []() {

});

  server.on("/download", HTTP_GET, []() {
    if (!authOK()) return;
    if (!server.hasArg("file")) return;
    File f = LittleFS.open("/" + server.arg("file"), "r");
    if (!f) return;
    server.streamFile(f, "application/octet-stream");
    f.close();
  });

  server.on("/delete", HTTP_GET, []() {
    if (!authOK()) return;
    String fn = server.arg("file");
    LittleFS.remove("/" + fn);
    loadFileList();
    server.send(200, "text/html", htmlDeleted(fn));
  });

  server.on("/togglefile", HTTP_GET, []() {
    if (!authOK()) return;
    String fn = server.arg("file");
    bool en = server.arg("enabled") == "1";
    for (int i=0;i<imgCount;i++) if (fileList[i]==fn) fileEnabled[i]=en;
    guardarConfiguracion();
    server.send(200,"text/plain","OK");
  });

  server.on("/upload", HTTP_POST, []() {
    if (!authOK()) return;
    loadFileList();
    server.send(200,"text/plain","OK");
  });

  server.on("/next", HTTP_GET, []() { if (!authOK()) return; rotarImagen=true; server.send(200,"text/plain","OK"); });
  server.on("/mode", HTTP_GET, []() { if (!authOK()) return; modoPantalla=(MODO_PANTALLA)server.arg("m").toInt(); server.send(200,"text/plain","OK"); });
  server.on("/stress", HTTP_GET, []() { if (!authOK()) return; stressMode=server.arg("sm").toInt(); modoPantalla=STRESS; server.send(200,"text/plain","OK"); });
  server.on("/settime", HTTP_GET, []() { if (!authOK()) return; tiempoImagenes=server.arg("t").toInt(); server.send(200,"text/plain","OK"); });
  server.on("/color", HTTP_GET, []() { if (!authOK()) return; colorIndex=server.arg("idx").toInt(); modoPantalla=COLOR_PULSE; server.send(200,"text/plain","OK"); });

  server.onNotFound([]() {
    server.send(200, "text/html; charset=utf-8", htmlLanding());
  });
}
