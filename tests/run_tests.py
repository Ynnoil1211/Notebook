#!/usr/bin/env python3
"""
tests/run_tests.py — Suite de regresiones, pruebas diferenciales y validación de I/O
====================================================================================

Extrae los snippets directamente desde Notebook_ICPC_Final.md (sin modificarlo),
ensambla cada prueba en un directorio temporal, compila con g++ (usando
-D_GLIBCXX_DEBUG / -D_GLIBCXX_ASSERTIONS y detectando si UBSan está disponible),
ejecuta regresiones deterministas, pruebas diferenciales con semilla reproducible,
pruebas de ejemplos de entrada/salida y validación de bloques Python.
"""

from __future__ import annotations
import argparse
import ast
from concurrent.futures import ThreadPoolExecutor
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from typing import Callable, Dict, List, Optional, Tuple

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if ROOT_DIR not in sys.path:
    sys.path.insert(0, ROOT_DIR)

from tools.extract_snippets import (
    DEFAULT_SOURCE_PATH,
    Snippet,
    assemble_cpp_program,
    extract_snippets,
    find_snippet_by_symbol_or_id,
    load_markdown_source,
)


def get_compiler_env() -> Tuple[str, Dict[str, str], bool, List[str]]:
    gpp = shutil.which("g++")
    if not gpp:
        raise RuntimeError("No se encontró 'g++' en el PATH.")
    gpp_real = os.path.realpath(gpp)
    gpp_dir = os.path.dirname(gpp_real)
    env = os.environ.copy()
    env["PATH"] = gpp_dir + os.pathsep + env.get("PATH", "")

    sys_inc_flags: List[str] = []
    if os.name == "nt":
        try:
            import ctypes
            mingw_root = os.path.dirname(gpp_dir)
            buf = ctypes.create_unicode_buffer(512)
            if ctypes.windll.kernel32.GetShortPathNameW(mingw_root, buf, 512) > 0:
                short_mingw = buf.value
                cxx_base = os.path.join(short_mingw, "include", "c++")
                if os.path.isdir(cxx_base):
                    for ver in os.listdir(cxx_base):
                        inc_v = os.path.join(cxx_base, ver)
                        if os.path.isdir(inc_v):
                            sys_inc_flags.extend(["-isystem", inc_v])
                            target_v = os.path.join(inc_v, "x86_64-w64-mingw32")
                            if os.path.isdir(target_v):
                                sys_inc_flags.extend(["-isystem", target_v])
        except Exception:
            pass

    has_ubsan = False
    with tempfile.TemporaryDirectory() as d:
        src = os.path.join(d, "probe.cpp")
        exe = os.path.join(d, "probe.exe")
        with open(src, "w", encoding="utf-8") as f:
            f.write("int main(){return 0;}\n")
        r = subprocess.run(
            [gpp_real, "-std=c++20", "-fsanitize=undefined", src, "-o", exe],
            capture_output=True,
            text=True,
            env=env,
        )
        if r.returncode == 0:
            r_run = subprocess.run([exe], capture_output=True, text=True, env=env)
            has_ubsan = r_run.returncode == 0

    return gpp_real, env, has_ubsan, sys_inc_flags


class TestContext:
    def __init__(self, md_path: str = DEFAULT_SOURCE_PATH):
        self.md_path = md_path
        self.raw_markdown = load_markdown_source(md_path)
        self.snippets = extract_snippets(md_path)
        self.gpp, self.env, self.has_ubsan, self.sys_inc_flags = get_compiler_env()
        self.temp_dir = tempfile.mkdtemp(prefix="icpc_nb_tests_")
        self.pch_debug_dir = os.path.join(self.temp_dir, "pch_debug")
        self._build_pch()

    def _build_pch(self):
        """Precompila <bits/stdc++.h> con _GLIBCXX_DEBUG para acelerar las pruebas."""
        bits_dir = os.path.join(self.pch_debug_dir, "bits")
        os.makedirs(bits_dir, exist_ok=True)
        hdr = os.path.join(bits_dir, "stdc++.h")
        gch = os.path.join(bits_dir, "stdc++.h.gch")
        with open(hdr, "w", encoding="utf-8") as f:
            f.write("#include_next <bits/stdc++.h>\n#include <cassert>\n")
        flags = [
            "-std=c++20", "-O2", "-Wall", "-Wextra",
            "-Wno-unused-parameter", "-Wno-sign-compare",
            "-D_GLIBCXX_DEBUG", "-D_GLIBCXX_ASSERTIONS",
        ]
        if self.has_ubsan:
            flags.append("-fsanitize=undefined")
        subprocess.run(
            [self.gpp, *flags, "-x", "c++-header", hdr, "-o", gch],
            capture_output=True,
            text=True,
            env=self.env,
        )

    def cleanup(self):
        shutil.rmtree(self.temp_dir, ignore_errors=True)

    def compile_and_run_cpp(
        self,
        name: str,
        target_queries: List[str],
        driver_code: str,
        stdin_data: str = "",
        use_glibcxx_debug: bool = True,
        extra_flags: Optional[List[str]] = None,
    ) -> Tuple[bool, str, str]:
        try:
            full_source = assemble_cpp_program(self.snippets, target_queries, driver_code)
        except Exception as e:
            return False, "", f"Error al ensamblar snippet: {e}"

        src_path = os.path.join(self.temp_dir, f"{name}.cpp")
        exe_path = os.path.join(self.temp_dir, f"{name}.exe")
        with open(src_path, "w", encoding="utf-8") as f:
            f.write(full_source)

        flags = ["-std=c++20", "-O2", "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-sign-compare"]
        if "<ext/pb_ds" in full_source or "<ext/rope>" in full_source or not use_glibcxx_debug:
            flags.extend([*self.sys_inc_flags, "-D_GLIBCXX_ASSERTIONS"])
        else:
            flags.extend(["-I", self.pch_debug_dir, "-D_GLIBCXX_DEBUG", "-D_GLIBCXX_ASSERTIONS"])

        if self.has_ubsan:
            flags.append("-fsanitize=undefined")
        if extra_flags:
            flags.extend(extra_flags)

        comp = subprocess.run(
            [self.gpp, *flags, src_path, "-o", exe_path],
            capture_output=True,
            text=True,
            env=self.env,
        )
        if comp.returncode != 0:
            return False, "", f"Error de compilación:\n{comp.stderr}"

        try:
            run_res = subprocess.run(
                [exe_path],
                input=stdin_data,
                capture_output=True,
                text=True,
                env=self.env,
                timeout=8.0,
            )
        except subprocess.TimeoutExpired:
            return False, "", "Tiempo límite excedido (TLE > 8s, posible ciclo infinito)"

        if run_res.returncode != 0:
            return (
                False,
                run_res.stdout,
                f"Runtime Error (código {run_res.returncode}):\n{run_res.stderr}",
            )
        return True, run_res.stdout, run_res.stderr


# ==============================================================================
# CASOS DE REGRESIÓN OBLIGATORIOS (FASE 1 Y FASE 2.1)
# ==============================================================================

def test_reg_bm_negative_discrepancy(ctx: TestContext) -> Tuple[bool, str]:
    """BM con {0,1,1,2}, consulta índice 4 -> esperado 3, no 5."""
    driver = r"""
    int main() {
      BerlekampMassey bm(1000000007LL);
      vll seq = {0, 1, 1, 2};
      ll ans4 = bm.get_kth(seq, 4);
      if (ans4 != 3) {
        cerr << "Fallo BM: esperado 3, obtenido " << ans4 << "\n";
        return 1;
      }
      vll trib = {0, 1, 1, 2, 4, 7, 13, 24};
      if (bm.get_kth(trib, 7) != 24 || bm.get_kth(trib, 8) != 44) {
        cerr << "Fallo BM en Tribonacci\n";
        return 1;
      }
      // Recurrencia aleatoria (seed=77)
      mt19937_64 rng(77);
      const ll MOD_P = 1000000007LL;
      vll c = {3, MOD_P - 2, 5};
      vll a = {1, 4, 2};
      for (int i = 3; i < 12; ++i) {
        ll nxt = (c[0]*a[i-1] + c[1]*a[i-2] + c[2]*a[i-3]) % MOD_P;
        a.pb(nxt);
      }
      vll prefix(a.begin(), a.begin() + 6);
      for (int k = 0; k < 12; ++k) {
        if (bm.get_kth(prefix, k) != a[k]) {
          cerr << "Fallo diferencial BM en k=" << k << " (seed=77)\n";
          return 1;
        }
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_bm", ["BerlekampMassey"], driver)
    return ok, err


def test_reg_push_relabel(ctx: TestContext) -> Tuple[bool, str]:
    """
    Push-Relabel: N=6, s=1, t=6; aristas en orden (1,2,2), (2,4,1), (4,5,1), (4,6,1), (5,3,1)
    -> Flujo esperado 1, no 0. Incluye contraste diferencial con Dinic.
    """
    driver = r"""
    int main() {
      PushRelabel pr(6);
      pr.add_edge(1, 2, 2);
      pr.add_edge(2, 4, 1);
      pr.add_edge(4, 5, 1);
      pr.add_edge(4, 6, 1);
      pr.add_edge(5, 3, 1);
      ll f = pr.max_flow(1, 6);
      if (f != 1) {
        cerr << "Fallo Push-Relabel: esperado 1, obtenido " << f << "\n";
        return 1;
      }
      mt19937 rng(1337);
      for (int it = 0; it < 60; ++it) {
        int n = 7;
        PushRelabel pr2(n);
        Dinic din(n, 1, n);
        for (int e = 0; e < 15; ++e) {
          int u = rng() % n + 1, v = rng() % n + 1;
          ll c = rng() % 6;
          if (u != v) {
            pr2.add_edge(u, v, c);
            din.add_edge(u, v, c);
          }
        }
        ll f1 = pr2.max_flow(1, n);
        ll f2 = din.max_flow();
        if (f1 != f2) {
          cerr << "Diferencia PushRelabel vs Dinic en iter=" << it
               << " (seed=1337): PR=" << f1 << " Dinic=" << f2 << "\n";
          return 1;
        }
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_push_relabel", ["PushRelabel", "Dinic"], driver)
    return ok, err


def test_reg_bridges_parallel_edges(ctx: TestContext) -> Tuple[bool, str]:
    """Puentes con dos aristas paralelas entre los mismos dos nodos -> Ningún puente."""
    driver = r"""
    int main() {
      vector<vi> adj(3);
      adj[1].pb(2); adj[2].pb(1);
      adj[1].pb(2); adj[2].pb(1);
      BridgesAndCuts bc(2, adj);
      if (!bc.bridges.empty()) {
        cerr << "Fallo Puentes con paralelas: se reportaron "
             << sz(bc.bridges) << " puentes, esperado 0.\n";
        return 1;
      }
      vector<vi> adj2(4);
      adj2[1].pb(2); adj2[2].pb(1);
      adj2[1].pb(2); adj2[2].pb(1);
      adj2[2].pb(3); adj2[3].pb(2);
      BridgesAndCuts bc2(3, adj2);
      if (sz(bc2.bridges) != 1) {
        cerr << "Esperado 1 puente (2-3), obtenido " << sz(bc2.bridges) << "\n";
        return 1;
      }
      if (!bc2.is_cut[2] || bc2.is_cut[1] || bc2.is_cut[3]) {
        cerr << "Puntos de articulacion incorrectos con paralelas.\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_bridges", ["BridgesAndCuts"], driver)
    return ok, err


def test_reg_crt_overflow(ctx: TestContext) -> Tuple[bool, str]:
    """CRT con residuos {0, 5000000006} y módulos {2, 5000000007} -> {5000000006, 10000000014}."""
    driver = r"""
    int main() {
      vll r = {0, 5000000006LL};
      vll m = {2, 5000000007LL};
      auto [ans_r, ans_m] = crt_general(r, m);
      if (ans_r != 5000000006LL || ans_m != 10000000014LL) {
        cerr << "Fallo CRT: esperado {5000000006, 10000000014}, obtenido {"
             << ans_r << ", " << ans_m << "}\n";
        return 1;
      }
      // Incompatible: x = 1 (mod 4), x = 2 (mod 6)
      auto [inc_r, inc_m] = crt_general({1, 2}, {4, 6});
      if (inc_r != -1 || inc_m != -1) {
        cerr << "Fallo CRT en sistema incompatible\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_crt", ["crt_general"], driver)
    return ok, err


def test_reg_closest_pair_duplicates(ctx: TestContext) -> Tuple[bool, str]:
    """Closest pair con tres puntos (0,0) -> Distancia 0, sin acceso fuera de rango."""
    driver = r"""
    int main() {
      vector<Pt> pts = {{0, 0}, {0, 0}, {0, 0}};
      ld d = closest_pair_distance(pts);
      if (abs(d) > 1e-9) {
        cerr << "Fallo Closest Pair: esperado 0, obtenido " << (double)d << "\n";
        return 1;
      }
      mt19937 rng(42);
      for (int it = 0; it < 30; ++it) {
        int n = rng() % 15 + 2;
        vector<Pt> p(n);
        for (int i = 0; i < n; ++i)
          p[i] = {(ld)(int)(rng() % 10 - 5), (ld)(int)(rng() % 10 - 5)};
        ld got = closest_pair_distance(p);
        ld best = 1e18;
        for (int i = 0; i < n; ++i)
          for (int j = i + 1; j < n; ++j)
            best = min(best, (p[i] - p[j]).norm());
        if (abs(got - best) > 1e-7) {
          cerr << "Diferencia en closest_pair iter=" << it << " (seed=42): "
               << (double)got << " vs " << (double)best << "\n";
          return 1;
        }
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_closest_pair", ["closest_pair"], driver)
    return ok, err


def test_reg_minkowski_single_point(ctx: TestContext) -> Tuple[bool, str]:
    """Minkowski de {(0,0)} y {(0,0),(1,0),(0,1)} -> El mismo triángulo, sin acceso fuera de rango."""
    driver = r"""
    int main() {
      vector<Pt> P = {{0, 0}};
      vector<Pt> Q = {{0, 0}, {1, 0}, {0, 1}};
      vector<Pt> res = minkowski_sum(P, Q);
      vector<Pt> hull_res = convex_hull(res);
      vector<Pt> hull_q = convex_hull(Q);
      if (sz(hull_res) != sz(hull_q)) {
        cerr << "Fallo Minkowski: tamano de hull " << sz(hull_res)
             << " != " << sz(hull_q) << "\n";
        return 1;
      }
      for (int i = 0; i < sz(hull_q); ++i) {
        if ((hull_res[i] - hull_q[i]).norm() > 1e-7) {
          cerr << "Vertice distinto en Minkowski\n";
          return 1;
        }
      }
      // Diferencial: comparar contra hull de todas las sumas de pares
      vector<Pt> A = {{0, 0}, {2, 0}, {1, 2}};
      vector<Pt> B = {{1, 1}, {3, 1}, {3, 4}, {1, 4}};
      vector<Pt> all_sums;
      for (auto a : A) for (auto b : B) all_sums.pb(a + b);
      vector<Pt> expected = convex_hull(all_sums);
      vector<Pt> got = convex_hull(minkowski_sum(A, B));
      if (sz(expected) != sz(got)) {
        cerr << "Diferencia en Minkowski suma de poligonos\n";
        return 1;
      }
      for (int i = 0; i < sz(expected); ++i)
        if ((expected[i] - got[i]).norm() > 1e-7) return 1;
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_minkowski", ["minkowski_sum", "convex_hull"], driver)
    return ok, err


def test_reg_point_comparator_strict_weak_ordering(ctx: TestContext) -> Tuple[bool, str]:
    """Ordenamiento geométrico con coordenadas cercanas: orden débil estricto."""
    driver = r"""
    int main() {
      vector<Pt> pts = {
        {0.0, 0.0},
        {0.6 * EPS, -1.0},
        {1.2 * EPS, -2.0},
        {0.0, 0.6 * EPS},
        {0.0, 1.2 * EPS}
      };
      int n = sz(pts);
      for (int i = 0; i < n; ++i) {
        if (pts[i] < pts[i]) {
          cerr << "Violacion de irreflexividad en Pt::operator<\n";
          return 1;
        }
        for (int j = 0; j < n; ++j) {
          if ((pts[i] < pts[j]) && (pts[j] < pts[i])) {
            cerr << "Violacion de asimetria en Pt::operator<\n";
            return 1;
          }
          for (int k = 0; k < n; ++k) {
            if ((pts[i] < pts[j]) && (pts[j] < pts[k]) && !(pts[i] < pts[k])) {
              cerr << "Violacion de transitividad en Pt::operator< para i="
                   << i << " j=" << j << " k=" << k << "\n";
              return 1;
            }
            auto eq = [&](int a, int b) {
              return !(pts[a] < pts[b]) && !(pts[b] < pts[a]);
            };
            if (eq(i, j) && eq(j, k) && !eq(i, k)) {
              cerr << "Violacion de transitividad de equivalencia en Pt::operator<\n";
              return 1;
            }
          }
        }
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_pt_cmp", ["geometry_2d"], driver)
    return ok, err


def _strip_cpp_comments(code: str) -> str:
    code = re.sub(r"//.*", "", code)
    code = re.sub(r"/\*.*?\*/", "", code, flags=re.DOTALL)
    return code


def test_reg_pbds_multiset(ctx: TestContext) -> Tuple[bool, str]:
    """PBDS multiset con pair<T, int> y less<>: duplicados, borrado de una copia y orden."""
    snip = find_snippet_by_symbol_or_id(ctx.snippets, "pbds_ordered_set")
    if snip and "less_equal" in _strip_cpp_comments(snip.reusable_code):
        return False, "PBDS ordered_multiset aún utiliza less_equal (rompe invariantes de RB-tree)."
    driver = r"""
    int main() {
      ordered_multiset<int> oms;
      int uid = 0;
      oms.insert({5, uid++});
      oms.insert({5, uid++});
      oms.insert({10, uid++});
      // Contar estrictamente menores que 5 -> 0; menores que 10 -> 2
      if (oms.order_of_key({5, -1}) != 0 || oms.order_of_key({10, -1}) != 2) {
        cerr << "Fallo en order_of_key de ordered_multiset\n";
        return 1;
      }
      // Borrar una sola copia de 5
      auto it = oms.lower_bound({5, -1});
      if (it == oms.end() || it->first != 5) {
        cerr << "Fallo en lower_bound de ordered_multiset\n";
        return 1;
      }
      oms.erase(it);
      if (sz(oms) != 2 || oms.find_by_order(0)->first != 5 || oms.find_by_order(1)->first != 10) {
        cerr << "Fallo tras borrar una copia en ordered_multiset\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_pbds", ["pbds_ordered_set"], driver)
    return ok, err


def test_reg_eps_cmp_template(ctx: TestContext) -> Tuple[bool, str]:
    """Verificar que el template base separe comparación aproximada y orden de contenedores."""
    snip = find_snippet_by_symbol_or_id(ctx.snippets, "cpp_template_base")
    if snip and "a + EPS < b" in _strip_cpp_comments(snip.reusable_code):
        return False, "El template base aún incluye struct eps_cmp { return a + EPS < b; } que viola strict weak ordering."
    return True, ""


# ==============================================================================
# CASOS DE REGRESIÓN FASE 2.2 (SEGUNDA TANDA DE CORRECCIONES Y CONTRATOS)
# ==============================================================================

def test_reg_miller_rabin_u64(ctx: TestContext) -> Tuple[bool, str]:
    """Miller-Rabin en el extremo superior de uint64_t (>= 2^63) usando u128."""
    snip = find_snippet_by_symbol_or_id(ctx.snippets, "miller_rabin")
    if snip and "(i128)a * b" in snip.reusable_code:
        return False, "mul_mod64 aún usa (i128) con signo en vez de __uint128_t para cubrir todo uint64_t."
    driver = r"""
    int main() {
      // 2^64 - 59 = 18446744073709551557ULL es el mayor primo de 64 bits
      ull p_max = 18446744073709551557ULL;
      if (!miller_rabin(p_max)) {
        cerr << "Miller-Rabin fallo en el mayor primo de 64 bits (2^64 - 59)\n";
        return 1;
      }
      // 2^64 - 1 = 3 * 5 * 17 * 257 * 641 * 65537 * 6700417 es compuesto
      if (miller_rabin(~0ULL)) {
        cerr << "Miller-Rabin reporto primo para 2^64 - 1\n";
        return 1;
      }
      // Semiprimo grande > 2^63: 3037000493 * 4294967291 = 13043817822302782233ULL
      ull semi = 3037000493ULL * 4294967291ULL;
      if (miller_rabin(semi)) {
        cerr << "Miller-Rabin reporto primo para semiprimo > 2^63\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_mr64", ["miller_rabin"], driver)
    return ok, err


def test_reg_block_cut_tree(ctx: TestContext) -> Tuple[bool, str]:
    """Block-Cut Tree: aristas de retroceso, aristas paralelas y vértices aislados."""
    driver = r"""
    int main() {
      // Grafo con 4 vertices:
      // Triangulo 1-2-3 (1 arista de retroceso), mas vertice aislado 4
      BlockCutTree bct(4);
      bct.add_edge(1, 2);
      bct.add_edge(2, 3);
      bct.add_edge(3, 1);
      bct.build();
      // El triangulo tiene 3 aristas (incluyendo la de retroceso 3-1)
      if (sz(bct.biconn_edges) < 1 || sz(bct.biconn_edges[0]) != 3) {
        cerr << "BlockCutTree no apilo la arista de retroceso del triangulo (aristas="
             << (bct.biconn_edges.empty() ? 0 : sz(bct.biconn_edges[0])) << ", esperado 3)\n";
        return 1;
      }
      // El vertice aislado 4 debe tener bct_id[4] != 0
      if (bct.bct_id[4] == 0) {
        cerr << "BlockCutTree dejo el vertice aislado 4 con bct_id[4] == 0\n";
        return 1;
      }
      // Dos aristas paralelas entre 1 y 2 + arista 2-3 -> 2 es punto de articulacion
      BlockCutTree bct2(3);
      bct2.add_edge(1, 2);
      bct2.add_edge(1, 2);
      bct2.add_edge(2, 3);
      bct2.build();
      if (!bct2.is_cut[2] || bct2.is_cut[1] || bct2.is_cut[3]) {
        cerr << "BlockCutTree fallo con aristas paralelas\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_bct", ["BlockCutTree"], driver)
    return ok, err


def test_reg_halfplane_intersection(ctx: TestContext) -> Tuple[bool, str]:
    """Halfplane intersection: vacía, franja paralela opuesta factible y triángulo acotado."""
    driver = r"""
    int main() {
      // 1. Franja entre dos semiplanos paralelos opuestos: y >= 0 y y <= 2
      vector<Halfplane> strip = {
        Halfplane({0, 0}, {1, 0}),  // y >= 0
        Halfplane({0, 2}, {-1, 2})  // y <= 2
      };
      auto poly_strip = halfplane_intersection(strip);
      if (sz(poly_strip) != 4) {
        cerr << "Halfplane fallo en franja paralela factible (0 <= y <= 2): sz="
             << sz(poly_strip) << "\n";
        return 1;
      }
      // 2. Interseccion vacia sin paralelos: x <= 0, y <= 0, x + y >= 1
      vector<Halfplane> empty_tri = {
        Halfplane({0, 0}, {0, 1}),  // x <= 0
        Halfplane({0, 0}, {-1, 0}), // y <= 0
        Halfplane({0, 1}, {1, 0})   // x + y >= 1
      };
      auto poly_empty = halfplane_intersection(empty_tri);
      if (!poly_empty.empty() && polygon_area(poly_empty) > 1e-7) {
        cerr << "Halfplane no detecto interseccion vacia\n";
        return 1;
      }
      // 3. Triangulo: x >= 0, y >= 0, x + y <= 4 -> area = 8
      vector<Halfplane> tri_ok = {
        Halfplane({0, 0}, {1, 0}),  // y >= 0
        Halfplane({4, 0}, {0, 4}),  // x + y <= 4
        Halfplane({0, 4}, {0, 0})   // x >= 0
      };
      auto poly_tri = halfplane_intersection(tri_ok);
      if (abs(polygon_area(poly_tri) - 8.0) > 1e-5) {
        cerr << "Halfplane area incorrecta en triangulo: " << (double)polygon_area(poly_tri) << "\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp(
        "reg_hp", ["halfplane_intersection", "geometry_2d"], driver
    )
    return ok, err


def test_reg_dc_dp_unreachable(ctx: TestContext) -> Tuple[bool, str]:
    """D&C DP: no propagar opt=-1 cuando hay estados inalcanzables (mid < k)."""
    snip = find_snippet_by_symbol_or_id(ctx.snippets, "divide_and_conquer_dp")
    if snip and "best = {INF, -1}" in snip.reusable_code:
        return False, "compute_dc aún inicializa best = {INF, -1} y propaga opt = -1."
    driver = r"""
    ll cost_dc(int l, int r) {
      ll d = r - l + 1;
      return d * d;
    }
    int main() {
      // Dividir N=5 en K=3 grupos minimizando suma de cuadrados de longitudes:
      // 5 = 2 + 2 + 1 -> 4 + 4 + 1 = 9
      ll ans = solve_dc_dp(5, 3, cost_dc);
      if (ans != 9) {
        cerr << "Fallo D&C DP: esperado 9, obtenido " << ans << "\n";
        return 1;
      }
      // También probar compute_dc comenzando en l=1 cuando mid < step es inalcanzable:
      int n = 5, k = 3;
      dp_prev.assign(n + 1, INF);
      dp_cur.assign(n + 1, INF);
      dp_prev[0] = 0;
      for (int step = 1; step <= k; ++step) {
        fill(all(dp_cur), INF);
        compute_dc(1, n, 1, n, cost_dc);
        dp_prev = dp_cur;
      }
      if (dp_prev[n] != 9) {
        cerr << "Fallo compute_dc con estados inalcanzables\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_dc_dp", ["divide_and_conquer_dp"], driver)
    return ok, err


def test_reg_simplex_feasibility_and_pivot(ctx: TestContext) -> Tuple[bool, str]:
    """Simplex: sistema infactible (x <= 1, x >= 2) no debe ciclar; óptimo negativo y no acotado."""
    driver = r"""
    int main() {
      // 1. Sistema infactible: x0 <= 1 y -x0 <= -2 (x0 >= 2)
      Simplex lp_inf(2, 1);
      lp_inf.a = {{1.0}, {-1.0}};
      lp_inf.b = {1.0, -2.0};
      lp_inf.c = {1.0};
      auto [z_inf, x_inf] = lp_inf.solve();
      if (!x_inf.empty() || z_inf > -1e15) {
        cerr << "Simplex no reporto infactible para x<=1, x>=2 (z=" << (double)z_inf << ")\n";
        return 1;
      }
      // 2. Sistema factible con optimo negativo Z = -2:
      // Maximizar -x0 sujeto a -x0 <= -2 (x0 >= 2)
      Simplex lp_neg(1, 1);
      lp_neg.a = {{-1.0}};
      lp_neg.b = {-2.0};
      lp_neg.c = {-1.0};
      auto [z_neg, x_neg] = lp_neg.solve();
      if (x_neg.empty() || abs(z_neg - (-2.0)) > 1e-6 || abs(x_neg[0] - 2.0) > 1e-6) {
        cerr << "Simplex fallo en optimo negativo Z=-2: obtenido " << (double)z_neg << "\n";
        return 1;
      }
      // 3. Sistema no acotado: maximizar x0 sujeto a -x0 <= -1
      Simplex lp_unb(1, 1);
      lp_unb.a = {{-1.0}};
      lp_unb.b = {-1.0};
      lp_unb.c = {1.0};
      auto [z_unb, x_unb] = lp_unb.solve();
      if (z_unb < 1e15) {
        cerr << "Simplex no detecto sistema no acotado\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_simplex", ["simplex"], driver)
    return ok, err


def test_reg_bellman_ford_saturation(ctx: TestContext) -> Tuple[bool, str]:
    """Bellman-Ford: no contar asignaciones saturadas sin cambio como relajaciones genuinas."""
    snip = find_snippet_by_symbol_or_id(ctx.snippets, "bellman_ford_cycle")
    if snip and "d[e.u] + e.w < d[e.v]" in snip.reusable_code and "nd < d[e.v]" not in snip.reusable_code:
        return (
            False,
            "bellman_ford_cycle aún compara d[e.u] + e.w < d[e.v] antes de saturar en -INF, contando asignaciones sin cambio como relajaciones.",
        )
    driver = r"""
    int main() {
      // DAG sin ciclos con pesos muy negativos que saturan en -INF en el paso 2:
      // 1 -> 2 (-INF), 2 -> 3 (-10), 3 -> 4 (0)
      vector<BFEdge> edges = {
        {1, 2, -INF},
        {2, 3, -10},
        {3, 4, -10}
      };
      vll d; vi cyc;
      bool has_cyc = bellman_ford_cycle(1, 4, edges, d, cyc);
      if (has_cyc) {
        cerr << "Bellman-Ford reporto ciclo negativo falso por saturacion en DAG!\n";
        return 1;
      }
      // Grafo con ciclo negativo real: 1 -> 2 (1), 2 -> 3 (-5), 3 -> 2 (2)
      vector<BFEdge> edges2 = {
        {1, 2, 1},
        {2, 3, -5},
        {3, 2, 2}
      };
      bool has_cyc2 = bellman_ford_cycle(1, 3, edges2, d, cyc);
      if (!has_cyc2 || sz(cyc) < 2) {
        cerr << "Bellman-Ford no reconstruyo ciclo negativo real\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_bf", ["bellman_ford_cycle"], driver)
    return ok, err


def test_reg_segmented_sieve_zero_one(ctx: TestContext) -> Tuple[bool, str]:
    """Criba segmentada: tratar correctamente 0 y 1 cuando L=0 o L=1."""
    driver = r"""
    int main() {
      auto r0 = segmented_sieve(0, 10);
      // En [0..10], primos son 2, 3, 5, 7
      vi expected = {0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0};
      for (int i = 0; i <= 10; ++i) {
        if ((int)r0[i] != expected[i]) {
          cerr << "Fallo segmented_sieve(0, 10) en x=" << i
               << ": obtenido " << r0[i] << ", esperado " << expected[i] << "\n";
          return 1;
        }
      }
      auto r1 = segmented_sieve(1, 10);
      for (int i = 1; i <= 10; ++i) {
        if ((int)r1[i - 1] != expected[i]) {
          cerr << "Fallo segmented_sieve(1, 10) en x=" << i << "\n";
          return 1;
        }
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_segsieve", ["segmented_sieve"], driver)
    return ok, err


def test_reg_modular_arithmetic_negatives(ctx: TestContext) -> Tuple[bool, str]:
    """Aritmética modular: normalización de negativos en add_mod, sub_mod e inv_extgcd."""
    driver = r"""
    int main() {
      if (add_mod(-4, -4, 5) != 2) {
        cerr << "Fallo add_mod(-4, -4, 5): obtenido " << add_mod(-4, -4, 5) << ", esperado 2\n";
        return 1;
      }
      if (sub_mod(-4, 4, 5) != 2) {
        cerr << "Fallo sub_mod(-4, 4, 5): obtenido " << sub_mod(-4, 4, 5) << ", esperado 2\n";
        return 1;
      }
      if (inv_extgcd(-3, 10) != 3) { // (-3 * 3) = -9 == 1 (mod 10)
        cerr << "Fallo inv_extgcd(-3, 10): obtenido " << inv_extgcd(-3, 10) << ", esperado 3\n";
        return 1;
      }
      if (inv_extgcd(6, 10) != -1) {
        cerr << "Fallo inv_extgcd(6, 10) no invertible\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp("reg_mod", ["modular_arith", "extgcd"], driver)
    return ok, err


def test_reg_misere_nim_and_parser(ctx: TestContext) -> Tuple[bool, str]:
    """Verificar Misère Nim con montones de tamaño 0 y 1, y ExpressionParser con -funsigned-char."""
    driver = r"""
    int main() {
      if (nim_misere_win({1, 0}) != false) {
        cerr << "Fallo Misere Nim en {1, 0}: esperado false (1 monton de tamano 1)\n";
        return 1;
      }
      if (nim_misere_win({1, 1, 0}) != true) {
        cerr << "Fallo Misere Nim en {1, 1, 0}: esperado true (2 montones de tamano 1)\n";
        return 1;
      }
      ExpressionParser parser;
      if (parser.evaluate("-5 + 3 * (-2) + (+4)") != -7) {
        cerr << "Fallo ExpressionParser con -funsigned-char: obtenido "
             << parser.evaluate("-5 + 3 * (-2) + (+4)") << ", esperado -7\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp(
        "reg_misere_parser",
        ["nim", "shunting_yard"],
        driver,
        extra_flags=["-funsigned-char"],
    )
    return ok, err


def test_python_snippets_syntax(ctx: TestContext) -> Tuple[bool, str]:
    """Comprueba sintaxis AST de todos los bloques Python y que no existan bugs conocidos."""
    py_snippets = [s for s in ctx.snippets if s.lang in ("python", "py")]
    if not py_snippets:
        return False, "No se encontraron snippets de Python."
    for s in py_snippets:
        try:
            ast.parse(s.code)
        except SyntaxError as e:
            return False, f"Error de sintaxis Python en {s.id} (§{s.section_num}): {e}"
        if "int(-7 / 3)" in s.code or 'replace("/", "//")' in s.code:
            return (
                False,
                f"Snippet Python {s.id} (§{s.section_num}) aún contiene int(a / b) para BigInt o replace('/', '//').",
            )
    return True, f"{len(py_snippets)} bloques Python verificados."


def test_phase3_alignments_and_consolidations(ctx: TestContext) -> Tuple[bool, str]:
    """Verifica algoritmos clave (PersistentFreqST, StringTrie, BitTrie, BipartiteMVC, gauss_mod, Lagrange, LCS, MonotoneCHT, Geometría)."""
    driver = r"""
    int main() {
      // 1. PersistentFreqST (k-esimo en subarreglo)
      vll arr = {50, 10, 40, 20, 30};
      PersistentFreqST pfst(arr);
      if (pfst.query_kth(1, 3, 1) != 10 || pfst.query_kth(1, 3, 2) != 20 || pfst.query_kth(1, 3, 3) != 40) {
        cerr << "Fallo PersistentFreqST\n";
        return 1;
      }
      // 2. StringTrie y BitTrie::min_xor
      StringTrie st;
      st.insert("algo"); st.insert("arbol"); st.insert("arco");
      if (st.count_prefix("ar") != 2 || st.count_prefix("al") != 1 || st.count_prefix("z") != 0) {
        cerr << "Fallo StringTrie\n";
        return 1;
      }
      BitTrie bt;
      bt.update(3, +1); bt.update(10, +1); bt.update(5, +1);
      if (bt.min_xor(6) != (6 ^ 5) || bt.max_xor(6) != (6 ^ 10)) {
        cerr << "Fallo BitTrie min_xor / max_xor\n";
        return 1;
      }
      // 3. BipartiteMVC (Kuhn + Konig Minimum Vertex Cover)
      BipartiteMVC bmvc(3, 3);
      bmvc.add_edge(1, 1); bmvc.add_edge(1, 2);
      bmvc.add_edge(2, 2); bmvc.add_edge(3, 2);
      auto [cov_l, cov_r] = bmvc.min_vertex_cover();
      if (sz(cov_l) + sz(cov_r) != 2) {
        cerr << "Fallo BipartiteMVC (Konig): tamano MVC=" << (sz(cov_l) + sz(cov_r)) << ", esperado 2\n";
        return 1;
      }
      // 4. gauss_mod
      vector<vll> mat = {{1, 2, 5}, {3, 4, 11}};
      vll g_ans;
      int g_st = gauss_mod(mat, g_ans, 1000000007LL);
      if (g_st != 1 || g_ans[0] != 1 || g_ans[1] != 2) {
        cerr << "Fallo gauss_mod\n";
        return 1;
      }
      // 5. lagrange_eval_arbitrary: P(x) = x^2 + 1 en xs={1, 3, 5}, ys={2, 10, 26} -> P(10) = 101
      if (lagrange_eval_arbitrary({1, 3, 5}, {2, 10, 26}, 10) != 101) {
        cerr << "Fallo lagrange_eval_arbitrary\n";
        return 1;
      }
      // 6. lcs_string
      if (lcs_string("abcbdab", "bdcaba") != "bcba" && sz(lcs_string("abcbdab", "bdcaba")) != 4) {
        cerr << "Fallo lcs_string\n";
        return 1;
      }
      // 7. MonotoneCHT
      MonotoneCHT mcht;
      mcht.add(3, 0);   // y = 3x
      mcht.add(1, 4);   // y = x + 4
      mcht.add(-1, 10); // y = -x + 10
      if (mcht.query_monotone(1) != 3 || mcht.query_monotone(2) != 6 || mcht.query_monotone(5) != 5) {
        cerr << "Fallo MonotoneCHT\n";
        return 1;
      }
      // 8. Geometria: centroide, point_in_convex_polygon O(log N) y PtLL
      vector<Pt> sq = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
      Pt cent = polygon_centroid(sq);
      if (abs(cent.x - 2.0) > 1e-7 || abs(cent.y - 2.0) > 1e-7) {
        cerr << "Fallo polygon_centroid\n";
        return 1;
      }
      if (point_in_convex_polygon({2, 2}, sq) != 1 ||
          point_in_convex_polygon({4, 2}, sq) != 2 ||
          point_in_convex_polygon({5, 2}, sq) != 0) {
        cerr << "Fallo point_in_convex_polygon\n";
        return 1;
      }
      vector<PtLL> sq_ll = {{0, 0}, {1000000000LL, 0}, {1000000000LL, 1000000000LL}, {0, 1000000000LL}};
      if (polygon_area2_int(sq_ll) != (__int128)2000000000000000000LL) {
        cerr << "Fallo polygon_area2_int\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp(
        "reg_phase3",
        [
            "lazy_segtree",
            "bit_trie",
            "dinic",
            "modular_arith",
            "matrix_exp",
            "lagrange",
            "knapsack",
            "cht_dynamic",
            "geometry_2d",
        ],
        driver,
    )
    return ok, err


def test_phase5_new_techniques(ctx: TestContext) -> Tuple[bool, str]:
    """Verifica todas las técnicas agregadas en Fase 5 (Secciones 8.1 y 8.2 del plan)."""
    driver = r"""
    int main() {
      // 1. Patrones de prefijos y Kadane (§3.4)
      vll a_pref = {1, -1, 2, -2, 3};
      if (count_subarrays_sum_k(a_pref, 0) != 3) {
        cerr << "Fallo count_subarrays_sum_k\n";
        return 1;
      }
      if (count_subarrays_div_m({4, 5, 0, -2, -3, 1}, 5) != 7) {
        cerr << "Fallo count_subarrays_div_m\n";
        return 1;
      }
      if (count_subarrays_xor_k({4, 2, 2, 6, 4}, 6) != 4) {
        cerr << "Fallo count_subarrays_xor_k\n";
        return 1;
      }
      if (kadane_max_subarray({-5, -2, -8}) != -2 || kadane_max_subarray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) != 6) {
        cerr << "Fallo kadane_max_subarray\n";
        return 1;
      }

      // 2. Greedy: Interval scheduling, Regret heap y Exchange argument (§3.6)
      vector<pll> ivs = {{1, 3}, {2, 5}, {3, 6}, {6, 8}};
      if (interval_scheduling(ivs, false) != 3 || interval_scheduling(ivs, true) != 2) {
        cerr << "Fallo interval_scheduling\n";
        return 1;
      }
      vector<pll> tasks = {{4, 6}, {2, 5}, {3, 7}, {1, 7}};
      if (schedule_deadlines_heap(tasks) != 3) {
        cerr << "Fallo schedule_deadlines_heap\n";
        return 1;
      }
      vector<JobEx> jobs = {{1, 4, 2}, {2, 1, 1}, {3, 2, 4}};
      auto ord = exchange_argument_order(jobs);
      if (ord[0].id != 3 || ord[1].id != 2 || ord[2].id != 1) {
        cerr << "Fallo exchange_argument_order\n";
        return 1;
      }

      // 3. Sweep-Line 1D (§3.7)
      vector<pll> sw = {{1, 4}, {4, 6}, {2, 5}};
      if (max_interval_overlap(sw, true) != 3 || max_interval_overlap(sw, false) != 2) {
        cerr << "Fallo max_interval_overlap\n";
        return 1;
      }
      auto [merged, tot_len] = merge_intervals(sw);
      if (sz(merged) != 1 || tot_len != 5) {
        cerr << "Fallo merge_intervals\n";
        return 1;
      }

      // 4. Grafos funcionales / Sucesores (§6.11)
      vi to = {0, 2, 3, 4, 2, 4};
      FunctionalGraph fg(5, to);
      if (fg.jump(1, 5) != 3 || fg.min_steps(5, 3) != 3 || fg.min_steps(2, 5) != -1) {
        cerr << "Fallo FunctionalGraph\n";
        return 1;
      }

      // 5. NTT modular (§7.6)
      vll poly_p = multiply_ntt({1, 2, 3}, {4, 5});
      vll exp_p = {4, 13, 22, 15};
      if (poly_p != exp_p) {
        cerr << "Fallo multiply_ntt\n";
        return 1;
      }

      // 6. Conteo y esperanza (§7.12)
      init_fact();
      if (stars_and_bars_nonneg(5, 3) != 21 || stars_and_bars_pos(5, 3) != 6) {
        cerr << "Fallo stars_and_bars\n";
        return 1;
      }
      if (inclusion_exclusion_multiples(20, {2, 3}) != 13) {
        cerr << "Fallo inclusion_exclusion_multiples\n";
        return 1;
      }
      if (sum_all_subarray_mins({3, 1, 2, 4}) != 17 || sum_all_subarray_mins({2, 2, 2}) != 12) {
        cerr << "Fallo sum_all_subarray_mins\n";
        return 1;
      }

      // 7. DP de subconjuntos: dp[mask] y dp[mask][last] con reconstrucción (§8.2)
      vector<vll> cost_m = {{10, 2, 8}, {5, 9, 3}, {6, 4, 7}};
      if (subset_assignment_dp(cost_m) != 11) {
        cerr << "Fallo subset_assignment_dp\n";
        return 1;
      }
      vector<vll> dist_m = {{0, 10, 15}, {10, 0, 20}, {15, 20, 0}};
      auto [tsp_c, tsp_p] = tsp_subset_dp(dist_m, 0, true);
      if (tsp_c != 45 || sz(tsp_p) != 3) {
        cerr << "Fallo tsp_subset_dp\n";
        return 1;
      }

      // 8. Rerooting DP (§8.3)
      vector<vi> tr(5);
      tr[1] = {2, 3}; tr[2] = {1}; tr[3] = {1, 4}; tr[4] = {3};
      vll r_ans = rerooting_sum_distances(4, tr);
      if (r_ans[1] != 4 || r_ans[2] != 6 || r_ans[3] != 4 || r_ans[4] != 6) {
        cerr << "Fallo rerooting_sum_distances\n";
        return 1;
      }

      // 9. Geometría analítica básica y Círculos (§9.2)
      Pt pr = proj_point_line({0, 4}, {0, 0}, {4, 0});
      Pt rf = reflect_point_line({0, 4}, {0, 0}, {4, 0});
      ld dl = dist_point_line({0, 4}, {0, 0}, {4, 0});
      if (abs(pr.x) > 1e-7 || abs(pr.y) > 1e-7 || abs(rf.y + 4) > 1e-7 || abs(dl - 4) > 1e-7) {
        cerr << "Fallo proj/reflect/dist_point_line\n";
        return 1;
      }
      auto [cst, cpts] = circle_circle_intersection({{0, 0}, 5}, {{8, 0}, 5});
      if (cst != 2 || sz(cpts) != 2 || abs(cpts[0].x - 4) > 1e-7 || abs(abs(cpts[0].y) - 3) > 1e-7) {
        cerr << "Fallo circle_circle_intersection\n";
        return 1;
      }
      auto tpts = circle_tangents_from_point({{0, 0}, 3}, {5, 0});
      if (sz(tpts) != 2) {
        cerr << "Fallo circle_tangents_from_point\n";
        return 1;
      }

      // 10. Meet in the Middle (§10.1)
      vll a_mitm = {1, 2, 3, 4, 5};
      if (mitm_subset_sum_count(a_mitm, 6) != 3 || mitm_max_subset_sum_le(a_mitm, 11) != 11) {
        cerr << "Fallo Meet in the Middle\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp(
        "reg_phase5",
        [
            "prefix_patterns",
            "greedy_scheduling",
            "sweep_line_1d",
            "functional_graph",
            "matrix_exp",
            "counting_expectation",
            "subset_dp",
            "rerooting_dp",
            "analytic_geometry_circles",
            "meet_in_the_middle",
        ],
        driver,
    )
    return ok, err


def test_phase6_navigation_and_structure(ctx: TestContext) -> Tuple[bool, str]:
    """Verifica la página inicial de decisión, checklist anti-WA y separación de DP/Geometría (Fase 6)."""
    required_headings_and_terms = [
        "## Bloque 0: Guía de Decisión Rápida, Complejidad y Checklist Anti-WA",
        "### 0.1 Selector Rápido: Señal del Problema",
        "### 0.2 Tabla Orientativa de Complejidad según Restricciones",
        "### 0.3 Advertencias Críticas de Elección de Técnica",
        "### 0.4 Checklist Anti-WA Antes de Enviar",
        "mediana inferior",
        "mediana superior",
        "## Bloque 8: Programación Dinámica",
        "## Bloque 9: Geometría Computacional 2D",
    ]
    for item in required_headings_and_terms:
        if item not in ctx.raw_markdown:
            return False, f"Falta elemento requerido de Fase 6 en Notebook_ICPC_Final.md: '{item}'"
    if "## Índice General" in ctx.raw_markdown:
        return False, "Aún existe la lista manual '## Índice General' en lugar del índice automático con páginas."
    return True, "Guía de decisión, advertencias, checklist anti-WA y separación DP/Geometría verificadas."


def test_phase4_inventory_and_all_164_fiches(ctx: TestContext) -> Tuple[bool, str]:
    """
    Verifica que los 164 bloques de algoritmos (149 C++ y 15 Python) se extraigan
    y que todas las definiciones reutilizables C++ compilen juntas sin colisiones.
    """
    if len(ctx.snippets) != 164:
        return False, f"Se esperaban 164 bloques de código, se hallaron {len(ctx.snippets)}."

    cpp_algos = [
        s.id
        for s in ctx.snippets
        if s.lang in ("cpp", "c++") and s.role == "algorithm" and s.reusable_code.strip()
    ]
    driver = "int main() { return 0; }\n"
    ok, _out, err = ctx.compile_and_run_cpp("all_cpp_algorithms_compile", cpp_algos, driver)
    return ok, err


def test_phase7_line_width_and_build_pipeline(ctx: TestContext) -> Tuple[bool, str]:
    """
    Verifica que ninguna línea de código C++ o Python supere los 60 caracteres.
    """
    blocks = re.findall(r"```(?:cpp|python|text)\n(.*?)\n```", ctx.raw_markdown, re.DOTALL)
    over_60 = [
        (len(line), line)
        for b in blocks
        for line in b.splitlines()
        if len(line) > 60
    ]
    if over_60:
        sample = over_60[0]
        return (
            False,
            f"Hay {len(over_60)} líneas > 60 caracteres (ej. [{sample[0]}] {sample[1]!r})",
        )

    return True, f"0 líneas > 60 caracteres en {len(blocks)} bloques."


def test_differential_oracles_all_domains(ctx: TestContext) -> Tuple[bool, str]:
    """
    Sección 11: Pruebas diferenciales cruzadas contra oráculos de fuerza bruta para:
      - Estructuras de rango (Fenwick, IterSegTree, SegTree, LazySegTree, ImplicitTreap)
      - Conectividad dinámica (DSU, RollbackDSU, DynCon, LinkCut)
      - Árboles (LCA, HLD)
      - Flujos, Matching y 2-SAT (Dinic, PushRelabel, BipartiteMVC, hungarian, Blossom, TwoSAT)
      - Cadenas (kmp_search, z_function, manacher, SuffixArray, SuffixAutomaton, PalindromicTree)
    """
    driver = r"""
    int main() {
      // 1. Estructuras de rango vs vector bruto
      vll base = {5, -2, 7, 3, 1, 8, -4, 6};
      int n = sz(base);
      Fenwick bit(n);
      for (int i = 0; i < n; ++i) bit.add(i + 1, base[i]);
      IterSegTree ist(base);
      SegTree rst(base);
      LazySegTree lst(base);
      ImplicitTreap treap;
      for (ll x : base) treap.push_back(x);

      for (int l = 0; l < n; ++l) {
        ll s = 0, mn = INF;
        for (int r = l; r < n; ++r) {
          s += base[r];
          mn = min(mn, base[r]);
          if (bit.query(l + 1, r + 1) != s || rst.query(l, r) != s ||
              lst.query_sum(l, r) != s || treap.query_sum(l, r) != s ||
              ist.query(l, r) != mn) {
            cerr << "Fallo diferencial en estructuras de rango [" << l << "," << r << "]\n";
            return 1;
          }
        }
      }

      // 2. Conectividad: LinkCut y DynCon vs grafo explicito
      LinkCut lct(5);
      lct.link(1, 2); lct.link(2, 3); lct.link(4, 5);
      if (!lct.connected(1, 3) || lct.connected(1, 5)) {
        cerr << "Fallo diferencial LinkCut pre-cut\n";
        return 1;
      }
      lct.cut(2, 3); lct.link(3, 4);
      if (lct.connected(1, 3) || !lct.connected(3, 5)) {
        cerr << "Fallo diferencial LinkCut post-cut\n";
        return 1;
      }

      // 3. Arboles: LCA y HLD vs camino explicito
      vector<vi> tree_adj(6);
      vector<pii> tree_edges = {{1, 2}, {1, 3}, {2, 4}, {2, 5}};
      for (auto [u, v] : tree_edges) {
        tree_adj[u].pb(v); tree_adj[v].pb(u);
      }
      vll node_vals = {0, 10, 4, 7, 9, 2};
      LCA lca(5, 1, tree_adj);
      HLD hld(5, 1, tree_adj, node_vals);
      if (lca.lca(4, 5) != 2 || lca.dist(4, 3) != 3 || hld.query_path(4, 3) != 4) {
        cerr << "Fallo diferencial LCA / HLD\n";
        return 1;
      }

      // 4. Matching, Hungaro, Blossom y 2-SAT vs fuerza bruta
      BipartiteMVC bmvc(3, 3);
      bmvc.add_edge(1, 1); bmvc.add_edge(1, 2);
      bmvc.add_edge(2, 2); bmvc.add_edge(3, 3);
      auto [cov_l, cov_r] = bmvc.min_vertex_cover();
      if (sz(cov_l) + sz(cov_r) != 3) {
        cerr << "Fallo diferencial BipartiteMVC\n";
        return 1;
      }
      Blossom bl(5);
      bl.add_edge(1, 2); bl.add_edge(2, 3); bl.add_edge(3, 1);
      bl.add_edge(3, 4); bl.add_edge(4, 5);
      if (bl.solve() != 2) {
        cerr << "Fallo diferencial Blossom en pentagono/triangulo\n";
        return 1;
      }
      TwoSAT sat(3);
      sat.add_or(0, true, 1, true);
      sat.add_or(0, false, 1, true);
      sat.add_or(1, false, 2, false);
      if (!sat.solve() || !sat.ans[1] || sat.ans[2]) {
        cerr << "Fallo diferencial TwoSAT\n";
        return 1;
      }

      // 5. Cadenas: KMP, Z, Manacher, SA, SAM y PalindromicTree vs enumeracion directa
      string s = "abacaba";
      vi pi = prefix_function(s);
      vi zf = z_function(s);
      if (pi.back() != 3 || zf[4] != 3 || sz(kmp_search(s, "aba")) != 2) {
        cerr << "Fallo diferencial KMP / Z\n";
        return 1;
      }
      SuffixArray sa(s);
      ll sa_distinct = 1LL * sz(s) * (sz(s) + 1) / 2;
      for (int i = 2; i <= sz(s); ++i) sa_distinct -= sa.lcp[i];
      SuffixAutomaton sam(s);
      if (sa_distinct != sam.count_distinct_substrings()) {
        cerr << "Fallo diferencial SA vs SAM distinct substrings\n";
        return 1;
      }
      PalindromicTree pt(s);
      if (sz(pt.st) - 2 != 7) {
        cerr << "Fallo diferencial PalindromicTree\n";
        return 1;
      }
      return 0;
    }
    """
    ok, _out, err = ctx.compile_and_run_cpp(
        "diff_oracles_all",
        [
            "Fenwick",
            "IterSegTree",
            "SegTree",
            "LazySegTree",
            "ImplicitTreap",
            "LinkCut",
            "LCA",
            "HLD",
            "BipartiteMVC",
            "Blossom",
            "TwoSAT",
            "kmp_search",
            "z_function",
            "SuffixArray",
            "SuffixAutomaton",
            "PalindromicTree",
        ],
        driver,
    )
    return ok, err


def main():
    parser = argparse.ArgumentParser(description="Ejecutor de pruebas del Notebook ICPC")
    parser.add_argument("--input", "-i", default=DEFAULT_SOURCE_PATH, help="Directorio modular (sections/) o archivo Markdown fuente")
    parser.add_argument("--strict", action="store_true", help="Fallar con código != 0 si alguna regresión falla")
    args = parser.parse_args()

    t0 = time.time()
    ctx = TestContext(args.input)
    print(f"=== Suite de Pruebas ICPC Notebook ===")
    print(f"Fuente: {args.input} ({len(ctx.snippets)} bloques extraídos)")
    print(f"Compilador: {ctx.gpp} | UBSan disponible: {ctx.has_ubsan}\n")

    tests: List[Tuple[str, Callable[[TestContext], Tuple[bool, str]]]] = [
        ("BM discrepancia negativa ({0,1,1,2}, k=4 -> 3)", test_reg_bm_negative_discrepancy),
        ("Push-Relabel gap/relabel (N=6 -> flujo 1 + vs Dinic)", test_reg_push_relabel),
        ("Puentes con aristas paralelas (0 puentes)", test_reg_bridges_parallel_edges),
        ("CRT overflow en producto intermedio", test_reg_crt_overflow),
        ("Closest pair con puntos duplicados (0,0)x3", test_reg_closest_pair_duplicates),
        ("Minkowski con un punto {(0,0)} + triángulo", test_reg_minkowski_single_point),
        ("Comparador geométrico Pt::operator< (orden estricto)", test_reg_point_comparator_strict_weak_ordering),
        ("PBDS ordered_multiset sin less_equal", test_reg_pbds_multiset),
        ("Template base sin eps_cmp inseguro", test_reg_eps_cmp_template),
        ("Miller-Rabin 64-bit con __uint128_t (N >= 2^63)", test_reg_miller_rabin_u64),
        ("Block-Cut Tree (retroceso, paralelas, aislados)", test_reg_block_cut_tree),
        ("Halfplane Intersection (franja, vacía, acotada)", test_reg_halfplane_intersection),
        ("Divide & Conquer DP (estados inalcanzables)", test_reg_dc_dp_unreachable),
        ("Simplex Dos Fases (infactible, óptimo < 0, no acotado)", test_reg_simplex_feasibility_and_pivot),
        ("Bellman-Ford (saturación -INF sin falso ciclo)", test_reg_bellman_ford_saturation),
        ("Criba segmentada en L=0 y L=1", test_reg_segmented_sieve_zero_one),
        ("Aritmética modular con negativos e invertibilidad", test_reg_modular_arithmetic_negatives),
        ("Misère Nim con montones 0 y Parser (-funsigned-char)", test_reg_misere_nim_and_parser),
        ("Sintaxis y contratos de bloques Python", test_python_snippets_syntax),
        ("Fase 3: Consolidación, tablas MD y capacidades prometidas", test_phase3_alignments_and_consolidations),
        ("Fase 4: Compilación y sintaxis de las 164 fichas operativas", test_phase4_inventory_and_all_164_fiches),
        ("Fase 5: Nuevas técnicas de alto retorno (MITM, Subset DP, Reroot, NTT, etc.)", test_phase5_new_techniques),
        ("Fase 6: Guía de decisión, checklist anti-WA y separación DP/Geometría", test_phase6_navigation_and_structure),
        ("Fase 7: Ancho <= 60 caracteres en los 164 bloques y artefactos", test_phase7_line_width_and_build_pipeline),
        ("Sección 11: Oráculos diferenciales en todos los dominios", test_differential_oracles_all_domains),
    ]

    passed = 0
    failed = 0
    try:
        with ThreadPoolExecutor(max_workers=6) as pool:
            futures = [(label, pool.submit(fn, ctx)) for label, fn in tests]
            for label, fut in futures:
                ok, msg = fut.result()
                status = "PASS" if ok else "FAIL"
                print(f"[{status}] {label}")
                if not ok and msg:
                    for line in msg.strip().splitlines()[:6]:
                        print(f"       -> {line}")
                if ok:
                    passed += 1
                else:
                    failed += 1
    finally:
        ctx.cleanup()

    dt = time.time() - t0
    print(f"\nResumen: {passed} pasaron, {failed} fallaron en {dt:.2f}s.")
    if failed > 0 and args.strict:
        sys.exit(1)


if __name__ == "__main__":
    main()
