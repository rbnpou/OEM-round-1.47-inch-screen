📺 OEM-round-1.47-inch-screen

> Insignia digital portátil con portal cautivo WiFi, slideshow de imágenes y landing page tipo Linktree — todo en un ESP32 con pantalla circular TFT de 240×240.
---
¿Qué es esto?
ESP32 Animaciones convierte un microcontrolador ESP32 en un punto de acceso WiFi autónomo que:
Muestra un slideshow de imágenes RAW y frames ASCII en una pantalla circular GC9A01
Sirve una landing page tipo Linktree (título, descripción, logo y botones a redes sociales) a cualquier dispositivo que se conecte al AP
Implementa un portal cautivo — el navegador abre la landing automáticamente al conectarse
Expone un panel de administración web para controlar todo desde el celular, sin necesidad de internet
No requiere router, no requiere nube. Todo corre localmente en el ESP32.
---
Demo
```
Dispositivo conectado a WiFi "p1n"
         ↓
Navegador abre automáticamente
         ↓
┌─────────────────────┐
│      [Logo]         │
│   Mi Empresa        │
│  Seguinos en redes  │
│                     │
│  📷 Instagram       │
│  👍 Facebook        │
│  🎵 TikTok          │
│  💬 WhatsApp        │
│  🌐 Sitio Web       │
│                     │
│  ⚙ Administración  │
└─────────────────────┘
```
---
Hardware requerido
Componente	Descripción
ESP32 (cualquier variante con SPI)	Microcontrolador principal
Display TFT circular GC9A01 240×240	Pantalla de visualización
Conexión SPI	Ver pinout abajo
Pinout
Pin display	GPIO ESP32
BL (backlight)	3
DC	2
RST	— (no conectado)
CS	10
SCLK	6
MOSI	7
> Ajustá los pines en `v3.ino` si tu placa usa una distribución diferente.
---
Dependencias
Instalá estas librerías desde el Library Manager de Arduino IDE:
Librería	Fuente
`Arduino_GFX_Library`	Arduino Library Manager
`LittleFS` (ESP32)	Incluida con el core ESP32
`WebServer`	Incluida con el core ESP32
`DNSServer`	Incluida con el core ESP32
Core ESP32: versión 2.x o superior vía Boards Manager.
---
Instalación
Cloná el repositorio:
```bash
   git clone https://github.com/tu-usuario/esp32-animaciones.git
   ```
Abrí `v3.ino` en Arduino IDE (abre todos los archivos `.ino` y `.h` automáticamente).
Configurá tus credenciales WiFi en `v3.ino`:
```cpp
   const char* ssid     = "NombreDeRedWiFi";
   const char* password = "ClaveDeAdmin";
   ```
> Esta es la clave del AP que genera el ESP32 **y** la contraseña del panel `/admin`.
Seleccioná tu placa ESP32 y el puerto correcto.
Cargá el firmware con Upload.
Al iniciar, la pantalla muestra la IP del AP (`192.168.4.1`) durante 4 segundos.
---
Uso
Conectarse
Desde cualquier dispositivo, conectate a la red WiFi configurada (por defecto `p1n`).
El portal cautivo abre la landing automáticamente. Si no abre, navegá a `http://192.168.4.1`.
Panel de administración
Ingresá a `http://192.168.4.1/admin` con:
Usuario: `admin`
Contraseña: la que configuraste en `ssid`/`password`
Desde el panel podés:
Editar título, descripción y URLs de redes sociales de la landing
Subir o borrar el logo (`.jpg`)
Convertir imágenes (JPG/PNG) a formato RAW 565 y subirlas al slideshow
Subir frames ASCII (`.txt`) para mostrar texto en pantalla
Habilitar/deshabilitar archivos del slideshow individualmente
Ajustar el tiempo de visualización por imagen (mínimo 3 ms)
Activar modos especiales: color sólido pulsante, test de stress de pantalla, arte ASCII animado
Rotar manualmente a la siguiente imagen
---
Estructura de archivos
```
esp32-animaciones/
├── v3.ino             # Setup, loop y configuración de hardware
├── LandingConfig.h    # Struct LandingConfig + lectura/escritura en LittleFS
├── LandingUI.h        # Generador de HTML para la landing pública
├── WebUI.h            # HTML + JS del panel de administración
├── Servidor.ino       # Rutas HTTP (WebServer)
├── Archivos.ino       # Gestión de archivos en LittleFS
└── Pantalla.ino       # Modos de visualización en el display TFT
```
Archivos en LittleFS (en el ESP32)
Archivo	Descripción
`/landing.cfg`	Configuración de la landing (clave=valor)
`/config.txt`	Estado habilitado/deshabilitado de cada archivo
`/logo.jpg`	Logo mostrado en la landing y el panel
`/*.raw`	Imágenes convertidas a RGB565 negativo (240×240×2 bytes)
`/*.txt`	Frames de texto ASCII
---
Modos de pantalla
Modo	Descripción
`NORMAL`	Slideshow automático de archivos `.raw` y `.txt`
`COLOR_PULSE`	Pantalla de color sólido con efecto de pulso de brillo
`ASCII`	Animaciones ASCII generativas (gato, Matrix, wave)
`STRESS`	Test de pantalla: tablero de ajedrez o barras de color
---
Formato de imagen RAW
Las imágenes se almacenan como RGB565 big-endian con colores invertidos (negativo):
Resolución fija: 240×240 píxeles
Tamaño por archivo: 240 × 240 × 2 = 115 200 bytes (~112 KB)
La conversión se hace automáticamente desde el panel web (subís una imagen JPG/PNG y el navegador la convierte antes de subirla)
La fórmula de conversión en JS:
```js
function toRGB565neg(r, g, b) {
  let c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
  return ((0x1F - ((c >> 11) & 0x1F)) << 11)
       | ((0x3F - ((c >>  5) & 0x3F)) <<  5)
       | ( 0x1F - ( c        & 0x1F));
}
```
> Si tu pantalla muestra los colores correctamente sin invertir, podés reemplazar `toRGB565neg` por una conversión RGB565 estándar.
---
API de control (endpoints HTTP)
Todos los endpoints marcados con 🔒 requieren autenticación HTTP Basic (`admin` / tu contraseña).
Método	Ruta	Descripción
GET	`/`	Landing pública
GET	`/admin` 🔒	Panel de administración
GET	`/landing.json`	Config de landing en JSON
POST	`/saveLanding` 🔒	Guarda la config de landing
GET	`/logo.jpg`	Sirve el logo
POST	`/uploadLogo` 🔒	Sube un nuevo logo
GET	`/deleteLogo` 🔒	Borra el logo
POST	`/upload` 🔒	Sube un archivo `.raw` o `.txt`
GET	`/download?file=nombre` 🔒	Descarga un archivo
GET	`/delete?file=nombre` 🔒	Borra un archivo
GET	`/togglefile?file=nombre&enabled=1` 🔒	Habilita/deshabilita un archivo
GET	`/next` 🔒	Avanza a la siguiente imagen
GET	`/mode?m=0` 🔒	Cambia el modo (0=NORMAL, 1=ASCII, 2=STRESS, 3=COLOR_PULSE)
GET	`/stress?sm=0` 🔒	Activa modo stress (0=tablero, 1=barras)
GET	`/settime?t=3000` 🔒	Cambia el tiempo de visualización en ms
GET	`/color?idx=0` 🔒	Activa color sólido (0=Rojo…5=Naranja)
---
Limitaciones conocidas
La memoria del ESP32 limita el slideshow a 50 archivos como máximo
El tamaño total de LittleFS depende de la partición configurada en tu placa (típicamente 1–3 MB)
Con imágenes de 112 KB cada una, en 1 MB de LittleFS entran aproximadamente 8 imágenes
La ruta `/landing.json` no requiere autenticación (cualquier cliente conectado al AP puede leerla)
Hay un doble registro de la ruta `/download` en `Servidor.ino` — el primero está vacío y puede ignorarse; la segunda definición es la funcional
---
Licencia
MIT — libre para usar, modificar y distribuir con atribución.
---
Créditos
Desarrollado con Arduino IDE sobre el ecosistema ESP32 + Arduino_GFX_Library.
