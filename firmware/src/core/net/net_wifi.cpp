#include "net_wifi.h"
#include "../../config.h"

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "../autonomy/autonomy.h"
#include "../modes/modes.h"
#include "../../wifi_secrets.h"
#include "esp_bt.h"

static WebServer server(80);
static bool ap_started = false;
static String ipStr = "";

static void disableBluetooth() {
  // Política WiFi único: BT/BLE siempre apagado (comparten 2.4 GHz).
  // En Arduino-ESP32 el BT no arranca solo, pero lo liberamos igual.
  esp_err_t e1 = esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
  Serial.printf("[WiFi] BT mem_release: %d (OK si 0/ya liberado)\n", (int)e1);
  // btStop() existe en Arduino-ESP32 cuando BT está compilado:
#if defined(CONFIG_BT_ENABLED) || defined(CONFIG_BLUEDROID_ENABLED)
  btStop();
  Serial.println("[WiFi] btStop() OK");
#endif
}

static String statusJson() {
  char buf[160];
  snprintf(buf, sizeof(buf),
           "{\"mode\":\"%s\",\"dist\":%.1f,\"sim\":%d,\"ip\":\"%s\"}",
           modeName(getMode()), lastDistanceCm(), SIM_SIN_SERVOS, ipStr.c_str());
  return String(buf);
}

static const char HTML_PAGE[] PROGMEM = R"HTML(
<!doctype html><html lang="es"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<title>Otto-bot</title>
<style>
*{box-sizing:border-box}
html{-webkit-text-size-adjust:100%}
body{font-family:system-ui,sans-serif;margin:0 auto;padding:12px;max-width:520px;
touch-action:manipulation;background:#101418;color:#e8ecef}
h1{font-size:1.25rem;margin:.3em 0}
.card{background:#1a2129;border:1px solid #2c3640;border-radius:14px;padding:12px;margin:10px 0}
.row{display:flex;gap:8px}
button{flex:1;min-height:54px;font-size:1.05rem;border-radius:12px;border:1px solid #3a4653;
background:#232c36;color:#e8ecef;cursor:pointer}
button:active{transform:scale(.97)}
button.on{background:#2e7d32;border-color:#2e7d32;color:#fff}
button:disabled{opacity:.35}
.badge{display:inline-block;padding:2px 10px;border-radius:999px;background:#2e7d32;color:#fff;font-size:.85rem}
.badge.manual{background:#b26a00}
.dist{font-size:2rem;font-weight:700;margin:.1em 0}
.small{font-size:.85rem;color:#9aa7b2}
.pad{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px;margin:8px 0}
.pad button{min-height:60px;font-size:1.4rem}
.msg{min-height:1.4em;font-size:.9rem;color:#9aa7b2}
</style></head>
<body>
<h1>&#129302; Otto-bot</h1>
<div class="card">
<div class="row">
<button id="bExpo" onclick="setMode('exposition')">Exposici&oacute;n</button>
<button id="bMan" onclick="setMode('manual')">Manual</button>
</div>
<p class="msg">Exposici&oacute;n: baila solo + evita obst&aacute;culos. Manual: mando desde aqu&iacute;.</p>
</div>
<div class="card">
<span id="mode" class="badge">...</span>
<div class="dist"><span id="dist">--</span> cm</div>
<div class="small" id="ip"></div>
</div>
<div class="card" id="padCard">
<div class="pad">
<span></span><button onclick="cmd('walk')">&#9650;</button><span></span>
<button onclick="cmd('turnL')">&#9664;</button><button onclick="cmd('home')">&#8962;</button><button onclick="cmd('turnR')">&#9654;</button>
<span></span><button onclick="cmd('back')">&#9660;</button><span></span>
</div>
<div class="row">
<button onclick="cmd('dance')">Baile</button>
<button onclick="cmd('beep')">Beep</button>
</div>
<p class="msg" id="msg"></p>
</div>
<script>
let mode='exposition';
async function refresh(){
 try{
  const r=await fetch('/status');const j=await r.json();mode=j.mode;
  const mb=document.getElementById('mode');
  mb.textContent=j.mode;mb.className='badge'+(j.mode==='manual'?' manual':'');
  document.getElementById('dist').textContent=(j.dist<0?'--':j.dist.toFixed(1));
  document.getElementById('ip').textContent=(j.ip||'')+(j.sim?' \u00b7 SIM sin servos':'');
  document.getElementById('bExpo').className=j.mode==='manual'?'':'on';
  document.getElementById('bMan').className=j.mode==='manual'?'on':'';
  const dis=j.mode!=='manual';
  document.querySelectorAll('#padCard button').forEach(b=>b.disabled=dis);
 }catch(e){}
 setTimeout(refresh,2000);
}
refresh();
async function setMode(m){await fetch('/mode?m='+m);refresh();}
async function cmd(a){
 const m=document.getElementById('msg');m.textContent='';
 try{const r=await fetch('/cmd?a='+a+'&steps=2&T=1000');
  if(!r.ok){m.textContent='Pasa a modo Manual primero';return;}
 }catch(e){m.textContent='Sin conexion';}
 refresh();
}
</script></body></html>)HTML";

static void handleRoot() { server.send(200, "text/html", HTML_PAGE); }
static void handleStatus() { server.send(200, "application/json", statusJson()); }

static void handleMode() {
  String m = server.arg("m");
  if (m.length() == 0) {
    server.send(400, "text/plain", "usa /mode?m=exposition|manual");
    return;
  }
  OttoMode nm = modeFromString(m.c_str());
  setMode(nm);
  server.send(200, "application/json", statusJson());
}

static void handleCmd() {
  if (getMode() != OttoMode::MANUAL) {
    server.send(409, "text/plain", "cambia a modo manual primero (/mode?m=manual)");
    return;
  }
  String a = server.arg("a");
  int steps = server.arg("steps").length() ? server.arg("steps").toInt() : 2;
  int T = server.arg("T").length() ? server.arg("T").toInt() : 1000;
  bool ok = false;
  if (a == "walk") ok = manualWalk(steps, T, 1);
  else if (a == "back") ok = manualWalk(steps, T, -1);
  else if (a == "turnR") ok = manualTurn(steps, T, 1);
  else if (a == "turnL") ok = manualTurn(steps, T, -1);
  else if (a == "home") ok = manualHome();
  else if (a == "dance") ok = manualDance();
  else if (a == "beep") {
    int f = server.arg("freq").length() ? server.arg("freq").toInt() : 2000;
    int ms = server.arg("ms").length() ? server.arg("ms").toInt() : 120;
    ok = manualBeep(f, ms);
  } else {
    server.send(400, "text/plain", "a=walk|back|turnL|turnR|home|dance|beep");
    return;
  }
  if (!ok) server.send(400, "text/plain", "comando rechazado (limites o modo)");
  else server.send(200, "application/json", statusJson());
}

void wifiBegin() {
  disableBluetooth();
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(WIFI_HOSTNAME);
  WiFi.setAutoReconnect(true);
  Serial.printf("[WiFi] Conectando a '%s' (solo 2.4 GHz)...\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < WIFI_TIMEOUT_MS) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    ipStr = WiFi.localIP().toString();
    Serial.printf("[WiFi] OK ip=%s\n", ipStr.c_str());
  } else {
    // Degradación elegante: la base sigue en EXPOSITION sin red.
    Serial.println("[WiFi] FAIL: sigo en base sin red (EXPOSITION local)");
    ipStr = "";
    return;
  }
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/mode", handleMode);
  server.on("/cmd", handleCmd);
  server.begin();
  ap_started = true;
  Serial.println("[WiFi] HTTP :80 / /status /mode /cmd listos");
}

void wifiHandle() {
  if (ap_started) server.handleClient();
}

bool wifiConnected() { return WiFi.status() == WL_CONNECTED; }
const char* wifiIP() { return ipStr.c_str(); }
