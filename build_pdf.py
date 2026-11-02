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


def normalize_lang_filter(lang_filter: str = "all") -> str:
    val = (lang_filter or "all").strip().lower()
    if val in ("py", "python"):
        return "py"
    if val in ("cpp", "c++"):
        return "cpp"
    return "all"


def iter_markdown_files(source_path: str, lang_filter: str = "all") -> list[str]:
    norm = normalize_lang_filter(lang_filter)
    if os.path.isfile(source_path):
        return [source_path]
    if not os.path.isdir(source_path):
        return []
    md_files: list[str] = []
    for root, dirs, files in os.walk(source_path):
        dirs.sort()
        has_cpp = any(f.endswith((".cpp", ".cc", ".cxx", ".hpp", ".h")) for f in files)
        has_py = any(f.endswith(".py") for f in files)
        if norm == "cpp" and has_py and not has_cpp:
            continue
        if norm == "py" and not has_py:
            continue
        for fname in sorted(files):
            if fname.endswith(".md"):
                md_files.append(os.path.join(root, fname))
    return md_files


OPTIONAL_TAG_RE = re.compile(
    r"\s*(?:\[(?:opcional|optional)\]|<!--\s*(?:opcional|optional)\s*-->)\s*$",
    re.IGNORECASE,
)


def expand_markdown_file(md_file: str, lang_filter: str = "all", include_optional: bool = False) -> str:
    norm = normalize_lang_filter(lang_filter)
    base_dir = os.path.dirname(os.path.abspath(md_file))
    with open(md_file, "r", encoding="utf-8") as f:
        raw_lines = f.read().splitlines()

    out: list[str] = []
    in_fence = False
    fence_marker = ""
    skip_until_level: int | None = None
    has_emitted_h3 = False
    pending_hr = False
    include_re = re.compile(
        r"^--\s+(\S+\.(?:cpp|cc|cxx|hpp|h|py))(?:\s+\[(?:opcional|optional)\])?\s*$",
        re.IGNORECASE,
    )
    heading_re = re.compile(r"^(#{2,4})\s+(.*)$")

    for line_no, line in enumerate(raw_lines, start=1):
        stripped = line.strip()
        if not in_fence:
            m_fence = re.match(r"^(`{3,}|~{3,})\s*(\S*).*$", stripped)
            if m_fence:
                in_fence = True
                fence_marker = m_fence.group(1)
                if skip_until_level is None:
                    out.append(line)
                continue

            if re.match(r"^-{3,}\s*$", stripped):
                if has_emitted_h3:
                    pending_hr = True
                continue

            m_head = heading_re.match(stripped)
            if m_head:
                level = len(m_head.group(1))
                if skip_until_level is not None and level <= skip_until_level:
                    skip_until_level = None
                is_opt = bool(OPTIONAL_TAG_RE.search(stripped))
                if skip_until_level is not None:
                    continue
                if is_opt and not include_optional:
                    skip_until_level = level
                    continue
                if level == 2:
                    has_emitted_h3 = False
                    pending_hr = False
                elif level == 3:
                    if pending_hr and has_emitted_h3:
                        out.append("---")
                        out.append("")
                    has_emitted_h3 = True
                    pending_hr = False
                clean_line = OPTIONAL_TAG_RE.sub("", line)
                out.append(clean_line)
                continue

            if skip_until_level is not None:
                continue

            m_inc = include_re.match(stripped)
            if m_inc:
                is_opt_inc = bool(OPTIONAL_TAG_RE.search(stripped))
                if is_opt_inc and not include_optional:
                    continue
                rel_code = m_inc.group(1)
                is_py = rel_code.endswith(".py")
                if norm == "cpp" and is_py:
                    continue
                if norm == "py" and not is_py:
                    continue
                code_path = os.path.join(base_dir, rel_code)
                if not os.path.isfile(code_path):
                    raise FileNotFoundError(
                        f"Archivo de código '{rel_code}' no encontrado (referenciado en {md_file}:{line_no})"
                    )
                with open(code_path, "r", encoding="utf-8") as cf:
                    code_body = cf.read().rstrip()
                lang = "python" if is_py else "cpp"
                out.append(f"```{lang}")
                out.append(code_body)
                out.append("```")
                continue
        else:
            if stripped.startswith(fence_marker) and len(stripped) == len(fence_marker):
                in_fence = False
                if skip_until_level is None:
                    out.append(line)
                continue
            if skip_until_level is not None:
                continue

        out.append(line)

    return "\n".join(out).strip() + "\n"


def get_source_mtime(source_path: str) -> float:
    if os.path.isfile(source_path):
        return os.path.getmtime(source_path)
    if not os.path.isdir(source_path):
        return 0.0
    latest = 0.0
    for root, dirs, files in os.walk(source_path):
        for fname in files:
            if fname.endswith((".md", ".cpp", ".cc", ".cxx", ".hpp", ".h", ".py")):
                p = os.path.join(root, fname)
                if os.path.exists(p):
                    latest = max(latest, os.path.getmtime(p))
    return latest


def convert_markdown_to_typst(
    md_path: str,
    typ_body_path: str,
    lang_filter: str = "all",
    include_optional: bool = False,
) -> None:
    """Convierte md_path (directorio modular o archivo .md) a typ_body_path con Pandoc."""
    norm = normalize_lang_filter(lang_filter)
    md_files = iter_markdown_files(md_path, lang_filter=norm)
    if not md_files:
        print(f"[ERROR] Fuente Markdown no encontrada: {md_path} (lang={norm})", file=sys.stderr)
        sys.exit(1)

    os.makedirs(os.path.dirname(os.path.abspath(typ_body_path)), exist_ok=True)
    opt_label = "opcionales=incluidos" if include_optional else "opcionales=omitidos"
    label = (
        f"{os.path.basename(md_path)}/ ({len(md_files)} bloques, lang={norm}, {opt_label})"
        if os.path.isdir(md_path)
        else f"{os.path.basename(md_path)} (lang={norm}, {opt_label})"
    )
    print(f"[PANDOC] Convirtiendo {label} -> {os.path.basename(typ_body_path)}...")
    combined_md = "\n\n".join(
        expand_markdown_file(p, lang_filter=norm, include_optional=include_optional).rstrip()
        for p in md_files
    )
    combined_md = re.sub(r"^\s*-{3,}\s*\n+", "", combined_md).strip() + "\n"
    cmd = [
        "pandoc",
        "-f",
        "markdown",
        "-t",
        "typst",
        "-o",
        typ_body_path,
    ]
    res = subprocess.run(
        cmd, input=combined_md, capture_output=True, text=True, encoding="utf-8", errors="replace"
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
    lang_filter: str = "all",
    include_optional: bool = False,
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
            md_mtime = get_source_mtime(md_path) if not no_pandoc else 0.0
            tpl_mtime = os.path.getmtime(template_path) if os.path.exists(template_path) else 0.0
            body_mtime = os.path.getmtime(body_path) if os.path.exists(body_path) else 0.0

            need_compile = False
            if not no_pandoc and md_mtime > last_md_mtime:
                last_md_mtime = md_mtime
                convert_markdown_to_typst(
                    md_path, body_path, lang_filter=lang_filter, include_optional=include_optional
                )
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
        default="sections",
        help="Directorio modular (sections/) o archivo Markdown de entrada",
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
    lang_group = parser.add_mutually_exclusive_group()
    lang_group.add_argument(
        "--lang",
        "-l",
        choices=["all", "cpp", "py", "python"],
        default="all",
        help="Filtra qué lenguaje compilar: all (por defecto), cpp (solo C++) o py (solo Python)",
    )
    lang_group.add_argument(
        "--cpp",
        action="store_true",
        help="Atajo para --lang cpp (compila únicamente los bloques de C++ y teoría general)",
    )
    lang_group.add_argument(
        "--py",
        "--python",
        action="store_true",
        help="Atajo para --lang py (compila únicamente el bloque de Python)",
    )
    opt_group = parser.add_mutually_exclusive_group()
    opt_group.add_argument(
        "--optional",
        "--include-optional",
        "--full",
        dest="include_optional",
        action="store_true",
        help="Incluye las secciones avanzadas marcadas con [opcional] en los index.md",
    )
    opt_group.add_argument(
        "--no-optional",
        "--essential",
        "--core",
        dest="exclude_optional",
        action="store_true",
        help="Omite las secciones avanzadas marcadas con [opcional] (comportamiento por defecto)",
    )

    args = parser.parse_args()
    lang_filter = "cpp" if args.cpp else ("py" if args.py else normalize_lang_filter(args.lang))
    include_optional = bool(args.include_optional)

    md_path = resolve_path(args.input, is_output=False)
    template_path = resolve_path(args.template, is_output=False)
    body_path = resolve_path(args.body, is_output=not args.no_pandoc)
    output_pdf_path = resolve_path(args.output, is_output=True)

    check_prerequisites(need_pandoc=not args.no_pandoc, body_path=body_path)

    if args.watch:
        watch_loop(
            md_path,
            template_path,
            body_path,
            output_pdf_path,
            args.no_pandoc,
            lang_filter=lang_filter,
            include_optional=include_optional,
        )
        return

    if not args.no_pandoc:
        convert_markdown_to_typst(
            md_path, body_path, lang_filter=lang_filter, include_optional=include_optional
        )

    rc = compile_typst_once(template_path, body_path, output_pdf_path)
    if rc != 0:
        sys.exit(rc)


if __name__ == "__main__":
    main()
