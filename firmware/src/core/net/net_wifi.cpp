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
<!doctype html><html><head><meta charset=utf-8><meta name=viewport content="width=device-width,initial-scale=1">
<title>Otto-bot</title><style>body{font-family:system-ui;margin:16px}button{padding:10px 14px;margin:4px;font-size:16px}a{text-decoration:none}</style></head>
<body><h2>Otto-bot</h2>
<p>Modo: <b id=m>-</b></p>
<p><button onclick="setMode('exposition')">Exposicion (baila+sensor)</button>
<button onclick="setMode('manual')">Manual</button></p>
<h3>Pad manual (solo en manual)</h3>
<p><button onclick="cmd('walk')">Adelante</button>
<button onclick="cmd('back')">Atras</button>
<button onclick="cmd('turnL')">Girar izq</button>
<button onclick="cmd('turnR')">Girar der</button></p>
<p><button onclick="cmd('home')">Home</button>
<button onclick="cmd('dance')">Baile</button>
<button onclick="cmd('beep')">Beep</button></p>
<pre id=s></pre>
<script>
async function refresh(){try{const r=await fetch('/status');const j=await r.json();document.getElementById('m').textContent=j.mode;document.getElementById('s').textContent=JSON.stringify(j,null,2);}catch(e){}setTimeout(refresh,2000);}refresh();
async function setMode(m){await fetch('/mode?m='+m);refresh();}
async function cmd(a){await fetch('/cmd?a='+a+'&steps=2&T=1000');refresh();}
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
