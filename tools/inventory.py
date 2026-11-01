#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
tools/inventory.py — Generador y verificador del inventario maestro del Notebook ICPC
====================================================================================

Inspecciona Notebook_ICPC_Final.md, identifica cada sección teórica y cada ficha
operativa de algoritmo (### X.Y o #### X.Y.Z), verifica la presencia y orden de las
10 partes obligatorias de cada ficha:
  1. Cuándo usar
  2. Cuándo no usar / alternativa
  3. Contrato (y dependencias)
  4. Complejidad
  5. Formato de entrada
  6. Entrada y Salida de ejemplo (bloques ```text)
  7. Cómo leer la entrada (ANTES de Implementación)
  8. Implementación
  9. Cómo llamarlo y producir la salida (DESPUÉS de Implementación)
 10. Trampas y adaptaciones

Genera INVENTORY.md en la raíz del repositorio y tests/inventory.json.
"""

from __future__ import annotations
import argparse
import json
import os
import re
import sys
from typing import Any, Dict, List

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(SCRIPT_DIR)
if SCRIPT_DIR not in sys.path:
    sys.path.insert(0, SCRIPT_DIR)

from extract_snippets import extract_snippets


THEORETICAL_SECTIONS: Dict[str, str] = {
    "0.1": "Tabla de selección rápida (navegación, sin código ejecutable)",
    "0.2": "Tabla orientativa de complejidad según N (teórica)",
    "0.3": "Advertencias críticas de elección de técnica (teórica)",
    "0.4": "Checklist anti-WA antes de enviar (teórica)",
    "7.8": "Referencia cruzada a §7.1.4 (evita duplicar segmented_sieve)",
    "11.1": "Condiciones teóricas de optimización de DP (tabla)",
    "11.2": "Fórmulas de combinatoria y sumatorias (teórica)",
    "11.3": "Propiedades de teoría de números y aritmética modular (teórica)",
    "11.4": "Teoremas de grafos, árboles y redes (teórica)",
    "11.5": "Fórmulas de geometría y distancias (teórica)",
    "11.6": "Identidades de teoría de juegos y XOR (teórica)",
    "11.7": "Equivalencias lógicas de 2-SAT (teórica)",
    "11.8": "Tabla de constantes, primos y módulos frecuentes (teórica)",
}


def parse_fiches_from_markdown(md_path: str) -> List[Dict[str, Any]]:
    with open(md_path, "r", encoding="utf-8") as fh:
        text = fh.read()

    snips = extract_snippets(md_path)
    algos_by_sec: Dict[str, Any] = {}
    for s in snips:
        if s.role == "algorithm":
            algos_by_sec[s.section_num] = s

    # Dividir por ### X.Y
    sec3_pattern = re.compile(
        r"(?ms)^###\s+(\d+\.\d+)\s+([^\n]+)\n(.*?)(?=^###\s+\d+\.\d+\s+|^##\s+Bloque\s+|\Z)"
    )
    sub4_pattern = re.compile(
        r"(?ms)^####\s+(\d+\.\d+\.\d+)\s+([^\n]+)\n(.*?)(?=^####\s+\d+\.\d+\.\d+\s+|\Z)"
    )

    records: List[Dict[str, Any]] = []

    for m3 in sec3_pattern.finditer(text):
        sec_num = m3.group(1)
        sec_title = m3.group(2).strip()
        sec_body = m3.group(3)

        if sec_num in THEORETICAL_SECTIONS:
            records.append(
                {
                    "section": sec_num,
                    "title": sec_title,
                    "kind": "theoretical",
                    "snippet_id": "-",
                    "lang": "-",
                    "dependencies": [],
                    "status": "no_aplicable",
                    "justification": THEORETICAL_SECTIONS[sec_num],
                    "fields_ok": True,
                    "order_ok": True,
                }
            )
            continue

        sub_matches = list(sub4_pattern.finditer(sec_body))
        units = (
            [(sm.group(1), sm.group(2).strip(), sm.group(3)) for sm in sub_matches]
            if sub_matches
            else [(sec_num, sec_title, sec_body)]
        )

        for u_num, u_title, u_body in units:
            pos_usar = u_body.find("- **Cuándo usar:**")
            pos_no_usar = u_body.find("- **Cuándo no usar / alternativa:**")
            pos_contrato = u_body.find("- **Contrato:**")
            pos_comp = u_body.find("- **Complejidad:**")
            pos_fmt = u_body.find("- **Formato de entrada:**")
            pos_in = u_body.find("**Entrada:**")
            pos_out = u_body.find("**Salida:**")
            pos_reader = u_body.find("**Cómo leer la entrada:**")
            pos_impl = u_body.find("**Implementación:**")
            pos_call = u_body.find("**Cómo llamarlo y producir la salida:**")
            pos_traps = u_body.find("- **Trampas y adaptaciones:**")

            positions = [
                pos_usar,
                pos_no_usar,
                pos_contrato,
                pos_comp,
                pos_fmt,
                pos_in,
                pos_out,
                pos_reader,
                pos_impl,
                pos_call,
                pos_traps,
            ]
            fields_ok = all(p != -1 for p in positions)
            order_ok = fields_ok and positions == sorted(positions)

            algo_snip = algos_by_sec.get(u_num)
            snip_id = algo_snip.id if algo_snip else f"sec_{u_num.replace('.', '_')}"
            lang = algo_snip.lang if algo_snip else "cpp"
            deps = algo_snip.dependencies if algo_snip else []

            records.append(
                {
                    "section": u_num,
                    "title": u_title,
                    "kind": "algorithm",
                    "snippet_id": snip_id,
                    "lang": lang,
                    "dependencies": deps,
                    "status": "completo" if (fields_ok and order_ok) else "incompleto",
                    "justification": (
                        "10 partes completas y verificadas (lector -> implementación -> llamada)"
                        if (fields_ok and order_ok)
                        else f"Faltan campos o el orden es inválido: positions={positions}"
                    ),
                    "fields_ok": fields_ok,
                    "order_ok": order_ok,
                }
            )

    return records


def write_inventory_files(
    records: List[Dict[str, Any]], md_out: str, json_out: str
) -> None:
    os.makedirs(os.path.dirname(os.path.abspath(json_out)), exist_ok=True)
    with open(json_out, "w", encoding="utf-8") as jf:
        json.dump(records, jf, indent=2, ensure_ascii=False)

    algo_records = [r for r in records if r["kind"] == "algorithm"]
    theo_records = [r for r in records if r["kind"] == "theoretical"]
    complete_cnt = sum(1 for r in algo_records if r["status"] == "completo")

    lines = [
        "# Inventario Maestro del Notebook ICPC (OUR BRAINS ARE DIAZ O)",
        "",
        f"- **Total de fichas operativas de algoritmos:** `{len(algo_records)}` (`{complete_cnt}/{len(algo_records)}` en estado `completo`)",
        f"- **Total de secciones teóricas / tablas de referencia:** `{len(theo_records)}` (`no_aplicable` con justificación explícita)",
        "- **Orden verificado en cada ficha de algoritmo:** `Cuándo usar` → `Cuándo no usar` → `Contrato` → `Complejidad` → `Formato de entrada` → `Entrada` → `Salida` → **`Cómo leer la entrada`** → **`Implementación`** → **`Cómo llamarlo y producir la salida`** → `Trampas y adaptaciones`.",
        "",
        "## 1. Fichas Operativas de Algoritmos",
        "",
        "| Sección | ID Estable | Algoritmo / Estructura | Lenguaje | Dependencias | Estado de las 10 Partes |",
        "| :--- | :--- | :--- | :--- | :--- | :--- |",
    ]
    for r in algo_records:
        deps_str = ", ".join(f"`{d}`" for d in r["dependencies"]) if r["dependencies"] else "`base_template`"
        lines.append(
            f"| `§{r['section']}` | `{r['snippet_id']}` | {r['title']} | `{r['lang']}` | {deps_str} | `{r['status']}` |"
        )

    lines.extend(
        [
            "",
            "## 2. Secciones Teóricas, Navegación y Referencias Cruzadas",
            "",
            "| Sección | Título | Estado | Justificación |",
            "| :--- | :--- | :--- | :--- |",
        ]
    )
    for r in theo_records:
        lines.append(
            f"| `§{r['section']}` | {r['title']} | `{r['status']}` | {r['justification']} |"
        )
    lines.append("")

    with open(md_out, "w", encoding="utf-8", newline="\n") as mf:
        mf.write("\n".join(lines))


def main() -> None:
    parser = argparse.ArgumentParser(description="Generador de inventario del Notebook ICPC")
    parser.add_argument(
        "--input",
        "-i",
        default=os.path.join(ROOT_DIR, "Notebook_ICPC_Final.md"),
        help="Ruta al archivo Markdown fuente",
    )
    parser.add_argument(
        "--output-md",
        default=os.path.join(ROOT_DIR, "INVENTORY.md"),
        help="Ruta de salida para INVENTORY.md",
    )
    parser.add_argument(
        "--output-json",
        default=os.path.join(ROOT_DIR, "tests", "inventory.json"),
        help="Ruta de salida para tests/inventory.json",
    )
    args = parser.parse_args()

    records = parse_fiches_from_markdown(args.input)
    write_inventory_files(records, args.output_md, args.output_json)

    incomplete = [r for r in records if r["status"] == "incompleto"]
    algos = [r for r in records if r["kind"] == "algorithm"]
    print(
        f"Inventario generado: {len(algos)} fichas de algoritmos ({len(algos) - len(incomplete)} completas), "
        f"{len(records) - len(algos)} secciones teóricas."
    )
    if incomplete:
        for r in incomplete:
            print(f"  [INCOMPLETO] §{r['section']} {r['title']}: {r['justification']}")
        sys.exit(1)


if __name__ == "__main__":
    main()
