# ICPC Team Notebook — Typst 3-Column Renderer

Este repositorio contiene el sistema de generación de notebooks para ICPC en formato apaisado (*landscape*), diseñado en **3 columnas compactas con resaltado de sintaxis nativo (Syntax Highlighting)** para código C++ y Python.

---

## 📁 Estructura del Proyecto

```text
├── Notebook_ICPC_Final.md   # Tu manual maestro en Markdown puro
├── notebook_3col.typ        # Plantilla maestra de Typst (configuración de 3 columnas y estilos)
├── body.typ                 # Archivo generado automáticamente por Pandoc desde el .md
├── build_pdf.py             # Script en Python para compilar el PDF de forma automática
├── build_pdf.ps1            # Acceso directo para PowerShell
└── Notebook_ICPC_Final.pdf  # PDF final renderizado listo para imprimir
```

---

## 🛠️ Requisitos Previos

Solo necesitas tener instaladas estas dos herramientas en tu sistema:

1. **Typst**:
   ```powershell
   winget install --id Typst.Typst
   ```
2. **Pandoc**:
   ```powershell
   winget install JohnMacFarlane.Pandoc
   ```

---

## 🚀 Cómo Generar el PDF

### Opción 1: Con Python (Recomendado)
Ejecuta en tu terminal:
```bash
python build_pdf.py
```

Opciones disponibles:
- **Modo en vivo (Typst Watch)**: Recompila instantáneamente cada vez que guardas cambios:
  ```bash
  python build_pdf.py --watch
  ```
- **Solo compilar Typst (Rápido, reusando `body.typ`)**:
  ```bash
  python build_pdf.py --no-pandoc
  ```
- **Compilar otro archivo Markdown**:
  ```bash
  python build_pdf.py --input otro_archivo.md --output mi_notebook.pdf
  ```

### Opción 2: Con PowerShell
```powershell
.\build_pdf.ps1
```

### Opción 3: Manual por Consola
Si deseas correr los comandos directamente:
```bash
pandoc Notebook_ICPC_Final.md -f markdown -t typst -o body.typ
typst compile notebook_3col.typ Notebook_ICPC_Final.pdf
```

---

## 🎨 Cómo Funciona la Plantilla Typst (`notebook_3col.typ`)

1. **Página Apaisada en A4**:
   ```typst
   #set page(paper: "a4", flipped: true, margin: (top: 1.0cm, bottom: 0.8cm, left: 0.7cm, right: 0.7cm))
   ```
2. **3 Columnas con Separador**:
   ```typst
   #show: columns.with(3, gutter: 0.55cm)
   ```
3. **Syntax Highlighting Automático para C++ y Python**:
   Typst detecta los bloques de código y les aplica fuente monospace Consolas con barra azul a la izquierda y fondo claro que no satura la tinta de impresión.
4. **Encabezados con Banners de Color**:
   Los bloques temáticos (`##`) tienen un banner azul marino con texto blanco para identificar fácilmente las secciones durante una competencia.
