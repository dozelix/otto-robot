#pragma once
// Módulo EXTRA net_wifi: solo WiFi STA 2.4 GHz. Bluetooth deshabilitado.
// La base funciona sin este módulo (ENABLE_WIFI_MODULE=0).
// Endpoints:
//   GET /              -> panel (modo + pad manual)
//   GET /status        -> JSON {mode, dist, sim}
//   GET /mode?m=exposition|manual
//   GET /cmd?a=walk|back|turnL|turnR|home|dance|beep&steps=&T=&dir=&freq=&ms=

void wifiBegin();
void wifiHandle();
bool wifiConnected();
const char* wifiIP();
