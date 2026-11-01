#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
build_pdf.py — Generador automático de PDF para ICPC Notebook con Typst
========================================================================

Flujo de generación:
  1. Lee y preprocesa el archivo Markdown EN MEMORIA (sin modificar el .md fuente).
  2. Convierte el Markdown a Typst nativo (por defecto: body.typ) usando Pandoc.
  3. Compila notebook_3col.typ hacia un archivo temporal y reemplaza atómicamente
     el PDF de destino si la compilación tiene éxito.

Uso:
  python build_pdf.py                         # Compila todo desde cero
  python build_pdf.py --watch                 # Vigila cambios en el .md y recompila
  python build_pdf.py --no-pandoc             # Solo compila con Typst (reusa --body)
  python build_pdf.py --body custom_body.typ  # Usa otro archivo intermedio .typ
  python build_pdf.py --input mi_doc.md       # Usa otro archivo Markdown
"""

from __future__ import annotations
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))


def resolve_path(path_arg: str, is_output: bool = False) -> str:
    """
    Resuelve rutas relativas: si se ejecuta el script desde otro directorio de trabajo y
    el archivo por defecto vive junto a build_pdf.py, usa SCRIPT_DIR.
    """
    if os.path.isabs(path_arg):
        return path_arg
    cwd_candidate = os.path.abspath(path_arg)
    if os.path.exists(cwd_candidate):
        return cwd_candidate
    script_candidate = os.path.join(SCRIPT_DIR, path_arg)
    if os.path.exists(script_candidate) or is_output:
        return script_candidate
    return cwd_candidate


def check_prerequisites(need_pandoc: bool, body_path: str) -> None:
    """Verifica las herramientas requeridas según las banderas activas."""
    missing = []
    if need_pandoc and not shutil.which("pandoc"):
        missing.append("pandoc (Instalar con: winget install JohnMacFarlane.Pandoc)")
    if not shutil.which("typst"):
        missing.append("typst (Instalar con: winget install --id Typst.Typst)")
    if missing:
        print("[ERROR] Herramientas faltantes:", file=sys.stderr)
        for tool in missing:
            print(f"  - {tool}", file=sys.stderr)
        sys.exit(1)

    if not need_pandoc and not os.path.exists(body_path):
        print(
            f"[ERROR] Se indicó --no-pandoc pero no existe el archivo --body: {body_path}",
            file=sys.stderr,
        )
        sys.exit(1)


def clean_markdown_math(text: str) -> str:
    """
    Normaliza delimitadores matemáticos comunes de Markdown en memoria:
      $`...`$  ->  $...$
      &lt;     ->  <
      &gt;     ->  >
    """
    bt = chr(96)
    target_start = "$" + bt
    target_end = bt + "$"
    parts = text.split(target_start)
    out = [parts[0]]
    for p in parts[1:]:
        if target_end in p:
            m, rest = p.split(target_end, 1)
            out.append("$" + m + "$" + rest)
        else:
            out.append(target_start + p)
    cleaned = "".join(out)
    cleaned = cleaned.replace("&lt;", "<").replace("&gt;", ">")
    return cleaned


def postprocess_typst_tables(typ_text: str) -> str:
    """
    Ajusta las tablas generadas por Pandoc para el diseño de 3 columnas de Typst:
      - Desenvuelve align(center)[#table(...)] para que sean rompibles entre columnas.
      - Ajusta proporciones de columnas en tablas de 4, 3 y 2 columnas para evitar
        celdas aplastadas o cortes de palabras.
    """
    typ_text = typ_text.replace(
        "columns: (25%, 25%, 25%, 25%),",
        "columns: (29%, 24%, 35%, 12%),",
    )
    typ_text = typ_text.replace(
        "columns: (33.33%, 33.33%, 33.33%),",
        "columns: (22%, 32%, 46%),",
    )
    typ_text = typ_text.replace(
        "columns: (50%, 50%),",
        "columns: (38%, 62%),",
    )
    typ_text = re.sub(
        r"#figure\(\s*align\(center\)\[#table\(",
        "#figure(\n  table(",
        typ_text,
    )
    typ_text = re.sub(
        r"\)\]\s*,\s*kind:\s*table\s*\)",
        "),\n  kind: table\n)",
        typ_text,
    )
    return typ_text


def convert_markdown_to_typst(md_path: str, typ_body_path: str) -> None:
    """Preprocesa el Markdown en memoria (sin alterar md_path) y genera typ_body_path con Pandoc."""
    if not os.path.exists(md_path):
        print(f"[ERROR] Archivo Markdown no encontrado: {md_path}", file=sys.stderr)
        sys.exit(1)

    print(f"[1/2] Preprocesando en memoria {os.path.basename(md_path)} (sin modificar el original)...")
    with open(md_path, "r", encoding="utf-8") as f:
        content = f.read()

    cleaned = clean_markdown_math(content)
    os.makedirs(os.path.dirname(os.path.abspath(typ_body_path)), exist_ok=True)

    print(f"[2/2] Convirtiendo a Typst mediante Pandoc -> {os.path.basename(typ_body_path)}...")
    with tempfile.NamedTemporaryFile(
        mode="w", suffix=".md", encoding="utf-8", delete=False
    ) as tmp_md:
        tmp_md.write(cleaned)
        tmp_md_path = tmp_md.name

    try:
        cmd = [
            "pandoc",
            tmp_md_path,
            "-f",
            "markdown",
            "-t",
            "typst",
            "-o",
            typ_body_path,
        ]
        res = subprocess.run(
            cmd, capture_output=True, text=True, encoding="utf-8", errors="replace"
        )
        if res.returncode != 0:
            print("[ERROR] Pandoc falló:", file=sys.stderr)
            print(res.stderr, file=sys.stderr)
            sys.exit(res.returncode)
        with open(typ_body_path, "r", encoding="utf-8") as tf:
            typ_content = tf.read()
        typ_content = postprocess_typst_tables(typ_content)
        with open(typ_body_path, "w", encoding="utf-8") as tf:
            tf.write(typ_content)
    finally:
        if os.path.exists(tmp_md_path):
            os.remove(tmp_md_path)


def compile_typst_once(template_path: str, body_path: str, output_pdf_path: str) -> int:
    """Compila el archivo maestro de Typst a un PDF temporal y reemplaza atómicamente el destino."""
    if not os.path.exists(template_path):
        print(f"[ERROR] Plantilla de Typst no encontrada: {template_path}", file=sys.stderr)
        return 1

    template_dir = os.path.dirname(os.path.abspath(template_path))
    body_abs = os.path.abspath(body_path)
    root_dir = os.path.commonpath([template_dir, os.path.dirname(body_abs)])
    rel_body_from_template = os.path.relpath(body_abs, template_dir).replace("\\", "/")

    os.makedirs(os.path.dirname(os.path.abspath(output_pdf_path)), exist_ok=True)
    tmp_pdf_path = output_pdf_path + ".tmp.pdf"

    print(
        f"[TYPST] Compilando {os.path.basename(template_path)} (body={rel_body_from_template}) -> {os.path.basename(output_pdf_path)}..."
    )
    t0 = time.time()
    cmd = [
        "typst",
        "compile",
        "--root",
        root_dir,
        "--input",
        f"body={rel_body_from_template}",
        template_path,
        tmp_pdf_path,
    ]
    res = subprocess.run(
        cmd, capture_output=True, text=True, encoding="utf-8", errors="replace"
    )
    dt = time.time() - t0

    if res.returncode != 0:
        if os.path.exists(tmp_pdf_path):
            os.remove(tmp_pdf_path)
        print("[ERROR] Typst falló:", file=sys.stderr)
        print(res.stderr, file=sys.stderr)
        return res.returncode

    try:
        os.replace(tmp_pdf_path, output_pdf_path)
    except PermissionError as exc:
        if os.path.exists(tmp_pdf_path):
            os.remove(tmp_pdf_path)
        print(
            f"[ERROR] No se pudo reemplazar '{output_pdf_path}' (¿está abierto y bloqueado en un visor de PDF?): {exc}",
            file=sys.stderr,
        )
        return 1

    if os.path.exists(output_pdf_path):
        size_mb = os.path.getsize(output_pdf_path) / (1024 * 1024)
        with open(output_pdf_path, "rb") as f:
            pdf_bytes = f.read()
        pages = len(re.findall(rb"/Type\s*/Page\b", pdf_bytes))
        print(f"[EXITO] PDF generado: {output_pdf_path}")
        print(f"        Páginas: {pages} | Tamaño: {size_mb:.2f} MB | Tiempo: {dt:.2f}s")
    return 0


def watch_loop(
    md_path: str,
    template_path: str,
    body_path: str,
    output_pdf_path: str,
    no_pandoc: bool,
) -> None:
    """
    Modo vigilancia real: monitorea cambios tanto en el .md fuente (si no es --no-pandoc)
    como en la plantilla .typ / body.typ y recompila automáticamente.
    """
    print(f"[LIVE] Vigilando cambios ({'body.typ' if no_pandoc else os.path.basename(md_path)} + {os.path.basename(template_path)})...")
    print("Presiona Ctrl+C para finalizar.")

    last_md_mtime = 0.0
    last_tpl_mtime = 0.0
    last_body_mtime = 0.0

    try:
        while True:
            md_mtime = os.path.getmtime(md_path) if (not no_pandoc and os.path.exists(md_path)) else 0.0
            tpl_mtime = os.path.getmtime(template_path) if os.path.exists(template_path) else 0.0
            body_mtime = os.path.getmtime(body_path) if os.path.exists(body_path) else 0.0

            need_compile = False
            if not no_pandoc and md_mtime > last_md_mtime:
                last_md_mtime = md_mtime
                convert_markdown_to_typst(md_path, body_path)
                body_mtime = os.path.getmtime(body_path) if os.path.exists(body_path) else 0.0
                last_body_mtime = body_mtime
                need_compile = True

            if tpl_mtime > last_tpl_mtime or body_mtime > last_body_mtime:
                last_tpl_mtime = tpl_mtime
                last_body_mtime = body_mtime
                need_compile = True

            if need_compile:
                compile_typst_once(template_path, body_path, output_pdf_path)

            time.sleep(0.8)
    except KeyboardInterrupt:
        print("\n[OK] Modo watch finalizado.")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Generador de PDF con Typst para ICPC Notebook"
    )
    parser.add_argument(
        "--input",
        "-i",
        default="Notebook_ICPC_Final.md",
        help="Archivo Markdown de entrada",
    )
    parser.add_argument(
        "--template",
        "-t",
        default="notebook_3col.typ",
        help="Plantilla Typst maestra",
    )
    parser.add_argument(
        "--body",
        "-b",
        default="body.typ",
        help="Archivo Typst intermedio generado por Pandoc",
    )
    parser.add_argument(
        "--output",
        "-o",
        default="Notebook_ICPC_Final.pdf",
        help="Archivo PDF final",
    )
    parser.add_argument(
        "--watch",
        "-w",
        action="store_true",
        help="Vigila cambios en el .md y la plantilla en tiempo real",
    )
    parser.add_argument(
        "--no-pandoc",
        action="store_true",
        help="Omite la conversión de Pandoc y reusa el archivo --body existente",
    )

    args = parser.parse_args()

    md_path = resolve_path(args.input, is_output=False)
    template_path = resolve_path(args.template, is_output=False)
    body_path = resolve_path(args.body, is_output=not args.no_pandoc)
    output_pdf_path = resolve_path(args.output, is_output=True)

    check_prerequisites(need_pandoc=not args.no_pandoc, body_path=body_path)

    if args.watch:
        watch_loop(md_path, template_path, body_path, output_pdf_path, args.no_pandoc)
        return

    if not args.no_pandoc:
        convert_markdown_to_typst(md_path, body_path)

    rc = compile_typst_once(template_path, body_path, output_pdf_path)
    if rc != 0:
        sys.exit(rc)


if __name__ == "__main__":
    main()
