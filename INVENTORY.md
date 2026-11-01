# Inventario Maestro del Notebook ICPC (OUR BRAINS ARE DIAZ O)

- **Total de fichas operativas de algoritmos:** `164` (`164/164` en estado `completo`)
- **Total de secciones teóricas / tablas de referencia:** `13` (`no_aplicable` con justificación explícita)
- **Orden verificado en cada ficha de algoritmo:** `Cuándo usar` → `Cuándo no usar` → `Contrato` → `Complejidad` → `Formato de entrada` → `Entrada` → `Salida` → **`Cómo leer la entrada`** → **`Implementación`** → **`Cómo llamarlo y producir la salida`** → `Trampas y adaptaciones`.

## 1. Fichas Operativas de Algoritmos

| Sección | ID Estable | Algoritmo / Estructura | Lenguaje | Dependencias | Estado de las 10 Partes |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `§1.1` | `sec_1_1` | Template Base ICPC, Pragmas, Formateo `cout`/`printf` y Comparador `EPS` | `cpp` | `base_template` | `completo` |
| `§1.2` | `rng_mt19937` | Generador Aleatorio (`mt19937_64`), `shuffle` y `rd(l, r)` | `cpp` | `base_template` | `completo` |
| `§1.3` | `sec_1_3_string_methods_demo` | `std::string`: Métodos Clave (`find`, `rfind`, `substr`, `erase`, `insert`, `replace`, `stringstream`, `stoll`) | `cpp` | `base_template` | `completo` |
| `§1.4` | `sec_1_4_bitmask_tricks` | Manipulación de Bits (`__builtin`, LSO, Submáscaras) y `std::bitset` | `cpp` | `base_template` | `completo` |
| `§1.5` | `sec_1_5_to_binary_string` | Cambios de Base (Base $A \leftrightarrow B$, Binario, Base Negativa) y Números Gigantes en String | `cpp` | `base_template` | `completo` |
| `§2.1` | `sec_2_1_coordcomp` | `std::vector` y `<algorithm>`: Métodos, `sort`, Búsqueda Binaria (`lower_bound`/`upper_bound`) y Compresión de Coordenadas | `cpp` | `base_template` | `completo` |
| `§2.2` | `sec_2_2_linear_containers_demo` | Contenedores Lineales: `std::deque`, `std::queue` y `std::stack` | `cpp` | `base_template` | `completo` |
| `§2.3` | `sec_2_3_state` | Colas de Prioridad / Heaps (`std::priority_queue`): Max-Heap, Min-Heap, Custom y **Deletable Heap** | `cpp` | `base_template` | `completo` |
| `§2.4` | `sec_2_4_set_multiset_map_cheatsheet` | Árboles Balanceados (`std::set`, `std::multiset`, `std::map`): Métodos, Búsqueda Binaria en `set` y Borrado Seguro en `multiset` | `cpp` | `base_template` | `completo` |
| `§2.5` | `custom_hash` | Tablas Hash (`unordered_map`, `unordered_set`) con `custom_hash` Anti-Hack (SplitMix64) y Pares Iguales | `cpp` | `base_template` | `completo` |
| `§2.6` | `pbds_ordered_set` | Extensiones GNU: `ordered_set` / `ordered_multiset` (PBDS) y `__gnu_cxx::rope` | `cpp` | `base_template` | `completo` |
| `§3.1.1` | `sec_3_1_1_bs_max` | Binary Search Discreta sobre la Respuesta (`bs_max` y `bs_min`) | `cpp` | `base_template` | `completo` |
| `§3.1.2` | `sec_3_1_2_algorithm_3` | Binary Search Continua en Punto Flotante (`bs_double`) | `cpp` | `base_template` | `completo` |
| `§3.2.1` | `sec_3_2_1_algorithm_3` | Ternary Search Continua (`ternary_continuous`) | `cpp` | `base_template` | `completo` |
| `§3.2.2` | `sec_3_2_2_ternary_discrete` | Ternary Search Discreta en Enteros (`ternary_discrete`) | `cpp` | `base_template` | `completo` |
| `§3.3.1` | `sec_3_3_1_max_subarray_sum_bound` | Two Pointers / Ventana Deslizante (`max_subarray_sum_bound`) | `cpp` | `base_template` | `completo` |
| `§3.3.2` | `sec_3_3_2_next_greater_right` | Monotonic Stack: Primer Mayor/Menor a la Derecha o Izquierda (`next_greater_right`) | `cpp` | `base_template` | `completo` |
| `§3.3.3` | `sec_3_3_3_sliding_window_min` | Monotonic Deque: Mínimo/Máximo en Ventana Deslizante (`sliding_window_min`) | `cpp` | `base_template` | `completo` |
| `§3.4.1` | `sec_3_4_1_prefixsum` | Prefix Sum 1D (`PrefixSum`) | `cpp` | `base_template` | `completo` |
| `§3.4.2` | `sec_3_4_2_prefixsum2d` | Prefix Sum 2D en Submatrices (`PrefixSum2D`) | `cpp` | `base_template` | `completo` |
| `§3.4.3` | `prefix_patterns` | Patrones de Prefijos (Suma $K$, Divisibles por $M$, Prefix XOR) y Kadane | `cpp` | `custom_hash` | `completo` |
| `§3.5.1` | `sec_3_5_1_diffarray1d` | Difference Array 1D y Doble Difference Array (`DiffArray1D`, `apply_double_diff`) | `cpp` | `base_template` | `completo` |
| `§3.5.2` | `sec_3_5_2_diffarray2d` | Difference Array 2D en Submatrices (`DiffArray2D`) | `cpp` | `base_template` | `completo` |
| `§3.6.1` | `greedy_scheduling` | Interval Scheduling (Selección Máxima de Intervalos Disjuntos) | `cpp` | `base_template` | `completo` |
| `§3.6.2` | `greedy_scheduling_2` | Greedy con arrepentimiento (Moore-Hodgson con Max-Heap) | `cpp` | `base_template` | `completo` |
| `§3.6.3` | `greedy_scheduling_3` | Argumento de Intercambio de Smith (`exchange_argument_order`) | `cpp` | `base_template` | `completo` |
| `§3.7.1` | `sweep_line_1d` | Máximo Solapamiento de Intervalos por Sweep-Line (`max_interval_overlap`) | `cpp` | `base_template` | `completo` |
| `§3.7.2` | `sweep_line_1d_2` | Unión de Intervalos Cerrados y Longitud Cubierta (`merge_intervals`) | `cpp` | `base_template` | `completo` |
| `§4.1.1` | `dsu` | Disjoint Set Union Clásico (`DSU`) | `cpp` | `base_template` | `completo` |
| `§4.1.2` | `rollback_dsu` | DSU con Rollback y Conectividad Dinámica Offline (`RollbackDSU`, `DynCon`) | `cpp` | `rollback_dsu` | `completo` |
| `§4.2` | `sparse_table` | Sparse Table (RMQ e Idempotentes en $\mathcal{O}(1)$) | `cpp` | `base_template` | `completo` |
| `§4.3.1` | `fenwick_1d` | Fenwick Tree / BIT 1D con Binary Lifting (`Fenwick`) | `cpp` | `base_template` | `completo` |
| `§4.3.2` | `fenwick_2d` | Fenwick Tree 2D (`Fenwick2D`) | `cpp` | `base_template` | `completo` |
| `§4.4.1` | `segtree_iter` | Segment Tree Iterativo (`IterSegTree`) | `cpp` | `base_template` | `completo` |
| `§4.4.2` | `segtree_rec` | Segment Tree Recursivo Clásico (`SegTree`) | `cpp` | `base_template` | `completo` |
| `§4.5.1` | `lazy_segtree` | Segment Tree con Lazy Propagation (`LazySegTree`) | `cpp` | `base_template` | `completo` |
| `§4.5.2` | `segtree_2d` | Segment Tree 2D Iterativo (`SegTree2D`) | `cpp` | `base_template` | `completo` |
| `§4.5.3` | `persistent_segtree` | Persistent Segment Tree de Sumas en Versiones Históricas (`PersistentST`) | `cpp` | `base_template` | `completo` |
| `§4.5.4` | `sec_4_5_4_persistentfreqst` | Persistent Segment Tree de Frecuencias: $k$-ésimo en Subarreglo (`PersistentFreqST`) | `cpp` | `base_template` | `completo` |
| `§4.6.1` | `sec_4_6_1_sqrtdecomp` | SQRT Decomposition en Bloques (`SqrtDecomp`) | `cpp` | `base_template` | `completo` |
| `§4.6.2` | `sec_4_6_2_moquery` | Algoritmo de Mo Offline (`MoQuery`, `solve_mo`) | `cpp` | `base_template` | `completo` |
| `§4.7` | `sec_4_7_implicittreap` | Treap (BST Aleatorizado) e Implicit Treap (Reversión y Suma en Rango) | `cpp` | `base_template` | `completo` |
| `§4.8.1` | `hld` | Heavy-Light Decomposition (`HLD`) | `cpp` | `base_template` | `completo` |
| `§4.8.2` | `sec_4_8_2_pre_sack` | DSU on Tree / Small-to-Large en Subárboles (`pre_sack`, `dfs_sack`) | `cpp` | `base_template` | `completo` |
| `§4.9` | `sec_4_9_lctnode` | Link-Cut Tree (LCT — Bosques Dinámicos Online) | `cpp` | `base_template` | `completo` |
| `§5.1` | `sec_5_1_min_cyclic_rotation` | Rotación Cíclica Mínima / Minimum Expression (Duval/Booth $\mathcal{O}(N)$) | `cpp` | `base_template` | `completo` |
| `§5.2` | `sec_5_2_polyhash` | Double Rolling Hash Polinomial y LCP con Búsqueda Binaria | `cpp` | `base_template` | `completo` |
| `§5.3.1` | `kmp` | KMP: Función Prefijo $\pi$ y Búsqueda de Patrón (`prefix_function`, `kmp_search`) | `cpp` | `base_template` | `completo` |
| `§5.3.2` | `sec_5_3_2_build_kmp_automaton` | Autómata KMP (`build_kmp_automaton`) | `cpp` | `base_template` | `completo` |
| `§5.4.1` | `z_algorithm` | Z-Algorithm (`z_function`) | `cpp` | `base_template` | `completo` |
| `§5.4.2` | `sec_5_4_2_manacher` | Algoritmo de Manacher para Palíndromos Pares e Impares (`Manacher`) | `cpp` | `base_template` | `completo` |
| `§5.5.1` | `sec_5_5_1_stringtrie` | Trie de Strings: Prefijos y Palabras Exactas (`StringTrie`) | `cpp` | `base_template` | `completo` |
| `§5.5.2` | `bit_trie` | Trie Binario para Max/Min XOR Dinámico (`BitTrie`) | `cpp` | `base_template` | `completo` |
| `§5.5.3` | `aho_corasick` | Autómata de Aho-Corasick para Múltiples Patrones (`AhoCorasick`) | `cpp` | `base_template` | `completo` |
| `§5.6.1` | `suffix_array` | Suffix Array y Arreglo LCP de Kasai (`SuffixArray`) | `cpp` | `base_template` | `completo` |
| `§5.6.2` | `suffix_automaton` | Suffix Automaton (`SuffixAutomaton`) | `cpp` | `base_template` | `completo` |
| `§5.6.3` | `sec_5_6_3_palindromictree` | Palindromic Tree / Eertree (`PalindromicTree`) | `cpp` | `base_template` | `completo` |
| `§6.1.1` | `sec_6_1_1_flood_fill_bfs` | Grid BFS / Flood Fill con Identificador de Componentes (`flood_fill_bfs`) | `cpp` | `base_template` | `completo` |
| `§6.1.2` | `sec_6_1_2_multi_source_bfs` | Multi-Source BFS y Aristas Invertidas (`multi_source_bfs`) | `cpp` | `base_template` | `completo` |
| `§6.1.3` | `sec_6_1_3_bfs_01` | 0-1 BFS con `std::deque` y Reconstrucción (`bfs_01`, `get_path_01`) | `cpp` | `base_template` | `completo` |
| `§6.2.1` | `sec_6_2_1_dfs_bipartite` | Grafo Bipartito y 2-Coloreo por DFS (`dfs_bipartite`) | `cpp` | `base_template` | `completo` |
| `§6.2.2` | `sec_6_2_2_dfs_dir_cycle` | Detección de Ciclo en Grafo Dirigido con 3 Estados (`dfs_dir_cycle`) | `cpp` | `base_template` | `completo` |
| `§6.2.3` | `sec_6_2_3_dfs_euler` | Euler Tour en Árboles y Ancestralidad en $\mathcal{O}(1)$ (`dfs_euler`, `is_ancestor`) | `cpp` | `base_template` | `completo` |
| `§6.2.4` | `sec_6_2_4_complement_components` | Componentes Conexas en el Grafo Complemento (`complement_components`) | `cpp` | `base_template` | `completo` |
| `§6.2.5` | `sec_6_2_5_count_triangles` | Conteo de Triángulos ($K_3$) por Orientación de Grado (`count_triangles`) | `cpp` | `base_template` | `completo` |
| `§6.3.1` | `sec_6_3_1_dijkstra` | Dijkstra con Min-Heap y Reconstrucción (`dijkstra`, `get_path_dij`) | `cpp` | `base_template` | `completo` |
| `§6.3.2` | `sec_6_3_2_bfedge` | Bellman-Ford con Reconstrucción de Ciclo Negativo (`bellman_ford_cycle`) | `cpp` | `base_template` | `completo` |
| `§6.3.3` | `sec_6_3_3_floyd_warshall` | Floyd-Warshall Todos contra Todos (`floyd_warshall`) | `cpp` | `base_template` | `completo` |
| `§6.4.1` | `sec_6_4_1_bfs_farthest` | Diámetro de un Árbol por Doble BFS (`bfs_farthest`, `get_diameter_path`) | `cpp` | `base_template` | `completo` |
| `§6.4.2` | `sec_6_4_2_kredge` | Árbol Generador Mínimo de Kruskal (`kruskal_mst`) | `cpp` | `base_template` | `completo` |
| `§6.4.3` | `sec_6_4_3_lca` | LCA, $K$-ésimo Ancestro y Distancia por Binary Lifting (`LCA`) | `cpp` | `base_template` | `completo` |
| `§6.4.4` | `sec_6_4_4_centroiddecomp` | Descomposición por Centroides (`CentroidDecomp`) | `cpp` | `base_template` | `completo` |
| `§6.5.1` | `toposort` | Orden Topológico de Kahn por BFS (`topological_sort`) | `cpp` | `base_template` | `completo` |
| `§6.5.2` | `bridges_and_cuts` | Puentes y Puntos de Articulación de Tarjan (`BridgesAndCuts`) | `cpp` | `base_template` | `completo` |
| `§6.5.3` | `tarjan_scc` | Componentes Fuertemente Conexas de Tarjan (`TarjanSCC`) | `cpp` | `base_template` | `completo` |
| `§6.5.4` | `twosat` | Solucionador 2-SAT en Tiempo Lineal (`TwoSAT`) | `cpp` | `tarjan_scc` | `completo` |
| `§6.6.1` | `dinic` | Flujo Máximo y Corte Mínimo de Dinic (`Dinic`) | `cpp` | `base_template` | `completo` |
| `§6.6.2` | `mcmf` | Flujo Máximo de Costo Mínimo (`MCMF`) | `cpp` | `base_template` | `completo` |
| `§6.6.3` | `bipartite_mvc` | Matching Bipartito de Kuhn y Reconstrucción de König (`BipartiteMVC`) | `cpp` | `base_template` | `completo` |
| `§6.6.4` | `hungarian` | Algoritmo Húngaro de Asignación de Costo Mínimo (`hungarian`) | `cpp` | `base_template` | `completo` |
| `§6.6.5` | `blossom` | Matching Máximo en Grafo General: Algoritmo Blossom de Edmonds (`Blossom`) | `cpp` | `base_template` | `completo` |
| `§6.7` | `directed_mst` | Árbol Generador Mínimo Dirigido (Directed MST / Arborescence) | `cpp` | `base_template` | `completo` |
| `§6.8` | `block_cut_tree` | Componentes Biconexas y Block-Cut Tree | `cpp` | `base_template` | `completo` |
| `§6.9` | `eulerian_directed` | Camino y Ciclo Euleriano (Hierholzer) | `cpp` | `base_template` | `completo` |
| `§6.10` | `push_relabel` | Flujo Máximo en Redes Densas: Push-Relabel (Highest-Label + Gap) | `cpp` | `base_template` | `completo` |
| `§6.11` | `functional_graph` | Grafos Funcionales (Successors): Ciclos, Colas y Binary Lifting | `cpp` | `base_template` | `completo` |
| `§7.1.1` | `sieve_eratosthenes` | Criba de Eratóstenes (`sieve`) | `cpp` | `base_template` | `completo` |
| `§7.1.2` | `harmonic_sieve` | Criba Armónica de Divisores (`harmonic_sieve`) | `cpp` | `base_template` | `completo` |
| `§7.1.3` | `linear_sieve_spf` | Criba Lineal SPF y Factorización en $\mathcal{O}(\log X)$ (`linear_sieve_spf`, `get_prime_factors`) | `cpp` | `base_template` | `completo` |
| `§7.1.4` | `segmented_sieve` | Criba Segmentada en Rango $[L, R]$ (`segmented_sieve`) | `cpp` | `base_template` | `completo` |
| `§7.1.5` | `is_prime_single` | Test de Primalidad Individual en $\mathcal{O}(\sqrt{N})$ (`is_prime_single`) | `cpp` | `base_template` | `completo` |
| `§7.2.1` | `miller_rabin` | Test de Primalidad Determinista Miller-Rabin 64-bit (`miller_rabin`) | `cpp` | `base_template` | `completo` |
| `§7.2.2` | `pollard_rho` | Factorización de 64-bit con Pollard's Rho (`pollard_rho`, `factorize_64`) | `cpp` | `rng_mt19937`, `miller_rabin` | `completo` |
| `§7.3.1` | `sec_7_3_1_array_diff_gcd` | GCD de Arreglo tras Sumar $X$ (`array_diff_gcd`) | `cpp` | `base_template` | `completo` |
| `§7.3.2` | `sec_7_3_2_solve_diff_of_squares` | Representación como Diferencia de Cuadrados $N = a^2 - b^2$ (`solve_diff_of_squares`) | `cpp` | `base_template` | `completo` |
| `§7.3.3` | `extgcd` | Algoritmo de Euclides Extendido (`extgcd`) | `cpp` | `base_template` | `completo` |
| `§7.3.4` | `diophantine` | Ecuación Diofántica Lineal $a x + b y = c$ (`find_diophantine`) | `cpp` | `base_template` | `completo` |
| `§7.3.5` | `crt_general` | Teorema Chino del Resto General (`crt_general`) | `cpp` | `base_template` | `completo` |
| `§7.4.1` | `modular_arith` | Aritmética Modular Segura e Inverso Modular (`norm_mod`, `binpow`, `inv_fermat`, `inv_extgcd`) | `cpp` | `base_template` | `completo` |
| `§7.4.2` | `combinatorics` | Combinatoria en $\mathcal{O}(1)$, Teorema de Lucas y Fórmula de Legendre (`init_fact`, `nCr`, `lucas_nCr`, `legendre_prime_in_fact`) | `cpp` | `extgcd`, `modular_arith`, `combinatorics` | `completo` |
| `§7.5.1` | `euler_phi` | Totient de Euler Individual $\phi(N)$ en $\mathcal{O}(\sqrt{N})$ (`euler_phi`) | `cpp` | `base_template` | `completo` |
| `§7.5.2` | `phi_mobius_sieve` | Criba de Euler $\phi[1..N]$ y Función de Möbius $\mu[1..N]$ (`init_phi_and_mobius`) | `cpp` | `base_template` | `completo` |
| `§7.6.1` | `matrix_exp` | Exponenciación de Matrices y Fibonacci Rápido (`Matrix`, `mat_pow`, `fib_fast`) | `cpp` | `modular_arith`, `custom_hash` | `completo` |
| `§7.6.2` | `xor_basis` | Base Lineal de XOR / Eliminación Gaussiana en $\mathbb{F}_2$ (`XorBasis`) | `cpp` | `base_template` | `completo` |
| `§7.6.3` | `gauss_double` | Eliminación Gaussiana en Punto Flotante (`gauss_double`) | `cpp` | `base_template` | `completo` |
| `§7.6.4` | `gauss_mod` | Eliminación Gaussiana Modular (`gauss_mod`) | `cpp` | `base_template` | `completo` |
| `§7.6.5` | `fft` | Transformada Rápida de Fourier para Polinomios (`fft`, `multiply_poly`) | `cpp` | `base_template` | `completo` |
| `§7.6.6` | `ntt` | Transformada Numérica Modular Exacta (`ntt`, `multiply_ntt`) | `cpp` | `base_template` | `completo` |
| `§7.6.7` | `fwt_xor` | Transformada Rápida de Walsh-Hadamard para Convolución XOR (`fwt_xor`) | `cpp` | `base_template` | `completo` |
| `§7.6.8` | `discrete_log` | Logaritmo Discreto Baby-Step Giant-Step (`discrete_log`) | `cpp` | `custom_hash` | `completo` |
| `§7.6.9` | `frac` | Estructura de Fracciones Racionales Exactas (`Frac`) | `cpp` | `base_template` | `completo` |
| `§7.7.1` | `xor_range` | XOR Acumulado en Rango $[L, R]$ en $\mathcal{O}(1)$ (`xor_n`, `xor_range`) | `cpp` | `base_template` | `completo` |
| `§7.7.2` | `mex` | Mínimo Excluido No Negativo (`mex`) para Grundy / Sprague-Grundy | `cpp` | `base_template` | `completo` |
| `§7.7.3` | `nim` | Juego de Nim Normal y Misère Nim (`nim_normal_win`, `nim_misere_win`) | `cpp` | `base_template` | `completo` |
| `§7.9` | `simplex` | Programación Lineal: Algoritmo Simplex de Dos Fases | `cpp` | `base_template` | `completo` |
| `§7.10` | `berlekamp_massey` | Berlekamp-Massey y Recurrencias Lineales Modulares | `cpp` | `modular_arith` | `completo` |
| `§7.11.1` | `lagrange` | Interpolación de Lagrange en $\mathcal{O}(D)$ para Abscisas Consecutivas $0..D$ (`lagrange_eval_consecutive`) | `cpp` | `modular_arith` | `completo` |
| `§7.11.2` | `sec_7_11_2_lagrange_eval_arbitrary` | Interpolación de Lagrange en $\mathcal{O}(N^2)$ para Puntos Arbitrarios (`lagrange_eval_arbitrary`) | `cpp` | `base_template` | `completo` |
| `§7.12.1` | `counting_expectation` | Stars and Bars: Composiciones No Negativas y Positivas (`stars_and_bars_nonneg`, `stars_and_bars_pos`) | `cpp` | `extgcd`, `modular_arith`, `combinatorics` | `completo` |
| `§7.12.2` | `counting_expectation_2` | Principio de Inclusión-Exclusión para Múltiplos (`inclusion_exclusion_multiples`) | `cpp` | `extgcd`, `modular_arith`, `combinatorics` | `completo` |
| `§7.12.3` | `counting_expectation_3` | Técnica de Contribuciones con Pila Monótona (`sum_all_subarray_mins`) | `cpp` | `extgcd`, `modular_arith`, `combinatorics` | `completo` |
| `§7.12.4` | `sec_7_12_4_expected_coupon_collector` | Linealidad de la Esperanza: Coupon Collector (`expected_coupon_collector`) | `cpp` | `base_template` | `completo` |
| `§8.1.1` | `knapsack` | Mochila 0/1 en Memoria Lineal (`knapsack_01`) | `cpp` | `base_template` | `completo` |
| `§8.1.2` | `lcs` | Subsecuencia Común Más Larga con Reconstrucción (`lcs_string`) | `cpp` | `base_template` | `completo` |
| `§8.1.3` | `edit_distance` | Distancia de Edición de Levenshtein (`edit_distance`) | `cpp` | `base_template` | `completo` |
| `§8.1.4` | `lis` | Subsecuencia Creciente Más Larga en $\mathcal{O}(N \log N)$ (`lis_length`) | `cpp` | `base_template` | `completo` |
| `§8.1.5` | `digit_dp` | Digit DP por Prefijos Decimales (`dp_dig`, `solve_digit_dp`) | `cpp` | `base_template` | `completo` |
| `§8.1.6` | `sos_dp` | SOS DP / Suma Sobre Submáscaras (`sos_dp`) | `cpp` | `base_template` | `completo` |
| `§8.2.1` | `subset_dp` | DP de Asignación por Máscaras de Bits (`subset_assignment_dp`) | `cpp` | `base_template` | `completo` |
| `§8.2.2` | `subset_dp_2` | DP de Caminos / Travelling Salesperson Problem con Reconstrucción (`tsp_subset_dp`) | `cpp` | `base_template` | `completo` |
| `§8.3` | `rerooting_dp` | DP en Árboles y Técnica de Rerooting (Cambio de Raíz en $\mathcal{O}(N)$) | `cpp` | `base_template` | `completo` |
| `§8.4.1` | `cht_dynamic` | Convex Hull Trick Dinámico (`LineContainer`) | `cpp` | `base_template` | `completo` |
| `§8.4.2` | `cht_monotone` | Convex Hull Trick Estático / Monótono con Deque (`MonotoneCHT`) | `cpp` | `base_template` | `completo` |
| `§8.4.3` | `divide_and_conquer_dp` | Optimización Divide & Conquer DP (`compute_dc`, `solve_dc_dp`) | `cpp` | `base_template` | `completo` |
| `§8.4.4` | `knuth_dp` | Optimización de Knuth para DP de Intervalos (`solve_knuth`) | `cpp` | `base_template` | `completo` |
| `§9.1.1` | `geometry_2d` | Punto 2D, Producto Punto, Producto Cruz y Orden Radial (`Pt`, `dot`, `cross`, `polar_cmp`) | `cpp` | `base_template` | `completo` |
| `§9.1.2` | `sec_9_1_2_on_segment` | Segmentos y Rectas: Pertenencia, Distancia e Intersección (`on_segment`, `dist_point_segment`, `segments_intersect`, `line_intersection`) | `cpp` | `base_template` | `completo` |
| `§9.1.3` | `geometry_int` | Polígonos: Área, Centroide, Inclusión $\mathcal{O}(N)$ / Convexo $\mathcal{O}(\log N)$ y Kernel Entero (`polygon_area`, `polygon_centroid`, `point_in_polygon`, `point_in_convex_polygon`, `PtLL`) | `cpp` | `geometry_2d` | `completo` |
| `§9.1.4` | `convex_hull` | Envolvente Convexa de Andrew (`convex_hull`) | `cpp` | `geometry_2d` | `completo` |
| `§9.1.5` | `rotating_callipers` | Diámetro de Polígono Convexo por Rotating Callipers (`rotating_callipers_max_dist2`) | `cpp` | `geometry_2d` | `completo` |
| `§9.1.6` | `closest_pair` | Par de Puntos Más Cercanos por Sweep-Line (`closest_pair_distance`) | `cpp` | `geometry_2d` | `completo` |
| `§9.2.1` | `analytic_geometry_circles` | Proyección, Reflexión y Distancia Punto-Recta (`proj_point_line`, `reflect_point_line`, `dist_point_line`) | `cpp` | `geometry_2d` | `completo` |
| `§9.2.2` | `analytic_geometry_circles_2` | Geometría de Círculos: Intersecciones y Tangentes (`Circle`, `circle_line_intersection`, `circle_circle_intersection`, `circle_tangents_from_point`) | `cpp` | `geometry_2d` | `completo` |
| `§9.3` | `halfplane_intersection` | Intersección de Semiplanos (Halfplane Intersection) | `cpp` | `geometry_2d` | `completo` |
| `§9.4` | `minkowski_sum` | Suma de Minkowski de Polígonos Convexos | `cpp` | `geometry_2d` | `completo` |
| `§10.1.1` | `meet_in_the_middle` | Meet in the Middle: Conteo de Subconjuntos con Suma Exacta (`get_subset_sums`, `mitm_subset_sum_count`) | `cpp` | `base_template` | `completo` |
| `§10.1.2` | `meet_in_the_middle_2` | Meet in the Middle: Mayor Suma de Subconjunto $\le S$ (`mitm_max_subset_sum_le`) | `cpp` | `base_template` | `completo` |
| `§10.2` | `shunting_yard` | Evaluador de Expresiones Aritméticas (Shunting-Yard Parser) | `cpp` | `base_template` | `completo` |
| `§10.3` | `max_clique` | Clique Máximo (Bron-Kerbosch Branch & Bound) | `cpp` | `base_template` | `completo` |
| `§12.1` | `sec_12_1_algorithm_3` | Template Base, Fast I/O, Límites de Recursión y Digits en BigInt | `python` | `base_template` | `completo` |
| `§12.2` | `sec_12_2_to_base` | Conversión de Bases Arbitrarias (Base $A \leftrightarrow B$) y Formateo | `python` | `base_template` | `completo` |
| `§12.3` | `sec_12_3_trunc_div` | Aritmética Gigante (BigInt), Potencia Modular y Raíz Entera | `python` | `base_template` | `completo` |
| `§12.4` | `sec_12_4_algorithm_3` | Fracciones Exactas (`Fraction`) y Decimales de Alta Precisión (`Decimal`) | `python` | `base_template` | `completo` |
| `§12.5` | `sec_12_5_algorithm_3` | Manejo de Fechas y Calendarios (`datetime`) | `python` | `base_template` | `completo` |
| `§12.6` | `sec_12_6_dp` | DP con Memoización Automática (`@cache` / `cache_clear`) | `python` | `base_template` | `completo` |
| `§12.7` | `sec_12_7_algorithm_3` | Extracción de Números en Texto Sucio con Expresiones Regulares (`re`) | `python` | `base_template` | `completo` |
| `§12.8` | `sec_12_8_algorithm_3` | Frecuencias y Álgebra de Multiconjuntos (`Counter`) | `python` | `base_template` | `completo` |
| `§12.9` | `sec_12_9_algorithm_3` | Agrupación y Grafos Instantáneos (`defaultdict`) | `python` | `base_template` | `completo` |
| `§12.10` | `sec_12_10_algorithm_3` | Rotación y Transposición de Matrices en 1 Línea (`zip(*mat)`) | `python` | `base_template` | `completo` |
| `§12.11` | `sec_12_11_algorithm_3` | Álgebra de Conjuntos Nativos (`set`) | `python` | `base_template` | `completo` |
| `§12.12` | `sec_12_12_algorithm_3` | Bits en Números Gigantes (`bit_count`, `bit_length`) | `python` | `base_template` | `completo` |
| `§12.13` | `sec_12_13_algorithm_3` | Búsqueda Binaria con Proyección (`bisect` con `key`) y Ordenamiento Multi-Criterio | `python` | `base_template` | `completo` |
| `§12.14` | `sec_12_14_algorithm_3` | Combinatoria y Generadores Nativos (`comb`, `perm`, `itertools`) | `python` | `base_template` | `completo` |
| `§12.15` | `sec_12_15_algorithm_3` | Evaluación de Expresiones Aritméticas (`eval`) | `python` | `base_template` | `completo` |

## 2. Secciones Teóricas, Navegación y Referencias Cruzadas

| Sección | Título | Estado | Justificación |
| :--- | :--- | :--- | :--- |
| `§0.1` | Selector Rápido: Señal del Problema → Técnica → Condición → Sección | `no_aplicable` | Tabla de selección rápida (navegación, sin código ejecutable) |
| `§0.2` | Tabla Orientativa de Complejidad según Restricciones ($N$) | `no_aplicable` | Tabla orientativa de complejidad según N (teórica) |
| `§0.3` | Advertencias Críticas de Elección de Técnica | `no_aplicable` | Advertencias críticas de elección de técnica (teórica) |
| `§0.4` | Checklist Anti-WA Antes de Enviar | `no_aplicable` | Checklist anti-WA antes de enviar (teórica) |
| `§7.8` | Referencia Cruzada: Criba Segmentada en Rango $[L, R]$ | `no_aplicable` | Referencia cruzada a §7.1.4 (evita duplicar segmented_sieve) |
| `§11.1` | Condiciones de Optimización de Programación Dinámica (DP Speedups) | `no_aplicable` | Condiciones teóricas de optimización de DP (tabla) |
| `§11.2` | Combinatoria y Sumatorias | `no_aplicable` | Fórmulas de combinatoria y sumatorias (teórica) |
| `§11.3` | Teoría de Números y Aritmética Modular | `no_aplicable` | Propiedades de teoría de números y aritmética modular (teórica) |
| `§11.4` | Teoría de Grafos, Árboles y Redes | `no_aplicable` | Teoremas de grafos, árboles y redes (teórica) |
| `§11.5` | Geometría y Distancias | `no_aplicable` | Fórmulas de geometría y distancias (teórica) |
| `§11.6` | Teoría de Juegos y XOR | `no_aplicable` | Identidades de teoría de juegos y XOR (teórica) |
| `§11.7` | Reglas Lógicas de 2-SAT | `no_aplicable` | Equivalencias lógicas de 2-SAT (teórica) |
| `§11.8` | Tabla de Constantes, Primos y Módulos Frecuentes | `no_aplicable` | Tabla de constantes, primos y módulos frecuentes (teórica) |
