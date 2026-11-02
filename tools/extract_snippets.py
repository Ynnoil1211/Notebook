#!/usr/bin/env python3
"""
tools/extract_snippets.py — Extractor y ensamblador de snippets del Notebook ICPC
=================================================================================

Identifica bloques de código en Notebook_ICPC_Final.md mediante IDs estables
(basados en símbolos definidos o slugs de sección), separa el código reutilizable
de funciones main() de demostración y permite ensamblar programas completos para pruebas.
"""

from __future__ import annotations
import argparse
import dataclasses
import json
import os
import re
from typing import Dict, List, Optional, Set

DEFAULT_SOURCE_PATH = "sections"


@dataclasses.dataclass
class Snippet:
    id: str
    section_num: str
    section_title: str
    heading_title: str
    lang: str
    role: str  # template | algorithm
    code: str
    reusable_code: str
    dependencies: List[str]
    symbols: List[str]
    block_index_in_section: int


SYMBOL_PATTERNS = [
    (r"\bstruct\s+([A-Za-z_]\w*)", "struct"),
    (r"\bclass\s+([A-Za-z_]\w*)", "class"),
    (r"\busing\s+([A-Za-z_]\w*)\s*=", "alias"),
    (
        r"\b(?:void|int|ll|ull|ld|double|bool|string|vi|vll|pii|pll|Pt|Matrix|Frac|Simplex|Dinic|MCMF)\s+([A-Za-z_]\w*)\s*\(",
        "func",
    ),
    (
        r"\b(?:vector|pair|tuple)\s*<(?:[^<>]|<[^<>]*>|<[^<>]*<[^<>]*>[^<>]*>)+>\s+([A-Za-z_]\w*)\s*\(",
        "func",
    ),
    (r"\bconst\s+int\s+(MAX_[A-Za-z0-9_]+)\s*=", "const"),
    (r"\bconstexpr\s+ll\s+(MOD)\s*=", "const"),
    (r"\bmt19937_64\s+([A-Za-z_]\w*)", "rng_var"),
    (r"\bvi\s+(adj_[A-Za-z0-9_]+)\s*\[", "global_arr"),
    (r"\bdef\s+([A-Za-z_]\w*)\s*\(", "py_func"),
]

# Mapa canónico de símbolos a IDs estables de algoritmos
CANONICAL_SYMBOL_IDS: Dict[str, str] = {
    "rd": "rng_mt19937",
    "rng": "rng_mt19937",
    "MOD": "modular_arith",
    "custom_hash": "custom_hash",
    "ordered_set": "pbds_ordered_set",
    "DSU": "dsu",
    "RollbackDSU": "rollback_dsu",
    "SparseTable": "sparse_table",
    "Fenwick": "fenwick_1d",
    "Fenwick2D": "fenwick_2d",
    "SegTree": "segtree_rec",
    "IterSegTree": "segtree_iter",
    "LazySegTree": "lazy_segtree",
    "SegTree2D": "segtree_2d",
    "PersistentST": "persistent_segtree",
    "HLD": "hld",
    "prefix_function": "kmp",
    "z_function": "z_algorithm",
    "BitTrie": "bit_trie",
    "AhoCorasick": "aho_corasick",
    "SuffixArray": "suffix_array",
    "SuffixAutomaton": "suffix_automaton",
    "topological_sort": "toposort",
    "BridgesAndCuts": "bridges_and_cuts",
    "TarjanSCC": "tarjan_scc",
    "TwoSAT": "twosat",
    "EulerianDirected": "eulerian_directed",
    "Dinic": "dinic",
    "MCMF": "mcmf",
    "BipartiteMVC": "bipartite_mvc",
    "hungarian": "hungarian",
    "Blossom": "blossom",
    "directed_mst": "directed_mst",
    "BlockCutTree": "block_cut_tree",
    "PushRelabel": "push_relabel",
    "sieve": "sieve_eratosthenes",
    "harmonic_sieve": "harmonic_sieve",
    "linear_sieve_spf": "linear_sieve_spf",
    "segmented_sieve": "segmented_sieve",
    "is_prime_single": "is_prime_single",
    "miller_rabin": "miller_rabin",
    "pollard_rho": "pollard_rho",
    "extgcd": "extgcd",
    "find_diophantine": "diophantine",
    "crt_general": "crt_general",
    "norm_mod": "modular_arith",
    "init_fact": "combinatorics",
    "euler_phi": "euler_phi",
    "init_phi_and_mobius": "phi_mobius_sieve",
    "Matrix": "matrix_exp",
    "XorBasis": "xor_basis",
    "gauss_double": "gauss_double",
    "gauss_mod": "gauss_mod",
    "fft": "fft",
    "multiply_poly": "fft",
    "ntt": "ntt",
    "multiply_ntt": "ntt",
    "fwt_xor": "fwt_xor",
    "discrete_log": "discrete_log",
    "Frac": "frac",
    "xor_n": "xor_range",
    "mex": "mex",
    "nim_normal_win": "nim",
    "Simplex": "simplex",
    "BerlekampMassey": "berlekamp_massey",
    "lagrange_eval_consecutive": "lagrange",
    "knapsack_01": "knapsack",
    "lcs_string": "lcs",
    "edit_distance": "edit_distance",
    "lis_length": "lis",
    "solve_digit_dp": "digit_dp",
    "sos_dp": "sos_dp",
    "LineContainer": "cht_dynamic",
    "MonotoneCHT": "cht_monotone",
    "compute_dc": "divide_and_conquer_dp",
    "solve_dc_dp": "divide_and_conquer_dp",
    "solve_knuth": "knuth_dp",
    "Pt": "geometry_2d",
    "PtLL": "geometry_int",
    "convex_hull": "convex_hull",
    "rotating_callipers_max_dist2": "rotating_callipers",
    "closest_pair_distance": "closest_pair",
    "Halfplane": "halfplane_intersection",
    "halfplane_intersection": "halfplane_intersection",
    "minkowski_sum": "minkowski_sum",
    "ExpressionParser": "shunting_yard",
    "MaxClique": "max_clique",
    "mitm_subset_sum_count": "meet_in_the_middle",
    "mitm_max_subset_sum_le": "meet_in_the_middle",
    "subset_assignment_dp": "subset_dp",
    "tsp_subset_dp": "subset_dp",
    "rerooting_sum_distances": "rerooting_dp",
    "count_subarrays_sum_k": "prefix_patterns",
    "kadane_max_subarray": "prefix_patterns",
    "interval_scheduling": "greedy_scheduling",
    "schedule_deadlines_heap": "greedy_scheduling",
    "exchange_argument_order": "greedy_scheduling",
    "max_interval_overlap": "sweep_line_1d",
    "merge_intervals": "sweep_line_1d",
    "FunctionalGraph": "functional_graph",
    "stars_and_bars_nonneg": "counting_expectation",
    "inclusion_exclusion_multiples": "counting_expectation",
    "sum_all_subarray_mins": "counting_expectation",
    "proj_point_line": "analytic_geometry_circles",
    "Circle": "analytic_geometry_circles",
    "circle_circle_intersection": "analytic_geometry_circles",
}

# Mapa inverso de ID canónico a símbolos C++
ID_TO_SYMBOLS: Dict[str, List[str]] = {}
for _sym, _cid in CANONICAL_SYMBOL_IDS.items():
    ID_TO_SYMBOLS.setdefault(_cid, []).append(_sym)

# Dependencias implícitas conocidas entre IDs canónicos
IMPLICIT_DEPENDENCIES: Dict[str, List[str]] = {
    "pollard_rho": ["rng_mt19937", "miller_rabin"],
    "combinatorics": ["extgcd", "modular_arith"],
    "counting_expectation": ["extgcd", "modular_arith", "combinatorics"],
    "prefix_patterns": ["custom_hash"],
    "berlekamp_massey": ["modular_arith"],
    "lagrange": ["modular_arith"],
    "matrix_exp": ["modular_arith", "custom_hash"],
    "discrete_log": ["custom_hash"],
    "twosat": ["tarjan_scc"],
    "halfplane_intersection": ["geometry_2d"],
    "minkowski_sum": ["geometry_2d"],
    "closest_pair": ["geometry_2d"],
    "convex_hull": ["geometry_2d"],
    "rotating_callipers": ["geometry_2d"],
    "analytic_geometry_circles": ["geometry_2d"],
}


def slugify(text: str) -> str:
    text = text.lower()
    text = re.sub(r"[^a-z0-9]+", "_", text)
    return text.strip("_")[:48]


def _find_matching_brace(code: str, brace_start: int) -> int:
    n = len(code)
    depth = 0
    j = brace_start
    in_str = False
    in_char = False
    while j < n:
        ch = code[j]
        if ch == "\\" and (in_str or in_char):
            j += 2
            continue
        if ch == '"' and not in_char:
            in_str = not in_str
        elif ch == "'" and not in_str:
            in_char = not in_char
        elif not in_str and not in_char:
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return j + 1
        j += 1
    return n


def strip_main_and_demos(code: str) -> str:
    """
    Elimina bloques `int main() { ... }` y `void solve() { ... }` del código
    para poder reutilizar definiciones de structs/funciones sin colisiones.
    """
    out = []
    i = 0
    n = len(code)
    pattern = re.compile(r"\b(?:int\s+main|void\s+solve|void\s+io_formatting_tricks)\s*\([^)]*\)\s*\{")
    while i < n:
        m = pattern.search(code, i)
        if not m:
            out.append(code[i:])
            break
        out.append(code[i : m.start()])
        brace_start = m.end() - 1
        i = _find_matching_brace(code, brace_start)
    return "".join(out).strip()


def remove_already_defined_structs(code: str, defined_structs: Set[str]) -> str:
    """
    Evita redefiniciones de structs cuando se ensamblan múltiples snippets.
    """
    out = []
    i = 0
    n = len(code)
    struct_pat = re.compile(r"(?:template\s*<[^>]+>\s*)?\bstruct\s+([A-Za-z_]\w*)\s*\{")
    while i < n:
        m = struct_pat.search(code, i)
        if not m:
            out.append(code[i:])
            break
        sname = m.group(1)
        brace_start = m.end() - 1
        end_pos = _find_matching_brace(code, brace_start)
        while end_pos < n and code[end_pos] in " \t;":
            if code[end_pos] == ";":
                end_pos += 1
                break
            end_pos += 1
        out.append(code[i : m.start()])
        if sname not in defined_structs:
            defined_structs.add(sname)
            out.append(code[m.start() : end_pos])
        i = end_pos
    return "".join(out)


def _top_level_code_only(code: str) -> str:
    clean = re.sub(r"//[^\n]*", "", code)
    clean = re.sub(r"/\*.*?\*/", "", clean, flags=re.DOTALL)
    out = []
    i = 0
    n = len(clean)
    while i < n:
        if clean[i] == "{":
            end = _find_matching_brace(clean, i)
            out.append("{ }")
            i = end
        else:
            out.append(clean[i])
            i += 1
    return "".join(out)


def detect_symbols(code: str) -> List[str]:
    tl = _top_level_code_only(code)
    found = []
    for pat, _kind in SYMBOL_PATTERNS:
        for m in re.finditer(pat, tl):
            sym = m.group(1)
            if sym not in ("main", "solve", "if", "for", "while", "switch") and sym not in found:
                found.append(sym)
    return found


def classify_block(code: str, section_num: str) -> str:
    if section_num.startswith("1.1") and "#include <bits/stdc++.h>" in code:
        return "template"
    return "algorithm"


def normalize_lang_filter(lang_filter: str = "all") -> str:
    val = (lang_filter or "all").strip().lower()
    if val in ("py", "python"):
        return "py"
    if val in ("cpp", "c++"):
        return "cpp"
    return "all"


def iter_markdown_files(source_path: str, lang_filter: str = "all") -> List[str]:
    norm = normalize_lang_filter(lang_filter)
    if os.path.isfile(source_path):
        return [source_path]
    if not os.path.isdir(source_path):
        raise FileNotFoundError(f"Ruta fuente no encontrada: {source_path}")
    md_files: List[str] = []
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


OPTIONAL_TAG_RE = re.compile(r"\s*(?:\[(?:opcional|optional)\]|<!--\s*(?:opcional|optional)\s*-->)\s*$", re.IGNORECASE)


def expand_markdown_file(md_file: str, lang_filter: str = "all", include_optional: bool = True) -> str:
    norm = normalize_lang_filter(lang_filter)
    base_dir = os.path.dirname(os.path.abspath(md_file))
    with open(md_file, "r", encoding="utf-8") as f:
        raw_lines = f.read().splitlines()

    out: List[str] = []
    in_fence = False
    fence_marker = ""
    skip_until_level: Optional[int] = None
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


def load_markdown_source(
    source_path: str = DEFAULT_SOURCE_PATH,
    lang_filter: str = "all",
    include_optional: bool = True,
) -> str:
    md_files = iter_markdown_files(source_path, lang_filter=lang_filter)
    if not md_files:
        raise FileNotFoundError(f"No se hallaron archivos .md en '{source_path}' para lang='{lang_filter}'")
    combined = "\n\n".join(
        expand_markdown_file(p, lang_filter=lang_filter, include_optional=include_optional).rstrip()
        for p in md_files
    )
    combined = re.sub(r"^\s*-{3,}\s*\n+", "", combined)
    return combined.strip() + "\n"


def extract_snippets(
    md_path: str = DEFAULT_SOURCE_PATH,
    lang_filter: str = "all",
    include_optional: bool = True,
) -> List[Snippet]:
    lines = load_markdown_source(
        md_path, lang_filter=lang_filter, include_optional=include_optional
    ).splitlines(keepends=True)

    snippets: List[Snippet] = []
    current_sec_num = "0.0"
    current_sec_title = "Preamble"
    current_heading = "Preamble"
    sec_block_counts: Dict[str, int] = {}
    used_ids: Dict[str, int] = {}

    in_fence = False
    fence_marker = ""
    fence_lang = ""
    fence_lines: List[str] = []

    heading_re = re.compile(r"^(#{2,4})\s+(.*)$")
    sec_num_re = re.compile(r"^(\d+\.\d+(?:\.\d+)?)\s+(.*)$")

    for line in lines:
        if not in_fence:
            m_head = heading_re.match(line.strip())
            if m_head:
                level_str, title = m_head.groups()
                current_heading = title.strip()
                if len(level_str) in (3, 4):
                    m_sec = sec_num_re.match(current_heading)
                    if m_sec:
                        current_sec_num = m_sec.group(1)
                        current_sec_title = m_sec.group(2)
                    elif len(level_str) == 3:
                        current_sec_num = slugify(current_heading)[:16]
                        current_sec_title = current_heading
                continue

            m_fence = re.match(r"^(`{3,}|~{3,})\s*(\S*).*$", line.strip())
            if m_fence:
                in_fence = True
                fence_marker = m_fence.group(1)
                fence_lang = (m_fence.group(2) or "text").lower()
                fence_lines = []
                continue
        else:
            if line.strip().startswith(fence_marker) and len(line.strip()) == len(fence_marker):
                in_fence = False
                code = "".join(fence_lines).rstrip()
                idx = sec_block_counts.get(current_sec_num, 0)
                sec_block_counts[current_sec_num] = idx + 1

                symbols = detect_symbols(code) if fence_lang in ("cpp", "c++", "python", "py") else []
                role = classify_block(code, current_sec_num)

                if role == "template":
                    base_id = "cpp_template_base"
                else:
                    mapped_id = None
                    for sym in symbols:
                        if sym in CANONICAL_SYMBOL_IDS:
                            mapped_id = CANONICAL_SYMBOL_IDS[sym]
                            break
                    if mapped_id:
                        base_id = mapped_id
                    elif symbols:
                        base_id = f"sec_{current_sec_num.replace('.', '_')}_{slugify(symbols[0])}"
                    else:
                        base_id = f"sec_{current_sec_num.replace('.', '_')}_{role}_{idx}"

                if base_id in used_ids:
                    used_ids[base_id] += 1
                    final_id = f"{base_id}_{used_ids[base_id]}"
                else:
                    used_ids[base_id] = 1
                    final_id = base_id

                deps: List[str] = []
                for sym in symbols:
                    c_id = CANONICAL_SYMBOL_IDS.get(sym)
                    if c_id and c_id in IMPLICIT_DEPENDENCIES:
                        for d in IMPLICIT_DEPENDENCIES[c_id]:
                            if d not in deps:
                                deps.append(d)

                reusable = strip_main_and_demos(code) if fence_lang in ("cpp", "c++") else code

                snippets.append(
                    Snippet(
                        id=final_id,
                        section_num=current_sec_num,
                        section_title=current_sec_title,
                        heading_title=current_heading,
                        lang=fence_lang,
                        role=role,
                        code=code,
                        reusable_code=reusable,
                        dependencies=deps,
                        symbols=symbols,
                        block_index_in_section=idx,
                    )
                )
            else:
                fence_lines.append(line)

    return snippets


def find_snippet_by_symbol_or_id(snippets: List[Snippet], query: str) -> Optional[Snippet]:
    # 1. Exact ID match
    for s in snippets:
        if s.id == query:
            return s
    # 2. Symbol match
    candidate_symbols = [query] + ID_TO_SYMBOLS.get(query, [])
    for sym in candidate_symbols:
        for s in snippets:
            if sym in s.symbols and s.role in ("algorithm", "template"):
                return s
    # 3. Prefix ID match
    for s in snippets:
        if s.id.startswith(query) and s.role in ("algorithm", "template"):
            return s
    # 4. Whole-word regex match in reusable_code
    for sym in candidate_symbols:
        for s in snippets:
            if re.search(rf"\b{re.escape(sym)}\b", s.reusable_code) and s.role in ("algorithm", "template"):
                return s
    return None


def get_base_template(snippets: List[Snippet]) -> str:
    tpl = find_snippet_by_symbol_or_id(snippets, "cpp_template_base")
    if not tpl:
        raise RuntimeError("No se encontró el bloque cpp_template_base (§1.1) en Notebook_ICPC_Final.md.")
    return tpl.reusable_code


def assemble_cpp_program(
    snippets: List[Snippet],
    target_queries: List[str],
    driver_code: str,
) -> str:
    """
    Ensambla el template base + dependencias + bloques solicitados + driver_code.
    Evita duplicar bloques o structs ya incluidos y resuelve automáticamente
    símbolos top-level referenciados en el código o en el driver.
    """
    defined_structs: Set[str] = set()
    base_code = remove_already_defined_structs(get_base_template(snippets), defined_structs)
    parts = [base_code]
    included_ids = {"cpp_template_base"}
    visiting_ids: Set[str] = set()

    # Mapa de cada símbolo top-level único al ID del snippet que lo define
    sym_to_snip_id: Dict[str, str] = {}
    for s in snippets:
        if s.role == "algorithm" and s.lang in ("cpp", "c++"):
            for sym in s.symbols:
                if len(sym) >= 3 and sym not in sym_to_snip_id:
                    sym_to_snip_id[sym] = s.id

    def add_query(q: str):
        snip = find_snippet_by_symbol_or_id(snippets, q)
        if not snip:
            raise KeyError(f"No se encontró el snippet o símbolo '{q}' en el Markdown.")
        if snip.id in included_ids or snip.id in visiting_ids:
            return
        visiting_ids.add(snip.id)
        for dep in snip.dependencies:
            add_query(dep)
        # Detectar símbolos top-level de otros snippets usados dentro de snip.reusable_code (ignorando comentarios)
        code_no_comments = re.sub(r"//[^\n]*", "", snip.reusable_code)
        for sym, owner_id in sym_to_snip_id.items():
            if owner_id != snip.id and owner_id not in included_ids and owner_id not in visiting_ids:
                if re.search(rf"\b{re.escape(sym)}\b", code_no_comments):
                    add_query(owner_id)
        visiting_ids.remove(snip.id)
        included_ids.add(snip.id)
        clean_reusable = remove_already_defined_structs(snip.reusable_code, defined_structs)
        parts.append(f"// --- Snippet: {snip.id} (§{snip.section_num}) ---\n{clean_reusable}")

    for q in target_queries:
        add_query(q)

    # Detectar símbolos top-level usados directamente en el driver
    for sym, owner_id in sym_to_snip_id.items():
        if owner_id not in included_ids and re.search(rf"\b{re.escape(sym)}\b", driver_code):
            add_query(owner_id)

    parts.append("// --- Driver ---\n" + driver_code)
    return "\n\n".join(parts) + "\n"


def main():
    parser = argparse.ArgumentParser(description="Extractor de snippets del Notebook ICPC")
    parser.add_argument("--input", "-i", default=DEFAULT_SOURCE_PATH, help="Directorio modular (sections/) o archivo Markdown")
    parser.add_argument("--json", "-j", help="Guardar catálogo de snippets en JSON")
    parser.add_argument("--list", "-l", action="store_true", help="Listar todos los snippets extraídos")
    args = parser.parse_args()

    snippets = extract_snippets(args.input)
    if args.list:
        for s in snippets:
            syms = ",".join(s.symbols[:4])
            print(f"{s.id:36s} | §{s.section_num:5s} | {s.lang:6s} | {s.role:12s} | {syms}")
        print(f"\nTotal bloques extraídos: {len(snippets)}")

    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump([dataclasses.asdict(s) for s in snippets], f, indent=2, ensure_ascii=False)
        print(f"Catálogo guardado en {args.json}")


if __name__ == "__main__":
    main()
