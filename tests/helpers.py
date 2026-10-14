"""Helpers compartidos de la suite tests/ (stdlib, Python 3.7+)."""
import os
import re

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO_ROOT, "firmware", "src")
CORE = os.path.join(SRC, "core")
MODULES = os.path.join(SRC, "modules")
TOOLS = os.path.join(SRC, "tools")
TEST_DIR = os.path.join(REPO_ROOT, "firmware", "test", "test_core")

# Rutas canónicas (un solo lugar: si core/ se reorganiza, se tocan aquí).
PIO_INI = os.path.join(REPO_ROOT, "platformio.ini")
CONFIG_H = os.path.join(SRC, "config.h")
WIFI_SECRETS_H = os.path.join(SRC, "wifi_secrets.h")
MODES_H = os.path.join(CORE, "modes", "modes.h")
VALIDATE_H = os.path.join(CORE, "validate", "validate.h")
SONAR_H = os.path.join(CORE, "sonar", "sonar.h")
SONAR_CPP = os.path.join(CORE, "sonar", "sonar.cpp")
SONAR_FILTER_H = os.path.join(CORE, "sonar", "sonar_filter.h")
GAIT_PARAMS_H = os.path.join(CORE, "motion", "gait_params.h")
GAIT_H = os.path.join(CORE, "motion", "gait.h")
GAIT_CPP = os.path.join(CORE, "motion", "gait.cpp")
AUTONOMY_H = os.path.join(CORE, "autonomy", "autonomy.h")
AUTONOMY_CPP = os.path.join(CORE, "autonomy", "autonomy.cpp")
NET_WIFI_H = os.path.join(CORE, "net", "net_wifi.h")
NET_WIFI_CPP = os.path.join(CORE, "net", "net_wifi.cpp")


def read(path):
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def parse_int_defines(text):
    """{#define NAME: int} para defines enteros simples (acepta sufijo UL)."""
    out = {}
    for m in re.finditer(r"#define\s+(\w+)\s+(\d+)(?:UL)?\b", text):
        out[m.group(1)] = int(m.group(2))
    return out
