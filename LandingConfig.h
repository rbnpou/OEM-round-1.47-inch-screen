#pragma once
#include <LittleFS.h>

#define LANDING_FILE "/landing.cfg"

struct LandingConfig {
  String titulo;
  String descripcion;

  String instagram;
  String facebook;
  String tiktok;
  String youtube;
  String whatsapp;
  String website;
};

LandingConfig landing;

void guardarLanding() {
  File f = LittleFS.open(LANDING_FILE, "w");
  if (!f) return;

  f.println("titulo=" + landing.titulo);
  f.println("descripcion=" + landing.descripcion);

  f.println("instagram=" + landing.instagram);
  f.println("facebook=" + landing.facebook);
  f.println("tiktok=" + landing.tiktok);
  f.println("youtube=" + landing.youtube);
  f.println("whatsapp=" + landing.whatsapp);
  f.println("website=" + landing.website);

  f.close();
}

void cargarLanding() {

  landing.titulo      = "Mi Empresa";
  landing.descripcion = "Seguinos en nuestras redes";

  landing.instagram = "";
  landing.facebook  = "";
  landing.tiktok    = "";
  landing.youtube   = "";
  landing.whatsapp  = "";
  landing.website   = "";

  if (!LittleFS.exists(LANDING_FILE))
  {
    guardarLanding();
    return;
  }

  File f = LittleFS.open(LANDING_FILE, "r");
  if (!f) return;

  while (f.available()) {

    String linea = f.readStringUntil('\n');
    linea.trim();

    int eq = linea.indexOf('=');
    if (eq < 0) continue;

    String key = linea.substring(0, eq);
String val = linea.substring(eq + 1);

key.trim();
val.trim();

    if (key == "titulo") landing.titulo = val;
    else if (key == "descripcion") landing.descripcion = val;
    else if (key == "instagram") landing.instagram = val;
    else if (key == "facebook") landing.facebook = val;
    else if (key == "tiktok") landing.tiktok = val;
    else if (key == "youtube") landing.youtube = val;
    else if (key == "whatsapp") landing.whatsapp = val;
    else if (key == "website") landing.website = val;
  }

  f.close();
}