# OUR BRAINS ARE DIAZ O — ICPC Team Notebook

Notebook de referencia del equipo para ICPC: fichas cortas de C++17 y
Python 3 con código real compilado y verificado, renderizadas a un PDF
apaisado de 3 columnas con Typst.

## La plantilla C++

Todo el código C++ del notebook se apoya en un único archivo:

**[`sections/01_cpp_base/cpp_template_base.cpp`](sections/01_cpp_base/cpp_template_base.cpp)**

Los snippets `.cpp` no son programas completos: se pegan después de la
plantilla, que ya aporta `bits/stdc++.h`, fast I/O y `main`. Por eso ningún
snippet repite `#include`, `using namespace std` ni alias.

La plantilla define:

- Alias: `ll`, `ull`, `ld`, `pii`, `pll`, `vi`, `vll`.
- Macros: `all`, `sz`, `pb`, `fi`, `se`, `el`, `forn`, `for1`.
- Constantes: `INF = 1e18` (sumas en `ll`), `INF_INT = 1e9` (distancias en
  `int`), `EPS` y `PI`.
- Comparación real: `eps_sgn`, `eps_eq`, `eps_lt` y `quantize_eps` para usar
  `ld` como clave de `set`/`map` (ver §1.1).

El contrato completo, con las trampas, está en
[§1.1 de C++ Base](sections/01_cpp_base/index.md).

## Estructura

```text
sections/
  00_guia/                  Selector rápido, complejidades y checklist anti-WA
  01_cpp_base/              Plantilla, RNG, strings, bitmasks y bases (5 .cpp)
  02_stl_pbds/              STL, compresión, custom_hash y PBDS (6 .cpp)
  03_busquedas_prefix/      Búsqueda binaria/ternaria, 2P, pilas, prefijos (17 .cpp)
  04_estructuras_datos/     DSU, Sparse Table, Fenwick, SegTree, Treap, HLD (17 .cpp)
  05_strings/               Duval, hashing, KMP, Z, Manacher, Trie, AC, SA (12 .cpp)
  06_grafos/                BFS/DFS, Dijkstra, SCC, 2-SAT, flujo, matching (29 .cpp)
  07_matematicas/           Cribas, Pollard-Rho, CRT, matrices, FFT/NTT (36 .cpp)
  08_dp/                    Knapsack, LIS, Digit DP, SOS DP, CHT, D&C, Knuth (13 .cpp)
  09_geometria/             Primitivas 2D, convex hull, callipers, semiplanos (10 .cpp)
  10_mitm_parsing_clique/   Meet in the middle, Shunting-Yard y max clique (4 .cpp)
  11_compendio_teorico/     Identidades, cotas y teoremas de grafos
  12_python/                Kit táctico de Python 3 / PyPy 3 (15 .py)
Notebook_ICPC_Final.pdf     PDF compilado del notebook
build_pdf.py                Expande las directivas -- archivo, Pandoc y Typst
notebook_3col.typ           Plantilla Typst de 3 columnas
tools/extract_snippets.py   Extrae y ensambla snippets con sus dependencias
tests/run_tests.py          Compilación, regresión, diferenciales y formato
```

## Cómo se insertan los snippets

Cada bloque tiene un `index.md` con las fichas y los `.cpp`/`.py` sueltos.
Una línea `-- archivo.cpp` dentro del `index.md` inserta el archivo:

```markdown
#### 6.6.1 Flujo Máximo: Dinic (`Dinic`)

- **Cuándo usar:** Flujo máximo y corte mínimo en redes generales.
- **Contrato:** `Dinic mf(N + 1)`, aristas con `mf.add_edge(u, v, cap)`.

-- dinic.cpp

- **Trampas:** aristas no dirigidas se agregan en ambos sentidos.
```

`build_pdf.py`, `tools/extract_snippets.py` y `tests/run_tests.py` expanden
esa directiva en memoria; los `.cpp` se editan como código normal, con LSP.

## Requisitos

- Python 3.10 o superior.
- `g++` con C++17, `__int128` y PBDS (para la suite de pruebas).
- Typst y Pandoc (para el PDF).

## Generar el PDF

```bash
python3 build_pdf.py
```

Opciones útiles:

```bash
python3 build_pdf.py --watch                 # reconstruye al guardar
python3 build_pdf.py --cpp -o Notebook_CPP.pdf
python3 build_pdf.py --py -o Notebook_Python.pdf
python3 build_pdf.py --optional              # incluye temas [opcional]
python3 build_pdf.py -i sections/06_grafos -o grafos.pdf
python3 build_pdf.py --no-pandoc             # sin Pandoc instalado
```

Por defecto se omiten los temas marcados `[opcional]`; `--optional` (o
`--full`) los incluye.

## Pruebas

```bash
python3 tests/run_tests.py --strict
```

Compila las 164 fichas, ejecuta los casos de regresión y los oráculos
diferenciales, y verifica el ancho de línea (≤ 60 caracteres) de los bloques.
