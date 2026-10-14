#pragma once
// CORE net_wifi: WiFi STA 2.4 GHz + dashboard, SIEMPRE presente.
// El robot siempre arranca con WiFi (independiente de servos/sonar).
// Bluetooth siempre deshabilitado (comparten 2.4 GHz).
// Endpoints:
//   GET /              -> panel (modo + pad manual)
//   GET /status        -> JSON {mode, dist, sim}
//   GET /mode?m=exposition|manual
//   GET /cmd?a=walk|back|turnL|turnR|home|dance|beep&steps=&T=&dir=&freq=&ms=

void wifiBegin();
void wifiHandle();
bool wifiConnected();
const char* wifiIP();
