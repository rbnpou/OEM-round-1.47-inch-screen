#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <DNSServer.h>
#include <Arduino_GFX_Library.h>

#include "LandingConfig.h"
#include "LandingUI.h"
#include "WebUI.h"

// --- CONFIGURACIONES ---
const char* ssid     = "p1n";
const char* password = "clave1234";
const byte  DNS_PORT = 53;
IPAddress   apIP(192,168,4,1);

// --- MATRIZ / HARDWARE ---
#define IMG_W 240
#define IMG_H 240
#define BLACK 0x0000
#define WHITE 0xFFFF

#define TFT_BL   3
#define TFT_DC   2
#define TFT_RST  -1
#define TFT_CS   10
#define TFT_SCLK 6
#define TFT_MOSI 7

// --- OBJETOS GLOBALES ---
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, -1);
Arduino_GC9A01  *gfx = new Arduino_GC9A01(bus, TFT_RST, 0, false, IMG_W, IMG_H);
WebServer  server(80);
DNSServer  dnsServer;

// --- VARIABLES DE ESTADO ---
enum MODO_PANTALLA { NORMAL=0, ASCII, STRESS, COLOR_PULSE };
volatile MODO_PANTALLA modoPantalla = NORMAL;
volatile int  stressMode  = 0;
volatile int  colorIndex  = 0;
volatile bool rotarImagen = false;
volatile bool wifiActivo  = true;

static const uint16_t COLORES_BASE[6] = {0x07FF, 0xF81F, 0xFFE0, 0x001F, 0x87F0, 0x039F};
static const char* COLORES_NOMBRE[6] = {"Rojo","Verde","Azul","Amarillo","Violeta","Naranja"};

int    imgCount = 0;
String fileList[50];
bool   fileEnabled[50];
int    imagenActual   = 0;
unsigned int  tiempoImagenes = 3000;

const char* CONFIG_FILE = "/config.txt";

#define READ_LINES 8
static uint8_t  bigBuffer[IMG_W * 2 * READ_LINES];
static uint16_t lineBuffer[IMG_W];

void configurarServidorRutas();

void setup() {
  Serial.begin(115200); delay(500);
  Serial.println("\n=== ESP32 Animaciones v9.0 ===");
  btStop(); // Liberar memoria RAM del Bluetooth

  pinMode(TFT_BL, OUTPUT); digitalWrite(TFT_BL, HIGH);
  gfx->begin(); gfx->fillScreen(BLACK);
  gfx->setTextColor(WHITE, BLACK); gfx->setTextSize(2);
  gfx->setCursor(15, 80);  gfx->println("ESP32 v9");
  gfx->setCursor(15, 110); gfx->println("Colores!");
  gfx->setCursor(15, 150); gfx->println("Iniciando...");

  if (!LittleFS.begin(true))
{
  while (true) delay(1000);
}

cargarLanding();
cargarEstadoArchivos();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password); delay(1000);
  dnsServer.start(DNS_PORT, "*", apIP);

  configurarServidorRutas();
  server.begin(); wifiActivo = true;

  gfx->fillScreen(BLACK); gfx->setTextSize(1); gfx->setTextColor(WHITE, BLACK);
  gfx->setCursor(10, 40);  gfx->println("===== WiFi ACTIVADO =====");
  gfx->setCursor(10, 80);  gfx->print("SSID: ");  gfx->println(ssid);
  gfx->setCursor(10, 100); gfx->print("Clave: "); gfx->println(password);
  gfx->setCursor(10, 120); gfx->print("IP: ");    gfx->println(WiFi.softAPIP());
  gfx->setCursor(10, 150); gfx->println("Abre navegador:");
  gfx->setTextSize(2); gfx->setCursor(30, 180); gfx->println("192.168.4.1");
  delay(4000);
}

void loop() {
  if (wifiActivo) { dnsServer.processNextRequest(); server.handleClient(); }

  switch (modoPantalla) {
    case ASCII:       displayAsciiArt();   break;
    case STRESS:      displayStress();     break;
    case COLOR_PULSE: displayColorPulse(); break;
    default:          visorAnimaciones();  break;
  }
}