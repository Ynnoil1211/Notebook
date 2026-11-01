#!/usr/bin/env python3
"""
build_pdf.py — Generador automático de PDF para ICPC Notebook con Typst
========================================================================

Este script realiza todo el flujo de generación:
  1. Toma tu archivo Markdown (por defecto: Notebook_ICPC_Final.md).
  2. Limpia y convierte el Markdown a Typst nativo (body.typ) usando Pandoc.
  3. Compila notebook_3col.typ usando Typst para generar el PDF final
     en 3 columnas apaisadas con resaltado de sintaxis (Syntax Highlighting).

Uso:
  python build_pdf.py                     # Compila todo desde cero
  python build_pdf.py --watch             # Modo vigilancia (Typst watch en vivo)
  python build_pdf.py --no-pandoc         # Solo compila con Typst (reusa body.typ)
  python build_pdf.py --input mi_doc.md   # Usar otro archivo Markdown

Requisitos:
  - Pandoc (https://pandoc.org/installing.html) -> winget install JohnMacFarlane.Pandoc
  - Typst  (https://github.com/typst/typst)     -> winget install --id Typst.Typst
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import time


def check_prerequisites():
    """Verifica que pandoc y typst estén instalados y en el PATH."""
    missing = []
    if not shutil.which("pandoc"):
        missing.append("pandoc (Instalar con: winget install JohnMacFarlane.Pandoc)")
    if not shutil.which("typst"):
        missing.append("typst (Instalar con: winget install --id Typst.Typst)")
    if missing:
        print("[ERROR] Herramientas faltantes:")
        for tool in missing:
            print(f"  - {tool}")
        sys.exit(1)


def clean_markdown_math(text: str) -> str:
    """
    Normaliza delimitadores matemáticos comunes de Markdown:
      $`...`$  ->  $...$
      &lt;     ->  <
      &gt;     ->  >
    """
    bt = chr(96)
    target_start = '$' + bt
    target_end = bt + '$'
    parts = text.split(target_start)
    out = [parts[0]]
    for p in parts[1:]:
        if target_end in p:
            m, rest = p.split(target_end, 1)
            out.append('$' + m + '$' + rest)
        else:
            out.append(target_start + p)
    cleaned = ''.join(out)
    cleaned = cleaned.replace("&lt;", "<").replace("&gt;", ">")
    return cleaned


def convert_markdown_to_typst(md_path: str, typ_body_path: str):
    """Preprocesa el Markdown y lo convierte a Typst usando Pandoc."""
    if not os.path.exists(md_path):
        print(f"[ERROR] Archivo Markdown no encontrado: {md_path}")
        sys.exit(1)

    print(f"[1/2] Preprocesando {os.path.basename(md_path)}...")
    with open(md_path, "r", encoding="utf-8") as f:
        content = f.read()

    cleaned = clean_markdown_math(content)
    if cleaned != content:
        with open(md_path, "w", encoding="utf-8") as f:
            f.write(cleaned)

    print(f"[2/2] Convirtiendo a Typst mediante Pandoc -> {os.path.basename(typ_body_path)}...")
    cmd = [
        "pandoc",
        md_path,
        "-f", "markdown",
        "-t", "typst",
        "-o", typ_body_path
    ]
    res = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", errors="ignore")
    if res.returncode != 0:
        print(f"[ERROR] Pandoc falló:")
        print(res.stderr)
        sys.exit(res.returncode)


def compile_typst(template_path: str, output_pdf_path: str, watch: bool = False):
    """Compila el archivo maestro de Typst a PDF."""
    if not os.path.exists(template_path):
        print(f"[ERROR] Plantilla de Typst no encontrada: {template_path}")
        sys.exit(1)

    if watch:
        print(f"[LIVE] Iniciando 'typst watch' sobre {template_path} -> {output_pdf_path}")
        print("Guarda cambios en body.typ o en la plantilla para actualización instantánea...")
        try:
            subprocess.run(["typst", "watch", template_path, output_pdf_path])
        except KeyboardInterrupt:
            print("\n[OK] Modo watch finalizado.")
        return

    print(f"[TYPST] Compilando {os.path.basename(template_path)} -> {os.path.basename(output_pdf_path)}...")
    t0 = time.time()
    res = subprocess.run(["typst", "compile", template_path, output_pdf_path], capture_output=True, text=True, encoding="utf-8", errors="ignore")
    dt = time.time() - t0

    if res.returncode != 0:
        print("[ERROR] Typst falló:")
        print(res.stderr)
        sys.exit(res.returncode)

    if os.path.exists(output_pdf_path):
        size_mb = os.path.getsize(output_pdf_path) / (1024 * 1024)
        # Contar páginas
        with open(output_pdf_path, "rb") as f:
            pdf_bytes = f.read()
        pages = len(re.findall(rb'/Type\s*/Page\b', pdf_bytes))
        print(f"[EXITO] PDF generado: {output_pdf_path}")
        print(f"        Páginas: {pages} | Tamaño: {size_mb:.2f} MB | Tiempo: {dt:.2f}s")


def main():
    parser = argparse.ArgumentParser(description="Generador de PDF con Typst para ICPC Notebook")
    parser.add_argument("--input", "-i", default="Notebook_ICPC_Final.md", help="Archivo Markdown de entrada")
    parser.add_argument("--template", "-t", default="notebook_3col.typ", help="Plantilla Typst maestra")
    parser.add_argument("--body", "-b", default="body.typ", help="Archivo Typst intermedio generado por Pandoc")
    parser.add_argument("--output", "-o", default="Notebook_ICPC_Final.pdf", help="Archivo PDF final")
    parser.add_argument("--watch", "-w", action="store_true", help="Ejecuta 'typst watch' en tiempo real")
    parser.add_argument("--no-pandoc", action="store_true", help="Omite la conversión de Pandoc y reusa el body.typ existente")

    args = parser.parse_args()

    check_prerequisites()

    if not args.no_pandoc:
        convert_markdown_to_typst(args.input, args.body)

    compile_typst(args.template, args.output, watch=args.watch)


if __name__ == "__main__":
    main()
