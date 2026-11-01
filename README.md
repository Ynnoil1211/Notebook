# OUR BRAINS ARE DIAZ O — ICPC Team Notebook (Typst 3-Column Renderer)

Este repositorio contiene el manual competitivo y sistema de generación de notebooks para ICPC del equipo **OUR BRAINS ARE DIAZ O**, diseñado en formato apaisado (*landscape*) de **3 columnas compactas con resaltado de sintaxis nativo (Syntax Highlighting)** para C++17 y Python 3, fichas operativas de 10 partes y suite automatizada de verificación.

---

## 📁 Estructura del Proyecto

```text
├── Notebook_ICPC_Final.md   # Manual maestro en Markdown con fichas operativas de 10 partes
├── INVENTORY.md             # Inventario maestro de los 164 algoritmos y secciones teóricas
├── notebook_3col.typ        # Plantilla maestra de Typst (3 columnas, índice con páginas y estilos)
├── body.typ                 # Archivo Typst intermedio generado por Pandoc desde el .md
├── build_pdf.py             # Script de compilación no destructivo (soporta --watch, --body, --no-pandoc)
├── build_pdf.ps1            # Wrapper de PowerShell con resolución por $PSScriptRoot
├── tools/
│   ├── extract_snippets.py  # Extractor y ensamblador de snippets C++/Python con resolución de dependencias
│   └── inventory.py         # Verificador de las 10 partes de cada ficha y generador de INVENTORY.md
└── tests/
    ├── run_tests.py         # Suite de pruebas de regresión, diferenciales, inventario y ejemplos I/O
    ├── inventory.json       # Estado estructurado de todas las fichas del notebook
    └── simulation_report.md # Reporte de simulación de competencia y verificación del PDF
```

---

## 🛠️ Requisitos Previos y Versiones Mínimas

1. **Python $\ge 3.10$** (requerido para `int.bit_count()`, `bisect` con `key=`, y `sys.set_int_max_str_digits` en Python 3.11+).
2. **Compilador C++17 (`g++`)** con soporte para `__int128` y PBDS (`<ext/pb_ds/assoc_container.hpp>`) para ejecutar la suite de pruebas (`tests/run_tests.py`).
3. **Typst**:
   ```powershell
   winget install --id Typst.Typst
   ```
4. **Pandoc** (solo requerido cuando no se usa `--no-pandoc`):
   ```powershell
   winget install JohnMacFarlane.Pandoc
   ```

---

## 🚀 Cómo Generar el PDF

### Opción 1: Con Python (Recomendado)
Ejecuta desde cualquier directorio:
```bash
python build_pdf.py
```
> **Nota:** `build_pdf.py` preprocesa las fórmulas matemáticas **en memoria** sin sobrescribir `Notebook_ICPC_Final.md`, compila hacia un archivo temporal y reemplaza `Notebook_ICPC_Final.pdf` de forma atómica.

Opciones disponibles:
- **Modo en vivo (`--watch`)**: Vigila cambios tanto en `Notebook_ICPC_Final.md` (re-ejecutando Pandoc automáticamente) como en `notebook_3col.typ`:
  ```bash
  python build_pdf.py --watch
  ```
- **Solo compilar Typst (`--no-pandoc`, sin requerir Pandoc instalado)**:
  ```bash
  python build_pdf.py --no-pandoc
  ```
- **Usar un archivo intermedio `--body` personalizado**:
  ```bash
  python build_pdf.py --body mi_cuerpo.typ --output mi_notebook.pdf
  ```
- **Compilar otro archivo Markdown**:
  ```bash
  python build_pdf.py --input otro_archivo.md --output mi_notebook.pdf
  ```

### Opción 2: Con PowerShell
Puedes invocar `build_pdf.ps1` desde cualquier directorio de trabajo:
```powershell
.\build_pdf.ps1
.\build_pdf.ps1 -NoPandoc
.\build_pdf.ps1 -Watch
```

---

## 🧪 Verificación Automatizada e Inventario

1. **Generar y auditar el inventario de fichas (`INVENTORY.md` y `tests/inventory.json`)**:
   ```bash
   python -X utf8 tools/inventory.py
   ```
2. **Ejecutar la suite completa de pruebas (`tests/run_tests.py`)**:
   Compila y ejecuta todas las pruebas de regresión, pruebas diferenciales contra oráculos, validación de las 164 fichas operativas (`Cómo leer la entrada` + `Implementación` + `Cómo llamarlo y producir la salida` contra `Entrada` y `Salida`) y auditoría de ancho de línea ($\le 60$ caracteres):
   ```bash
   python -X utf8 tests/run_tests.py --strict
   ```

---

## 🎨 Estándar de Ficha Operativa (10 Partes)

Cada algoritmo individual del notebook incluye, en este orden exacto:
1. **Cuándo usar**
2. **Cuándo no usar / alternativa**
3. **Contrato y dependencias** (indexación `0-based` vs `1-based`, mutaciones y precondiciones)
4. **Complejidad** de la implementación impresa
5. **Formato de entrada**
6. **Entrada** y **Salida** de ejemplo literales (verificadas automáticamente)
7. **Cómo leer la entrada** (**antes** de la implementación)
8. **Implementación** (líneas de a lo sumo $60$ caracteres para no truncarse en 3 columnas)
9. **Cómo llamarlo y producir la salida** (**después** de la implementación)
10. **Trampas y adaptaciones**
