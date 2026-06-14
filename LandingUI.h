#pragma once

String landingButton(const String& texto, const String& url)
{
  return "<a class='btn' target='_blank' href='" + url + "'>" + texto + "</a>";
}

String htmlLanding()
{
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0">

<title>Portal</title>
<link rel="icon" href="/logo.jpg">
<style>

*{
  margin:0;
  padding:0;
  box-sizing:border-box;
}

body{
  background:#0a0e27;
  color:#e0e6ff;
  font-family:'Courier New',monospace;
  min-height:100vh;
  display:flex;
  justify-content:center;
  align-items:center;
  padding:20px;
}

.card{
  width:100%;
  max-width:500px;
  background:linear-gradient(135deg,#1a1f3a,#16213e);
  border:1px solid #00d4ff;
  border-radius:16px;
  padding:25px;
  box-shadow:0 0 25px rgba(0,212,255,.25);
}

.logo{
  text-align:center;
  margin-bottom:20px;
}

.logo img{
  max-width:180px;
  max-height:180px;
  border-radius:12px;
}

h1{
  text-align:center;
  color:#00d4ff;
  margin-bottom:10px;
}

.desc{
  text-align:center;
  margin-bottom:25px;
  color:#d0d8ff;
}

.btn{
  display:block;
  width:100%;
  text-decoration:none;
  text-align:center;

  background:linear-gradient(135deg,#00d4ff,#0099cc);

  color:#000;
  font-weight:bold;

  padding:14px;
  border-radius:8px;

  margin-bottom:10px;

  transition:.25s;
}

.btn:hover{
  transform:scale(1.03);
}

.admin{
  margin-top:20px;
  text-align:center;
}

.admin a{
  color:#00d4ff;
  text-decoration:none;
  font-size:.9em;
}

</style>
</head>
<body>

<div class='card'>
)rawliteral";

  if (LittleFS.exists("/logo.jpg"))
  {
    html += "<div class='logo'><img src='/logo.jpg'></div>";
  }

  String titulo = landing.titulo;

if (!titulo.length())
  titulo = "Bienvenido";

html += "<h1>" + titulo + "</h1>";

  if (landing.descripcion.length())
  {
    html += "<div class='desc'>" + landing.descripcion + "</div>";
  }

  if (landing.instagram.length())
    html += landingButton("📷 Instagram", landing.instagram);

  if (landing.facebook.length())
    html += landingButton("👍 Facebook", landing.facebook);

  if (landing.tiktok.length())
    html += landingButton("🎵 TikTok", landing.tiktok);

  if (landing.youtube.length())
    html += landingButton("▶ YouTube", landing.youtube);

  if (landing.whatsapp.length())
    html += landingButton("💬 WhatsApp", landing.whatsapp);

  if (landing.website.length())
    html += landingButton("🌐 Sitio Web", landing.website);

  html += R"rawliteral(

<div class="admin">
  <a href="/admin">⚙ Administración</a>
</div>

</div>
</body>
</html>
)rawliteral";

  return html;
}