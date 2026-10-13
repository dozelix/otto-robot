# 02. Planos (Archivos STL para Impresión 3D)

Archivos oficiales conservados con sus nombres originales `OttoDIY_*_V9` en [`../hardware/stl/`](../hardware/stl/FUENTE.md).

| Pieza | Archivo local | Cantidad | Descripción |
| --- | --- | --- | --- |
| Cabeza | [OttoDIY_head_V9.stl](../hardware/stl/OttoDIY_head_V9.stl) | x1 | Cabeza con recortes para los ojos (sensor HC-SR04) y acceso posterior. |
| Cuerpo | [OttoDIY_body_V9.stl](../hardware/stl/OttoDIY_body_V9.stl) | x1 | Torso/Cuerpo base para fijar la placa y los 2 servos de la cadera. |
| Pierna | [OttoDIY_Leg_V9.stl](../hardware/stl/OttoDIY_Leg_V9.stl) | x2 | Pierna izquierda y derecha, simétrica (con hueco para cables). |
| Pie | [OttoDIY_foot_V9.stl](../hardware/stl/OttoDIY_foot_V9.stl) | x2 | Pie izquierdo y derecho, simétrico (aloja el servo del tobillo). |

> **Actualización 2026-10-13:** el `body_V9` recibido mide **69 × 69 × 39 mm** (9 100 triángulos, malla única). El archivo se entrega como **ensamblaje multi-pieza en un único STL** (ver [`FUENTE.md`](../hardware/stl/FUENTE.md)). Si tu laminador lo abre como un bloque, usa “separar objetos” o recupera `head/leg/foot` del historial git.

**Parámetros recomendados de impresión 3D:**

* **Material:** PLA o PETG.
* **Relleno (Infill):** 20%.
* **Altura de capa:** 0.2 mm.
* **Soportes:** No requiere (diseñado para imprimirse sin soportes).

## Fuente oficial

* GitHub: [Open-Hardware-Leaders/OttoDIY `3Dprint`](https://github.com/Open-Hardware-Leaders/OttoDIY/tree/master/3Dprint)
* Thingiverse original: [thing:1568652/files](https://www.thingiverse.com/thing:1568652/files)
* Web: [ottodiy.tech](https://ottodiy.tech) / [ottodiy.com](https://www.ottodiy.com/) — 3D en [Printables @hprobots](https://www.printables.com/@hprobots)
* Trazabilidad local: [hardware/stl/FUENTE.md](../hardware/stl/FUENTE.md)

---

[← Ficha Técnica](./01-ficha-tecnica.md) | [Índice](../README.md) | [Docs](./index.md) | [Siguiente: Lista de Materiales →](./03-lista-materiales-bom.md)
