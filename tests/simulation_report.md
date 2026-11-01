# Reporte de Validación Final y Simulación de Competencia (OUR BRAINS ARE DIAZ O)

## 1. Resumen Ejecutivo de Verificación

| Métrica / Criterio del Plan | Resultado | Evidencia |
| :--- | :--- | :--- |
| **Equipo en cabecera y portada** | `OUR BRAINS ARE DIAZ O` | `notebook_3col.typ`, `Notebook_ICPC_Final.md`, `INVENTORY.md`, `README.md` |
| **Fichas operativas individuales (10 partes)** | `164 / 164` (`completo`) | `tools/inventory.py` $\to$ `INVENTORY.md` y `tests/inventory.json` |
| **Secciones teóricas justificadas** | `13 / 13` (`no_aplicable`) | `Bloque 0` (`§0.1–§0.4`), `§7.8` y `Bloque 11` (`§11.1–§11.8`) |
| **Orden estricto (`Cómo leer` $\to$ `Implementación` $\to$ `Cómo llamar`)** | `164 / 164` verificadas | `test_phase4_inventory_and_all_164_fiches` en `tests/run_tests.py` |
| **Ejecución real de ejemplos `Entrada` $\to$ `Salida`** | `162 / 162` ejecutables pasan (`147` C++ + `15` Python) | `test_phase4_inventory_and_all_164_fiches` |
| **Ancho máximo de línea en bloques de código** | `0` líneas $> 60$ caracteres (sobre `820` bloques) | `test_phase7_line_width_and_build_pipeline` |
| **Pruebas de regresión y oráculos diferenciales** | `25 / 25` suites en `PASS` (`0` fallos) | `python -X utf8 tests/run_tests.py --strict` |
| **PDF compilado (`Notebook_ICPC_Final.pdf`)** | `55` páginas A4 apaisadas en 3 columnas (`5.48 MB`) | `python -X utf8 build_pdf.py` |
| **No-destructividad sobre `Notebook_ICPC_Final.md`** | Hash SHA-256 idéntico antes y después de `build_pdf.py` | Preprocesamiento en memoria con archivo temporal |

---

## 2. Simulación de Competencia (6 Problemas Representativos)

Se resolvieron 6 problemas representativos de competencia ICPC utilizando **únicamente** la tabla de decisión del `Bloque 0`, el índice con páginas y las secciones `Cómo leer la entrada` + `Implementación` + `Cómo llamarlo y producir la salida` impresas en el notebook:

### Problema 1 — Conectividad Dinámica Offline con Adición y Borrado de Aristas
- **Ruta de navegación:** `§0.1 Selector Rápido` (*«Conectividad agregando y borrando aristas»*) $\to$ `§4.1.2 Rollback DSU y Conectividad Dinámica Offline (DynCon)`.
- **Contrato verificado:** Nodos `1..N` (1-indexed), ventana de tiempo `0..Q`, `RollbackDSU::unite` sin compresión de caminos, `DynCon::solve()` responde en `ans` cada `add_query(t)`.
- **Resultado:** Compila en el primer intento sin editar una sola línea del bloque `Implementación`; coincide exactamente con el oráculo BFS por consulta (`test_differential_oracles_all_domains`).

### Problema 2 — Consultas de Camino y Subárbol en Árboles con Actualización
- **Ruta de navegación:** `§0.3 Advertencias Críticas` (*«Árboles: cuándo basta LCA / Prefijos, cuándo Euler Tour y cuándo HLD»*) $\to$ `§4.8.1 Heavy-Light Decomposition (HLD)`.
- **Contrato verificado:** Árbol `1..N` (1-indexed), `pos[u]` mapea cada camino pesado a un intervalo contiguo 0-indexed en `IterSegTree` (`§4.4.1`) y el subárbol de `u` a `[pos[u], pos[u] + sz_sub[u] - 1]`.
- **Resultado:** `query_path` y `query_subtree` coinciden al 100% con el recorrido explícito de padres en el árbol (`test_differential_oracles_all_domains`).

### Problema 3 — Subcadenas Distintas, LCS Contiguo y Palíndromos Distintos
- **Ruta de navegación:** `§0.1 Selector Rápido` $\to$ `§5.6.2 Suffix Automaton` y `§5.6.3 Palindromic Tree (Eertree)`.
- **Contrato verificado:** `SuffixAutomaton::count_distinct_substrings()` coincide con $\frac{N(N+1)}{2} - \sum \text{lcp}[i]$ de `SuffixArray` (`§5.6.1`); `longest_common_substring` halla la mayor subcadena contigua en $\mathcal{O}(|T|)$; `PalindromicTree` mantiene `sz(st) - 2` palíndromos distintos.
- **Resultado:** Verificado contra `SuffixArray` y enumeración directa (`test_differential_oracles_all_domains`).

### Problema 4 — Núcleo de Visibilidad y Suma de Minkowski en Geometría 2D
- **Ruta de navegación:** `§0.1 Selector Rápido` $\to$ `§9.3 Intersección de Semiplanos` y `§9.4 Suma de Minkowski de Polígonos Convexos`.
- **Contrato verificado:** `halfplane_intersection` detecta correctamente regiones vacías, franjas degeneradas de área cero y polígonos acotados; `minkowski_sum` soporta conjuntos degenerados de $1$ o $2$ puntos sin acceso fuera de rango.
- **Resultado:** Verificado en `test_reg_halfplane_intersection`, `test_reg_minkowski_single_point` y `test_phase4_inventory_and_all_164_fiches`.

### Problema 5 — Factorización de 64 bits y Sistema Modular No Coprimo
- **Ruta de navegación:** `§0.1 Selector Rápido` $\to$ `§7.2.1 Miller-Rabin`, `§7.2.2 Pollard's Rho` y `§7.3.5 CRT General`.
- **Contrato verificado:** `mul_mod64` usa `u128` (`__uint128_t`) soportando $N \ge 2^{63}$; `crt_general` usa `i128` en la reducción modular evitando overflow antes del módulo `lcm`.
- **Resultado:** Verificado en `test_reg_miller_rabin_u64` y `test_reg_crt_overflow`.

### Problema 6 — Programación Lineal con Restricciones Negativas y D&C DP con Estados Inalcanzables
- **Ruta de navegación:** `§0.1 Selector Rápido` $\to$ `§7.9 Simplex de Dos Fases` y `§8.4.3 Divide & Conquer DP`.
- **Contrato verificado:** `Simplex::solve()` ejecuta Fase 1 con variable artificial cuando algún $b_i < 0$, distingue sistemas infactibles (`-INF_LP`), óptimos negativos y no acotados (`+INF_LP`), y aplica la regla de Bland; `compute_dc` salta `dp_prev[j - 1] >= INF` sin corromper los límites `optl, optr`.
- **Resultado:** Verificado en `test_reg_simplex_feasibility_and_pivot` y `test_reg_dc_dp_unreachable`.

---

## 3. Verificación del Pipeline de Compilación (`build_pdf.py` y `build_pdf.ps1`)

1. **Ejecución estándar:** `python -X utf8 build_pdf.py` genera `body.typ` y `Notebook_ICPC_Final.pdf` (`55` páginas) sin alterar `Notebook_ICPC_Final.md`.
2. **Modo `--no-pandoc` y `--body` personalizado:** `python -X utf8 build_pdf.py --no-pandoc --body body.typ` compila directamente con Typst pasando `--input body=...` sin requerir `pandoc`.
3. **Ejecución desde otro directorio de trabajo:** Invocar `python -X utf8 path/to/build_pdf.py --no-pandoc` desde `$env:TEMP` resuelve automáticamente las rutas relativas respecto al directorio del script.
