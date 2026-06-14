#pragma once
#include <Arduino.h>

const char* HTML_HEADER = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1.0">
  <title>ESP32 Animaciones</title>
  <style>
    *{margin:0;padding:0;box-sizing:border-box;}
    body{font-family:'Courier New',monospace;background:#0a0e27;color:#e0e6ff;padding:20px;line-height:1.6;}
    .container{max-width:900px;margin:0 auto;}
    h1{color:#00d4ff;text-align:center;margin-bottom:30px;text-shadow:0 0 10px #00d4ff;font-size:2em;}
    .box{background:linear-gradient(135deg,#1a1f3a,#16213e);padding:20px;border-radius:10px;
         border:1px solid #00d4ff;box-shadow:0 0 20px rgba(0,212,255,.3);margin-bottom:20px;}
    .box h2{color:#00d4ff;margin-bottom:15px;font-size:1.2em;border-bottom:2px solid #00d4ff;padding-bottom:10px;}
    .box h3{color:#00d4ff;margin-bottom:10px;font-size:1em;}
    button{background:linear-gradient(135deg,#00d4ff,#0099cc);color:#000;border:none;
           padding:10px 20px;border-radius:5px;cursor:pointer;font-weight:bold;
           margin:5px;transition:all .3s;font-size:.9em;}
    button:hover{transform:scale(1.05);box-shadow:0 0 15px #00d4ff;}
    button.danger{background:linear-gradient(135deg,#ff3333,#cc0000);}
    button.danger:hover{box-shadow:0 0 15px #ff3333;}
    button.full{width:calc(100% - 10px);display:block;}
    .cbtn{color:#fff!important;text-shadow:0 1px 3px #0008;min-width:80px;}
    .cbtn.rojo    {background:linear-gradient(135deg,#ff2222,#aa0000)!important;}
    .cbtn.verde   {background:linear-gradient(135deg,#22dd22,#007700)!important;}
    .cbtn.azul    {background:linear-gradient(135deg,#2244ff,#0011aa)!important;}
    .cbtn.amarillo{background:linear-gradient(135deg,#ffee00,#aa8800)!important;color:#000!important;text-shadow:none!important;}
    .cbtn.violeta {background:linear-gradient(135deg,#aa22ff,#550088)!important;}
    .cbtn.naranja {background:linear-gradient(135deg,#ff8800,#cc4400)!important;}
    .cbtn:hover{transform:scale(1.07);filter:brightness(1.2);}
    input[type=file],input[type=text],input[type=number],textarea{
      background:#0f1419;border:1px solid #00d4ff;padding:8px;border-radius:5px;
      color:#e0e6ff;width:100%;margin:5px 0;font-family:'Courier New',monospace;}
    input[type=file]{cursor:pointer;padding:10px;}
    input:focus,textarea:focus{outline:none;box-shadow:0 0 10px #00d4ff;}
    .file-list{background:#0f1419;border-radius:5px;padding:10px;max-height:260px;overflow-y:auto;}
    .file-list li{list-style:none;padding:8px;border-bottom:1px solid #1a1f3a;
                  display:flex;align-items:center;gap:8px;}
    .file-list input[type=checkbox]{cursor:pointer;width:18px;height:18px;flex-shrink:0;accent-color:#00d4ff;}
    .file-list a{color:#00d4ff;text-decoration:none;flex:1;word-break:break-all;}
    .file-list a:hover{text-decoration:underline;}
    .control-row{display:flex;gap:10px;align-items:center;margin:10px 0;}
    .control-row input{flex:1;}
    .status{background:#0f1419;padding:10px;border-radius:5px;margin:10px 0;
            border-left:3px solid #00d4ff;color:#7fff00;max-height:120px;overflow-y:auto;font-size:.9em;}
    canvas{border:2px solid #00d4ff;border-radius:5px;width:100%;max-width:200px;
           display:block;margin:10px auto;background:#000;}
    .tabs{display:flex;gap:10px;margin-bottom:15px;border-bottom:2px solid #00d4ff;}
    .tab-btn{padding:10px 20px;background:#0f1419;border:none;color:#00d4ff;cursor:pointer;
             border-radius:5px 5px 0 0;font-family:'Courier New',monospace;}
    .tab-btn.active{background:#00d4ff;color:#000;font-weight:bold;}
    .tab-content{display:none;}
    .tab-content.active{display:block;}
    progress{width:100%;height:18px;margin:6px 0;accent-color:#00d4ff;}
    .color-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:8px;margin-bottom:12px;}
    @media(max-width:600px){.color-grid{grid-template-columns:repeat(2,1fr);}}
  </style>
</head>
<body>
<div class="container">
  <h1>&#9654; ESP32 Animaciones v9</h1>
<div class="box">
  <h2>Landing Pública</h2>

  <input type="text"
         id="landingTitulo"
         placeholder="Título">

  <textarea id="landingDescripcion"
            rows="3"
            placeholder="Descripción"></textarea>

  <input type="text"
         id="landingInstagram"
         placeholder="Instagram URL">

  <input type="text"
         id="landingFacebook"
         placeholder="Facebook URL">

  <input type="text"
         id="landingTiktok"
         placeholder="TikTok URL">

  <input type="text"
         id="landingYoutube"
         placeholder="YouTube URL">

  <input type="text"
         id="landingWhatsapp"
         placeholder="WhatsApp URL">

  <input type="text"
         id="landingWebsite"
         placeholder="Sitio web URL">

  <button onclick="guardarLanding()"
          class="full">
    Guardar Landing
  </button>

  <hr style="margin:15px 0;opacity:.3;">

  <h3>Logo</h3>

<img id="logoPreview"
     src="/logo.jpg"
     style="max-width:150px;
            display:block;
            margin:10px auto;
            border-radius:8px;"
     onerror="this.style.display='none';">

  <input type="file"
         id="logoFile"
         accept=".jpg,.jpeg,.png">

  <button onclick="subirLogo()"
          class="full">
    Subir Logo
  </button>

<button onclick="borrarLogo()"
        class="danger full">
  Borrar Logo
</button>

  <div id="landingStatus"
       class="status">
       Landing lista
  </div>
</div>

  <div class="box">
    <h2>Tiempo de Visualizacion</h2>
    <div class="control-row">
      <input type="number" id="tiempoInput" min="3" step="1" value="3000" placeholder="ms">
      <button onclick="cambiarTiempo()">Aplicar</button>
    </div>
    <div style="font-size:.9em;color:#7fff00;">3–60000 ms</div>
  </div>


  <div class="box">
    <h2>Colores Solidos (Pulso)</h2>
    <div style="font-size:.85em;color:#7fff00;margin-bottom:10px;">Toca un color para activarlo:</div>
    <div class="color-grid">
      <button class="cbtn rojo"     onclick="activarColor(0)">Rojo</button>
      <button class="cbtn verde"    onclick="activarColor(1)">Verde</button>
      <button class="cbtn azul"     onclick="activarColor(2)">Azul</button>
      <button class="cbtn amarillo" onclick="activarColor(3)">Amarillo</button>
      <button class="cbtn violeta"  onclick="activarColor(4)">Violeta</button>
      <button class="cbtn naranja"  onclick="activarColor(5)">Naranja</button>
    </div>
    <div id="colorStatus" style="font-size:.85em;color:#7fff00;margin-top:10px;text-align:center;">Ninguno activo</div>
  </div>

  <div class="box">
    <h2>Convertidor de Archivos</h2>
    <div class="tabs">
      <button class="tab-btn active" onclick="switchTab(0)">Imagen RAW</button>
      <button class="tab-btn"        onclick="switchTab(1)">ASCII Frame</button>
    </div>

    <div id="tab0" class="tab-content active">
      <h3>Convertidor RAW (240x240)</h3>
      <input type="file" id="rawFileInput" accept=".jpg,.jpeg,.png,.bmp">
      <input type="text" id="rawFileName" placeholder="nombre (sin extension)" value="imagen">
      <button onclick="convertirYSubirRAW()" class="full">Convertir y Subir</button>
      <b style="display:block;margin-top:12px;">Vista previa:</b>
      <canvas id="previewCanvas"></canvas>
      <div id="rawStatus" class="status">Listo</div>
    </div>

    <div id="tab1" class="tab-content">
      <h3>Pegar Frame ASCII</h3>
      <input type="text" id="asciiFileName" placeholder="nombre (sin extension)" value="frame">
      <textarea id="asciiContent" rows="6" placeholder="Pega aqui tu contenido ASCII"></textarea>
      <button onclick="convertirYSubirASCII()" class="full">Guardar Frame ASCII</button>
      <div id="asciiStatus" class="status">Listo</div>
    </div>
  </div>

  <div class="box">
    <h2>Archivos en el Dispositivo</h2>
    <div style="font-size:.85em;color:#7fff00;margin-bottom:8px;">[IMG]=imagen RAW &nbsp; [ASCII]=texto</div>
    <ul class="file-list">
)rawliteral";

const char* HTML_FOOTER = R"rawliteral(
    </ul>
  </div>
</div>

<script charset="UTF-8">
function activarColor(idx) {
  const nombres = ['Rojo','Verde','Azul','Amarillo','Violeta','Naranja'];
  fetch('/color?idx=' + idx).then(() => {
    document.getElementById('colorStatus').textContent = nombres[idx] + ' — Pulso activo';
  }).catch(console.error);
}
function switchTab(tab) {
  document.querySelectorAll('.tab-content').forEach(t => t.classList.remove('active'));
  document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));
  document.getElementById('tab'+tab).classList.add('active');
  document.querySelectorAll('.tab-btn')[tab].classList.add('active');
}
function toggleFile(cb) {
  fetch('/togglefile?file='+encodeURIComponent(cb.getAttribute('data-file'))+'&enabled='+(cb.checked?'1':'0'))
    .catch(e => { console.error(e); cb.checked = !cb.checked; });
}
function rotar()     { fetch('/next').catch(console.error); }
function modo(x)     { fetch('/mode?m='+x).then(()=>location.reload()).catch(console.error); }
function videoTest(s){ fetch('/stress?sm='+s).then(()=>location.reload()).catch(console.error); }
function cambiarTiempo() {
  const t = document.getElementById('tiempoInput').value;
  if (!t || t < 3) { alert('Minimo 3ms'); return; }
  fetch('/settime?t='+t).then(()=>alert('Tiempo: '+t+'ms')).catch(console.error);
}

function toRGB565neg(r,g,b) {
  let c = ((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3);
  return ((0x1F-((c>>11)&0x1F))<<11)|((0x3F-((c>>5)&0x3F))<<5)|(0x1F-(c&0x1F));
}
async function convertirYSubirRAW() {
  const s = document.getElementById('rawStatus');
  const f = document.getElementById('rawFileInput').files[0];
  let nombre = document.getElementById('rawFileName').value.trim();
  if (!nombre||!f) { s.textContent=(!nombre?'Ingresa un nombre':'Selecciona imagen'); return; }
  nombre += '.raw'; s.textContent = 'Convirtiendo...';
  const img = new Image();
  img.onload = async () => {
    const cv = document.getElementById('previewCanvas'); cv.width=240; cv.height=240;
    const ctx = cv.getContext('2d'); ctx.fillStyle='#000'; ctx.fillRect(0,0,240,240); ctx.drawImage(img,0,0,240,240);
    const px = ctx.getImageData(0,0,240,240).data; const arr = new Uint8Array(240*240*2);
    for (let i=0,j=0;i<240*240;i++) {
      const c=toRGB565neg(px[i*4],px[i*4+1],px[i*4+2]);
      arr[j++]=(c>>8)&0xFF; arr[j++]=c&0xFF;
    }
    s.textContent='Subiendo '+nombre+'...';
    const fd=new FormData(); fd.append('file',new Blob([arr]),nombre);
    try {
      const r=await fetch('/upload',{method:'POST',body:fd});
      if(r.ok){s.textContent='Subido!';setTimeout(()=>location.reload(),1200);} else s.textContent='Error HTTP '+r.status;
    } catch(e){s.textContent='Error: '+e.message;}
  };
  img.onerror=()=>s.textContent='Error al cargar'; img.src=URL.createObjectURL(f);
}
async function convertirYSubirASCII(){
  const s=document.getElementById('asciiStatus'); let n=document.getElementById('asciiFileName').value.trim();
  const c=document.getElementById('asciiContent').value;
  if(!n||!c){s.textContent=(!n?'Ingresa nombre':'Pega contenido');return;}
  n+='.txt';
  const fd=new FormData();fd.append('file',new Blob([c]),n);
  try{const r=await fetch('/upload',{method:'POST',body:fd});if(r.ok){s.textContent='Guardado!';setTimeout(()=>location.reload(),1200);}else s.textContent='Error '+r.status;}
  catch(e){s.textContent='Error: '+e.message;}
}
async function guardarLanding() {

  const fd = new FormData();

  fd.append("titulo",
      document.getElementById("landingTitulo").value);

  fd.append("descripcion",
      document.getElementById("landingDescripcion").value);

  fd.append("instagram",
      document.getElementById("landingInstagram").value);

  fd.append("facebook",
      document.getElementById("landingFacebook").value);

  fd.append("tiktok",
      document.getElementById("landingTiktok").value);

  fd.append("youtube",
      document.getElementById("landingYoutube").value);

  fd.append("whatsapp",
      document.getElementById("landingWhatsapp").value);

  fd.append("website",
      document.getElementById("landingWebsite").value);

  try {

    const r = await fetch("/saveLanding", {
      method:"POST",
      body:fd
    });

    document.getElementById("landingStatus").textContent =
      r.ok ? "Landing guardada"
           : "Error";

  } catch(e) {

    document.getElementById("landingStatus").textContent =
      e.message;
  }
}

async function subirLogo() {

  const f =
    document.getElementById("logoFile").files[0];

  if(!f) return;

  const fd = new FormData();

fd.append("file", f);
  try {

    const r = await fetch("/uploadLogo", {
      method:"POST",
      body:fd
    });

    document.getElementById("landingStatus").textContent =
      r.ok ? "Logo actualizado"
           : "Error";

  } catch(e) {

    document.getElementById("landingStatus").textContent =
      e.message;
  }
}
async function cargarLanding() {

  try {

    const r = await fetch('/landing.json');

    if(!r.ok) return;

    const d = await r.json();

    document.getElementById('landingTitulo').value =
      d.titulo || '';

    document.getElementById('landingDescripcion').value =
      d.descripcion || '';

    document.getElementById('landingInstagram').value =
      d.instagram || '';

    document.getElementById('landingFacebook').value =
      d.facebook || '';

    document.getElementById('landingTiktok').value =
      d.tiktok || '';

    document.getElementById('landingYoutube').value =
      d.youtube || '';

    document.getElementById('landingWhatsapp').value =
      d.whatsapp || '';

    document.getElementById('landingWebsite').value =
      d.website || '';

  } catch(e) {
    console.error(e);
  }
}

window.addEventListener('load', cargarLanding);
window.addEventListener('load', () => {
  cargarLanding();
});

async function borrarLogo() {

  if(!confirm("¿Borrar el logo actual?"))
    return;

  try {

    const r = await fetch("/deleteLogo");

    document.getElementById("landingStatus").textContent =
      r.ok ? "Logo eliminado"
           : "Error al eliminar";

    if(r.ok) {

      const img =
        document.getElementById("logoPreview");

      if(img)
        img.style.display = "none";
    }

  } catch(e) {

    document.getElementById("landingStatus").textContent =
      e.message;
  }
}
</script>
</body>
</html>
)rawliteral";