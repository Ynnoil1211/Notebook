# OUR BRAINS ARE DIAZ O — ICPC C++ Team Notebook (Manual Completo de Algoritmos y Estructuras)

------------------------------------------------------------------------

## Bloque 0: Guía de Decisión Rápida, Complejidad y Checklist Anti-WA

### 0.1 Selector Rápido: Señal del Problema → Técnica → Condición → Sección

| Señal en el Enunciado | Técnica Recomendada | Condición Obligatoria | Sección |
| :--- | :--- | :--- | :--- |
| «Maximizar el mínimo» / «Minimizar el máximo» | Binary Search sobre la respuesta | Predicado `check(X)` monótono (`VVVFFF` o `FFFVVV`) | §3.1 |
| Óptimo de función continua o discreta en U / $\cap$ | Ternary Search | Función **estrictamente unimodal** (sin mesetas planas) | §3.2 |
| Subarreglo más largo/corto con suma $\le S$ | Two Pointers (ventana deslizante) | Elementos **no negativos** ($a_i \ge 0$) para monotonía | §3.3 |
| Subarreglos con suma $= K$ o divisible por $M$ (con $<0$) | Prefix Sum + `safe_umap` | Arreglo estático; normalizar residuo `(s%M+M)%M` | §3.4 |
| Subarreglo contiguo de suma máxima | Algoritmo de Kadane | Al menos 1 elemento; maneja todos negativos | §3.4 |
| Mayor rectángulo / primer menor o mayor a izq/der | Monotonic Stack + Contribuciones | Desigualdad estricta de un lado y $\le$ del otro en duplicados | §3.3, §7.11 |
| Mínimo/máximo en todas las ventanas de tamaño $K$ | Monotonic Deque | Ventana de longitud monótona (extremos avanzan) | §3.3 |
| Sumar $V$ a rangos $[L, R]$ y consultar solo al final | Difference Array 1D / 2D | Todas las consultas ocurren **después** de los updates | §3.5 |
| Máximo número de intervalos disjuntos | Greedy por fin de intervalo | Ordenar por extremo derecho $r_i$ ascendente | §3.6 |
| Tareas con duración $t_i$ y plazo $d_i$ (máx. cantidad) | Greedy con Max-Heap (*Regret*) | Ordenar por $d_i$; si $\sum t > d_i$, descartar mayor $t$ | §3.6 |
| Intersección máxima o unión de intervalos | Sweep-Line 1D de eventos | Definir desempate en $x$ según intervalo cerrado/abierto | §3.7 |
| Conectividad agregando aristas / MST | DSU Clásico / Kruskal | Solo inserciones (sin borrado de aristas) | §4.1, §6.4 |
| Conectividad agregando y **borrando** aristas | `DynCon` (`RollbackDSU` offline) o `LinkCut` | `DynCon`: todas las operaciones conocidas offline | §4.1, §4.9 |
| RMQ / GCD en rango sin actualizaciones | Sparse Table | Arreglo estático y operación idempotente ($\mathcal{O}(1)$) | §4.2 |
| Suma en rango + update puntual (1D / 2D) | Fenwick Tree (BIT) | Operación invertible (`+`, `^`); 1-indexed | §4.3 |
| Operación asociativa en rango + update puntual/rango | Segment Tree / Lazy SegTree | Monoid asociativo; distributividad del lazy tag | §4.4, §4.5 |
| $k$-ésimo menor en subarreglo $[L, R]$ estático | `PersistentFreqST` | Compresión de coordenadas previa ($1 \le L \le R \le N$) | §4.5 |
| Consultas complejas en rango sin asociatividad fácil | Algoritmo de Mo | **Offline**, sin updates (o pocos), agregar/quitar $\mathcal{O}(1)$ | §4.6 |
| Cortar, pegar, revertir subarreglos dinámicamente | Implicit Treap / `rope` | Índices 0-based; `push` antes de dividir/unir | §4.7, §2.6 |
| Consultas / updates en caminos o subárboles de árbol | LCA / Euler Tour / HLD / Sack | Ver §0.3 para elegir la herramienta mínima suficiente | §4.8, §6.4 |
| Buscar patrón $P$ en texto $T$ o periodo mínimo | KMP / Z-Algorithm | Exacto y determinista en $\mathcal{O}(|T| + |P|)$ | §5.3, §5.4 |
| Palíndromo más largo centrado en cada posición | Manacher | Transforma pares e impares en $\mathcal{O}(|S|)$ | §5.4 |
| Múltiples patrones / DP prohibiendo subcadenas | Aho-Corasick | Construir `link` y `next` por BFS antes de consultar | §5.5 |
| Max/Min XOR con un conjunto de enteros | `BitTrie` / `XorBasis` | `BitTrie`: elemento individual; `XorBasis`: subconjunto | §5.5, §7.6 |
| Subcadenas distintas / LCS contigua / ocurrencias | Suffix Array + LCP o SAM | SA: $\mathcal{O}(N\log N)$; SAM: $\mathcal{O}(N\log N)$ con sort | §5.6 |
| Camino mínimo sin pesos / pesos $\{0, 1\}$ / $\ge 0$ | BFS / 0-1 BFS / Dijkstra | Dijkstra exige **pesos no negativos** ($w \ge 0$) | §6.1, §6.3 |
| Camino mínimo con pesos negativos o detectar ciclo $<0$ | Bellman-Ford / Floyd-Warshall | $N \le 2500$ (BF) o $N \le 400$ (Floyd) | §6.3 |
| Decisiones binarias con cláusulas $(a \lor b)$, $(a \Rightarrow b)$ | 2-SAT con Tarjan SCC | A lo sumo 2 literales por cláusula | §6.5 |
| Flujo máximo / Corte mínimo / Matching bipartito | Dinic / `BipartiteMVC` (Kuhn+König) | Capacidades $\ge 0$; en bipartito Dinic es $\mathcal{O}(E\sqrt{V})$ | §6.6 |
| Flujo de costo mínimo / Asignación $N \times M$ | MCMF (SPFA) / Húngaro | Sin ciclos de costo negativo; Húngaro para matriz densa | §6.6 |
| Matching máximo en grafo general (con ciclos impares) | Algoritmo de Blossom | $V \le 500$, $\mathcal{O}(V^3)$ | §6.6 |
| Grafo donde cada nodo tiene exactamente 1 salida | Grafo Funcional (Ciclos + Lifting) | `LOG = 62` si $K \le 10^{18}$ | §6.11 |
| Primalidad y factorización hasta $10^{18}$ | Miller-Rabin + Pollard's Rho | Usar multiplicación `__uint128_t` (`u128`) | §7.2 |
| Sistema de congruencias $x \equiv r_i \pmod{m_i}$ | CRT General + `extgcd` | Ensanchar a `i128` antes de multiplicar por $m_1$ | §7.3 |
| Recurrencia lineal $a_n$ para $N \le 10^{18}$ | Exponenciación de Matriz / BM | Matriz conocida $\mathcal{O}(K^3\log N)$ o inferida con BM | §7.6, §7.9 |
| Convolución de polinomios exacta o módulo $998244353$ | FFT (`double`) / NTT (`998244353`) | NTT exige primo $c\cdot 2^k+1$ y raíz primitiva $g=3$ | §7.6 |
| Subconjuntos / Asignación / TSP con $N \le 20$ | Subset DP (`dp[mask]`, `dp[mask][u]`) | $\mathcal{O}(N 2^N)$ asignación vs $\mathcal{O}(N^2 2^N)$ TSP | §8.2 |
| Respuesta para cada nodo si fuera la raíz del árbol | Rerooting DP (2 DFS) | Combinar `down[v]` de hijos con contribución exterior `up[u]` | §8.3 |
| DP cuadrática con rectas o costos convexos | CHT / D&C DP / Knuth | Verificar monotonía de pendientes/óptimos (§8.4) | §8.4 |
| Geometría exacta sin errores de redondeo | Kernel Entero `PtLL` (`i128`) | Coordenadas enteras $\le 10^9$ (productos en `i128`) | §9.1 |
| $N \le 40$ demasiado grande para $2^N$ pero $2^{N/2}$ cabe | Meet in the Middle (MITM) | Partir en $N/2$, ordenar una mitad y usar `equal_range` | §10.1 |

### 0.2 Tabla Orientativa de Complejidad según Restricciones ($N$)

> **Nota sobre límites reales:** La tabla asume un límite estándar de **1–2 segundos en C++20** ($\approx 10^8$ operaciones simples/seg). El límite real depende de: (1) **Tiempo límite** del problema, (2) **Lenguaje** (en Python dividir el presupuesto por $10\times$–$30\times$ salvo operaciones vectorizadas/C nativas), (3) **Memoria límite** (evitar tablas `ll` de $> 3\cdot 10^7$ celdas para 256 MB) y (4) **Constantes ocultas** (`set`/`map` y grafos densos tienen constante mucho mayor que vectores contiguos o Fenwick).

| Cota de $N$ | Complejidad Objetivo | Técnicas Típicas |
| :--- | :--- | :--- |
| $N \le 10$ | $\mathcal{O}(N!)$, $\mathcal{O}(N \cdot N!)$ | `next_permutation`, backtracking exhaustivo |
| $N \le 20$ | $\mathcal{O}(2^N)$, $\mathcal{O}(N \cdot 2^N)$, $\mathcal{O}(N^2 \cdot 2^N)$ | Bitmask DP (`dp[mask]`, TSP `dp[mask][last]`), SOS DP ($N \le 20$), submáscaras $\mathcal{O}(3^N)$ ($N \le 15$) |
| $N \le 40$ | $\mathcal{O}(2^{N/2} \log(2^{N/2}))$ | Meet in the Middle (§10.1), Clique Máximo con poda (§10.3) |
| $N \le 400$ | $\mathcal{O}(N^3)$ | Floyd-Warshall, Gauss, Húngaro, Blossom, DP de intervalos / Knuth $\mathcal{O}(N^2)$ |
| $N \le 3000$ | $\mathcal{O}(N^2)$, $\mathcal{O}(N^2 / 64)$ | DP 2D clásica (LCS, Edit Distance, Knapsack), `bitset` $\mathcal{O}(N^2/64)$, árboles cuadráticos |
| $N \le 10^5$ | $\mathcal{O}(N\sqrt{N})$, $\mathcal{O}(N\log^2 N)$ | Algoritmo de Mo, SQRT Decomposition, HLD + SegTree, D&C DP, Centroid Decomp |
| $N \le 2\cdot 10^5$ – $5\cdot 10^5$ | $\mathcal{O}(N\log N)$ | `sort`, Binary Search, Dijkstra, Fenwick, SegTree, LCA, FFT/NTT, Suffix Array, Convex Hull |
| $N \le 10^6$ – $5\cdot 10^6$ | $\mathcal{O}(N)$ | Two Pointers, Monotonic Stack/Deque, KMP, Z, Manacher, Criba Lineal, Prefix/Diff Array |
| $N \le 10^{18}$ | $\mathcal{O}(\log N)$, $\mathcal{O}(\sqrt{N})$, $\mathcal{O}(N^{1/4})$ | Binary Lifting, Exponenciación de Matrices, Digit DP, Miller-Rabin + Pollard's Rho |

### 0.3 Advertencias Críticas de Elección de Técnica

1. **Sliding Window / Two Pointers de suma exige monotonía**:
   - **Nunca** usar ventana deslizante estándar (`while (sum > K) sum -= a[l++]`) para suma de subarreglo si hay **números negativos**, porque avanzar `l` puede aumentar `sum` y avanzar `r` puede disminuirla.
   - *Alternativa con negativos*: para contar suma exacta $K$ usar `Prefix Sum + safe_umap` (§3.4); para suma máxima usar Kadane (§3.4); para subarreglo más corto/largo con suma $\ge K$ o $\le K$ con negativos usar `Prefix Sum + Monotonic Deque / Binary Search`.
2. **Mediana transformada a $+1 / -1$ (precisión de paridad y desigualdad)**:
   - Al probar si existe un subarreglo con mediana $\ge X$, se transforma cada elemento en $b_i = +1$ si $a_i \ge X$ y $b_i = -1$ si $a_i < X$.
   - **No usar `sum == 0` como regla general**:
     - Para la **mediana inferior** (el elemento $\lfloor (L+1)/2 \rfloor$-ésimo en orden 1-based, donde en longitud par $L=2k$ es el $k$-ésimo): la mediana es $\ge X$ si y solo si la suma del subarreglo cumple **$\text{sum} > 0$** ($\text{sum} \ge 1$).
     - Para la **mediana superior** (el elemento $\lfloor L/2 \rfloor + 1$-ésimo, donde en $L=2k$ es el $(k+1)$-ésimo): la mediana es $\ge X$ si y solo si **$\text{sum} \ge 0$** (en longitud impar equivale a $\text{sum} \ge 1$).
   - Distinguir también si el problema pide mediana $\ge X$ (búsqueda binaria sobre $X$ con $+1/-1$) o mediana **exactamente igual** a un valor $X$ sin elementos repetidos (donde $a_i > X \to +1$, $a_i < X \to -1$, $a_i = X \to 0$ y se busca subarreglo que contenga a $X$ con suma $0$ o $+1$).
3. **Consultas en Árboles — Cuándo bastan LCA/Prefijos y cuándo sí se necesita HLD**:
   - **Árbol estático + operación invertible en camino** (suma, XOR, conteo): basta guardar `pref[u]` desde la raíz y usar `LCA` (§6.4): $\text{camino}(u, v) = \text{pref}[u] + \text{pref}[v] - 2\cdot\text{pref}[\text{lca}(u, v)] + \text{val}[\text{lca}(u, v)]$ en $\mathcal{O}(\log N)$ (sin HLD).
   - **Updates en nodos + suma en subárbol** (o update de suma en camino + consulta puntual en nodo): basta **Euler Tour (`tin`/`tout`, §6.2) + Fenwick Tree (§4.3)** en $\mathcal{O}(\log N)$.
   - **Updates en subárbol + consultas en subárbol**: basta **Euler Tour (`tin`/`tout`) + Lazy SegTree (§4.5)** porque todo subárbol es el rango contiguo `[tin[u], tout[u]]`.
   - **Cuándo SÍ usar HLD (§4.8)**: cuando hay **updates dinámicos** y consultas en **camino** de una operación **no invertible** (`max`, `min`, `gcd`, matriz), o cuando hay simultáneamente **updates en camino y consultas en camino/subárbol**.

### 0.4 Checklist Anti-WA Antes de Enviar

- [ ] **Overflow entero**: ¿Usaste `ll` en sumas de prefijos, distancias de Dijkstra, costos de DP y conteos de pares `1LL * n * (n - 1) / 2`? ¿Usaste `i128` antes de multiplicar en CRT, geometría entera (`PtLL`) o `mul_mod64` (`u128`)?
- [ ] **Indexación ($0$-based vs $1$-based)**: Fenwick, DSU, LCA, HLD y grafos suelen ser `1..N`; Segment Tree, Sparse Table, Strings y Geometría suelen ser `0..N-1`. ¿Ajustaste `--u, --v` al leer?
- [ ] **Casos múltiples (`T` testcases) y reinicio**: ¿Limpiaste `adj[1..n]`, `vis[1..n]` y contadores entre casos? **Prohibido** hacer `memset(arr, 0, sizeof(arr))` sobre un arreglo global de $2\cdot 10^5$ dentro de cada caso cuando $\sum N \le 2\cdot 10^5$ (causa TLE $\mathcal{O}(T \cdot N_{\max})$); usar `fill(arr, arr + n + 1, 0)`.
- [ ] **Constante `INF`**: ¿Es suficientemente grande (`1e18` para `ll`, no `1e9`) pero protegida contra overflow al hacer `dist[u] + w` (`if (dist[u] < INF)`)?
- [ ] **Negativos en módulo e inversos**: En C++ `(-a) % MOD` es negativo; usar `(x % MOD + MOD) % MOD`. Antes de usar `inv_fermat(a, p)`, verificar que $p$ sea primo y $a \not\equiv 0 \pmod p$; para módulo compuesto usar `inv_extgcd` solo si $\gcd(a, m) = 1$.
- [ ] **Recursión y pila**: En grafos/árboles con $N = 2\cdot 10^5$, no pasar vectores por valor en el DFS. En Python, configurar `sys.setrecursionlimit(300000)` o reescribir el DFS iterativo.
- [ ] **Precisión en punto flotante**: Usar `ld` (`long double`), imprimir con `fixed << setprecision(10)`, comparar con `eps_eq`/`eps_lt`, y **nunca** pasar un comparador aproximado `a + EPS < b` a `std::set`/`std::map`.
- [ ] **Estructura vacía y bordes**: Verificar $N=1$, $K=0$, grafo desconectado, llamar a `ms.find(x)` antes de `ms.erase(it)` en `multiset`, y comprobar `!s.empty()` antes de `*s.begin()`, `*s.rbegin()` o `pq.top()`.
