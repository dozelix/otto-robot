"""Helpers compartidos de la suite tests/ (stdlib, Python 3.7+)."""
import os
import re

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO_ROOT, "firmware", "src")
CORE = os.path.join(SRC, "core")
MODULES = os.path.join(SRC, "modules")
TEST_DIR = os.path.join(REPO_ROOT, "firmware", "test", "test_core")


def read(path):
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def parse_int_defines(text):
    """{#define NAME: int} para defines enteros simples (acepta sufijo UL)."""
    out = {}
    for m in re.finditer(r"#define\s+(\w+)\s+(\d+)(?:UL)?\b", text):
        out[m.group(1)] = int(m.group(2))
    return out
