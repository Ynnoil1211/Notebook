# OUR BRAINS ARE DIAZ O — ICPC Team Notebook (Typst 3-Column Renderer)

Este repositorio contiene el manual competitivo y sistema de generación de notebooks para ICPC del equipo **OUR BRAINS ARE DIAZ O**, diseñado en formato apaisado (*landscape*) de **3 columnas compactas con resaltado de sintaxis nativo (Syntax Highlighting)** para C++17 y Python 3 y suite automatizada de verificación.

---

## 📁 Estructura Modular del Proyecto

```text
├── sections/                         # 13 bloques con fichas Markdown (index.md) y código (.cpp / .py) separado
│   ├── 00_guia/                      # Selector rápido y checklist anti-WA (index.md)
│   ├── 01_cpp_base/                  # Template base C++, RNG, strings, bitmasks, bases (index.md + 5 .cpp)
│   ├── 02_stl_pbds/                  # STL, compresión, custom_hash y PBDS (index.md + 6 .cpp)
│   ├── 03_busquedas_prefix/          # Búsqueda binaria/ternaria, 2P, pilas, prefijos, greedy (index.md + 17 .cpp)
│   ├── 04_estructuras_datos/         # DSU, Sparse Table, Fenwick, SegTree, Treap, HLD, LCT (index.md + 17 .cpp)
│   ├── 05_strings/                   # Duval, Hashing, KMP, Z, Manacher, Trie, AC, SA, SAM, Eertree (index.md + 12 .cpp)
│   ├── 06_grafos/                    # BFS/DFS, Dijkstra, Árboles, SCC, 2-SAT, Flujo, Matching (index.md + 29 .cpp)
│   ├── 07_matematicas/               # Cribas, Pollard-Rho, CRT, Matrices, FFT/NTT, Simplex, BM (index.md + 36 .cpp)
│   ├── 08_dp/                        # Knapsack, LIS, Digit DP, SOS DP, Rerooting, CHT, D&C, Knuth (index.md + 13 .cpp)
│   ├── 09_geometria/                 # Primitivas 2D, Convex Hull, Callipers, Semiplanos, Minkowski (index.md + 10 .cpp)
│   ├── 10_mitm_parsing_clique/       # Meet in the Middle, Shunting-Yard y Max Clique (index.md + 4 .cpp)
│   ├── 11_compendio_teorico/         # Identidades, cotas, combinatoria y teoremas de grafos (index.md)
│   └── 12_python/                    # Kit táctico de Python 3 / PyPy 3 (index.md + 15 .py)
├── Notebook_ICPC_Final.pdf           # Notebook compilado en PDF (3 columnas apaisadas, 37 páginas)
├── notebook_3col.typ                 # Plantilla maestra de Typst (3 columnas, índice con páginas y estilos)
├── build_pdf.py                      # Compilador modular con expansión de directivas -- archivo.cpp/.py
├── tools/
│   └── extract_snippets.py           # Extractor y ensamblador de snippets C++/Python con resolución de dependencias
└── tests/
    └── run_tests.py                  # Suite de pruebas de regresión, diferenciales, compilación y formato
```

---

## 🧩 Cómo Funciona la Separación entre Markdown y Código

Cada carpeta `sections/XX_<bloque>/` contiene:
1. Un único archivo `index.md` con las fichas explicativas del bloque (cuándo usar, contrato, complejidad y trampas).
2. Los archivos `.cpp` o `.py` puros de cada algoritmo (sin bloques ``` de Markdown), listos para abrir con autocompletado/LSP en cualquier IDE.

Para insertar un algoritmo dentro de `index.md`, basta con escribir una línea con `-- <archivo>`:

```markdown
#### 6.6.1 Flujo Máximo: Algoritmo de Dinic (`Dinic`)

- **Cuándo usar:** Flujo máximo y corte mínimo en redes generales...
- **Contrato:** Inicializar `Dinic mf(N + 1)`, agregar aristas con `mf.add_edge(u, v, cap)`...
- **Complejidad:** $\mathcal{O}(V^2 E)$ en redes generales...

-- dinic.cpp

- **Trampas y adaptaciones:** En grafos no dirigidos...
```

Tanto `build_pdf.py` como `tools/extract_snippets.py` y `tests/run_tests.py` expanden automáticamente cada directiva `-- archivo.cpp` / `-- archivo.py` al bloque de código correspondiente en memoria.

---

## 🛠️ Requisitos Previos y Versiones Mínimas

1. **Python $\ge 3.10$** (requerido para `int.bit_count()`, `bisect` con `key=`, y `sys.set_int_max_str_digits` en Python 3.11+).
2. **Compilador C++17 (`g++`)** con soporte para `__int128` y PBDS (`<ext/pb_ds/assoc_container.hpp>`) para ejecutar la suite de pruebas (`tests/run_tests.py`).
3. **Typst** y **Pandoc** (requeridos para generar `Notebook_ICPC_Final.pdf`).

---

## 🚀 Cómo Generar el PDF

Ejecuta desde cualquier directorio:
```bash
python3 build_pdf.py
```

Opciones disponibles:
- **Modo en vivo (`--watch`)**: Vigila cambios tanto en cualquier `index.md` o archivo `.cpp`/`.py` dentro de `sections/` como en `notebook_3col.typ`:
  ```bash
  python3 build_pdf.py --watch
  ```
- **Temas esenciales vs. opcionales (`[opcional]`)**:
  Por defecto se omiten los temas muy avanzados o específicos marcados con `[opcional]` en los `sections/*/index.md` (también puedes pasar `--no-optional` / `--essential`). Para incluir todo el material avanzado usa `--optional` (o `--full`):
  ```bash
  python3 build_pdf.py                      # Solo temas esenciales (Regional)
  python3 build_pdf.py --optional           # Incluye también los temas marcados [opcional]
  ```
- **Compilar solo C++ (`--cpp` o `--lang cpp`) o solo Python (`--py` o `--lang py`)**:
  ```bash
  python3 build_pdf.py --cpp -o Notebook_CPP.pdf
  python3 build_pdf.py --cpp --optional -o Notebook_CPP_Full.pdf
  python3 build_pdf.py --py -o Notebook_Python.pdf
  ```
- **Compilar solo un bloque específico (por ejemplo, solo Grafos)**:
  ```bash
  python3 build_pdf.py --input sections/06_grafos --output grafos.pdf
  ```
- **Solo compilar Typst (`--no-pandoc`, sin requerir Pandoc instalado)**:
  ```bash
  python3 build_pdf.py --no-pandoc
  ```

---

## 🧪 Verificación Automatizada

Ejecutar la suite completa de pruebas (`tests/run_tests.py`):
Compila y ejecuta todas las pruebas de regresión, pruebas diferenciales contra oráculos, compilación estricta de las 164 fichas operativas y auditoría de ancho de línea ($\le 60$ caracteres):
```bash
python3 tests/run_tests.py --strict
```
