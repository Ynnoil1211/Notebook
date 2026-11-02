------------------------------------------------------------------------

## Bloque 6: Grafos, Árboles, Conectividad y Flujos

### 6.1 Recorridos BFS: Grid BFS (Flood Fill), Multi-Source BFS (Aristas Invertidas) y 0-1 BFS

#### 6.1.1 Grid BFS / Flood Fill con Identificador de Componentes (`flood_fill_bfs`)

- **Cuándo usar:** Etiquetar componentes conexas en una grilla $R \times C$ con movimiento en 4 direcciones (`dx4, dy4`) y precalcular una métrica por componente (`ans_comp[id]`) para responder consultas en $\mathcal{O}(1)$.
- **Cuándo no usar / alternativa:** Si las celdas cambian dinámicamente (se abren paredes online), usar **DSU** (§4.1.1) aplanando `(r, c) -> r * m + c`.
- **Contrato:** Grilla 0-indexed de tamaño `n_g x m_g` en `grid_g`. Inicializar `comp_g` en `-1` antes de recorrer las celdas `.`.
- **Complejidad:** $\mathcal{O}(R \cdot C)$ tiempo y memoria.

-- flood_fill_bfs.cpp

- **Lectura de entrada y uso:** Leer `n_g, m_g` y las filas en `grid_g[0..n_g-1]`, inicializar `comp_g` en `-1` y lanzar BFS por cada celda `'.'` libre:
```cpp
cin >> n_g >> m_g;
forn(r, n_g) {
  cin >> grid_g[r];
  forn(c, m_g) comp_g[r][c] = -1;
}
int id = 0;
forn(r, n_g) forn(c, m_g)
  if (grid_g[r][c] == '.' && comp_g[r][c] == -1)
    flood_fill_bfs(r, c, id++); // ans_comp[comp_g[r][c]]
```
- **Trampas y adaptaciones:** (1) Marcar `comp_g[nr][nc] = id` **al encolar** (no al desencolar) para no insertar la misma celda múltiples veces y provocar TLE/MLE.

#### 6.1.2 Multi-Source BFS y Aristas Invertidas (`multi_source_bfs`)

- **Cuándo usar:** Hallar la distancia mínima desde cada nodo hacia el nodo más cercano de un conjunto de fuentes/salidas $S$ en un grafo sin pesos.
- **Cuándo no usar / alternativa:** Si las aristas tienen pesos positivos arbitrarios, inicializar un **Dijkstra multi-fuente** (§6.3.1) metiendo todas las fuentes con distancia `0` al min-heap.
- **Contrato:** Nodos 1-indexed (`1..n`). Si el grafo es dirigido y quieres ir *hacia* las salidas $S$, cargar las aristas al revés (`v -> u`) en `adj_rev`. `dist_ms[u] = -1` si es inalcanzable.
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- multi_source_bfs.cpp

- **Lectura de entrada y uso:** Limpiar `adj_rev[1..n]`, leer las $K$ fuentes (1-based) y las $M$ aristas:
```cpp
int n, m, k; cin >> n >> m >> k;
for1(i, n) adj_rev[i].clear();
vi src(k); forn(i, k) cin >> src[i];
forn(i, m) {
  int u, v; cin >> u >> v;
  adj_rev[v].pb(u); // + adj_rev[u].pb(v) si es no dirigido
}
multi_source_bfs(src, n); // dist_ms[1..n]
```
- **Trampas y adaptaciones:** (1) No correr un BFS separado desde cada fuente ($\mathcal{O}(K(V+E))$ da TLE); meter las $K$ fuentes juntas en la cola inicial.

#### 6.1.3 0-1 BFS con `std::deque` y Reconstrucción (`bfs_01`, `get_path_01`)

- **Cuándo usar:** Camino más corto desde `s` cuando todos los pesos de arista son exclusivamente $0$ o $1$ (ej. laberintos donde romper pared o cambiar dirección cuesta $1$ y avanzar cuesta $0$).
- **Cuándo no usar / alternativa:** Si hay pesos mayores que $1$ (como $\{0, 1, 2\}$), el `deque` pierde monotonía; usar **Dijkstra** (§6.3.1).
- **Contrato:** Nodos 1-indexed (`1..n`). Arista de peso `0` se inserta con `push_front`, peso `1` con `push_back`.
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- bfs_01.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); limpiar `adj01[1..n]`, leer aristas con $w \in \{0, 1\}$ y llamar `bfs_01(s, n)`:
```cpp
int n, m, s, t; cin >> n >> m >> s >> t;
for1(i, n) adj01[i].clear();
forn(i, m) {
  int u, v, w; cin >> u >> v >> w;
  adj01[u].pb({v, w}); adj01[v].pb({u, w});
}
bfs_01(s, n); // dist01[t], ruta = get_path_01(t)
```
- **Trampas y adaptaciones:** (1) Un mismo nodo puede entrar al `deque` primero con costo $+1$ y luego mejorarse con costo $+0$; verificar `dist01[u] + w < dist01[v]` antes de relajar.

------------------------------------------------------------------------

### 6.2 Patrones DFS: Grafo Bipartito, Ciclos, Euler Tour (`tin`/`tout`), DFS en Complemento y Conteo de Triángulos

#### 6.2.1 Grafo Bipartito y 2-Coloreo por DFS (`dfs_bipartite`)

- **Cuándo usar:** Determinar si un grafo no dirigido carece de ciclos de longitud impar y particionar sus nodos en dos conjuntos independientes (colores `1` y `2`).
- **Cuándo no usar / alternativa:** Si las aristas se agregan online y quieres mantener bipartición con deshacer, usar **DSU con paridad / Rollback** (§4.1.2).
- **Contrato:** Nodos 1-indexed (`1..n`) en `adj_dfs`. Inicializar `col_bip[1..n] = 0` y lanzar `dfs_bipartite(i, 1)` desde cada componente no visitada.
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- dfs_bipartite.cpp

- **Lectura de entrada y uso:** Limpiar `adj_dfs[1..n]` y `col_bip[1..n] = 0`, cargar aristas no dirigidas y probar cada componente:
```cpp
int n, m; cin >> n >> m;
for1(i, n) { adj_dfs[i].clear(); col_bip[i] = 0; }
forn(i, m) {
  int u, v; cin >> u >> v;
  adj_dfs[u].pb(v); adj_dfs[v].pb(u);
}
bool ok = true;
for1(i, n) if (!col_bip[i]) ok &= dfs_bipartite(i, 1);
```
- **Trampas y adaptaciones:** (1) Iterar `for (int i = 1; i <= n; ++i)` porque el grafo puede ser desconexo.

#### 6.2.2 Detección de Ciclo en Grafo Dirigido con 3 Estados (`dfs_dir_cycle`)

- **Cuándo usar:** Detectar si un grafo dirigido contiene algún ciclo dirigido usando coloreo de 3 estados (`0` no visitado, `1` en la pila recursiva actual, `2` cerrado).
- **Cuándo no usar / alternativa:** Si además necesitas el orden topológico o comprimir los ciclos en un DAG, usar `topological_sort` (§6.5.1) o `TarjanSCC` (§6.5.3).
- **Contrato:** Nodos 1-indexed (`1..n`) en `adj_dfs`. Un ciclo dirigido existe si y solo si se encuentra una arista hacia un nodo con `state_dir[v] == 1`. Dependencias: `adj_dfs` (§6.2.1).
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- dfs_cycle.cpp

- **Lectura de entrada y uso:** Limpiar `adj_dfs[1..n]` y `state_dir[1..n] = 0`, cargar aristas dirigidas y buscar ciclo desde cada nodo no visitado:
```cpp
int n, m; cin >> n >> m;
for1(i, n) { adj_dfs[i].clear(); state_dir[i] = 0; }
forn(i, m) { int u, v; cin >> u >> v; adj_dfs[u].pb(v); }
bool has_cyc = false;
for1(i, n) if (!state_dir[i]) has_cyc |= dfs_dir_cycle(i);
```
- **Trampas y adaptaciones:** (1) En grafos dirigidos, ver un nodo ya visitado con `state_dir[v] == 2` (*cross-edge* o *forward-edge*) **no** es un ciclo; solo `state_dir[v] == 1` (*back-edge*) indica ciclo.

#### 6.2.3 Euler Tour en Árboles y Ancestralidad en $\mathcal{O}(1)$ (`dfs_euler`, `is_ancestor`)

- **Cuándo usar:** Aplanar un árbol en un arreglo contiguo donde el subárbol de cada nodo `u` ocupa exactamente el intervalo `[tin_e[u], tout_e[u]]`, permitiendo consultar en $\mathcal{O}(1)$ si `u` es ancestro de `v` o actualizar/consultar subárboles con Fenwick/Segment Tree.
- **Cuándo no usar / alternativa:** Si las consultas son sobre **caminos** entre `u` y `v` con actualizaciones de peso, el rango `[tin, tout]` no representa caminos; usar **HLD** (§4.8).
- **Contrato:** Árbol 1-indexed en `adj_dfs`. Reiniciar `timer_e = 0` antes de llamar `dfs_euler(root, 0)`. Dependencias: `adj_dfs` (§6.2.1).
- **Complejidad:** $\mathcal{O}(N)$ preprocesamiento y $\mathcal{O}(1)$ por consulta `is_ancestor`.

-- euler_tour.cpp

- **Lectura de entrada y uso:** Cargar el árbol en `adj_dfs[1..n]`, reiniciar `timer_e = 0` y llamar `dfs_euler(1, 0)` (subárbol de $u$: rango 1-based `[tin_e[u], tout_e[u]]`):
```cpp
int n; cin >> n; timer_e = 0;
for1(i, n) adj_dfs[i].clear();
forn(i, n - 1) {
  int u, v; cin >> u >> v;
  adj_dfs[u].pb(v); adj_dfs[v].pb(u);
}
dfs_euler(1, 0); // is_ancestor(u, v) en O(1)
```
- **Trampas y adaptaciones:** (1) Con este Euler Tour de $N$ tiempos (`++timer_e` solo al entrar), el tamaño del subárbol de `u` es `tout_e[u] - tin_e[u] + 1`.

#### 6.2.4 Componentes Conexas en el Grafo Complemento (`complement_components`) [opcional]

- **Cuándo usar:** Hallar las componentes conexas de un grafo donde existen todas las $\binom{N}{2}$ aristas **excepto** $M$ aristas prohibidas dadas en la entrada ($N, M \le 2 \cdot 10^5$).
- **Cuándo no usar / alternativa:** Si $N \le 2000$, construir la matriz de adyacencia explícita y hacer BFS normal en $\mathcal{O}(N^2)$ es más simple.
- **Contrato:** Nodos 1-indexed (`1..n`). `forbidden_adj[u]` contiene los vecinos prohibidos de `u`. Mantiene los nodos aún no visitados en un `std::set<int>` y salta sobre los prohibidos.
- **Complejidad:** $\mathcal{O}((N + M) \log N)$ tiempo y $\mathcal{O}(N + M)$ memoria.

-- complement_bfs.cpp

- **Trampas y adaptaciones:** (1) El algoritmo es $\mathcal{O}((N + M)\log N)$ porque cada vez que `binary_search` da `true` se paga una de las $2M$ aristas prohibidas, y cuando da `false` el nodo se borra definitivamente del `set`.

#### 6.2.5 Conteo de Triángulos ($K_3$) por Orientación de Grado (`count_triangles`) [opcional]

- **Cuándo usar:** Contar exactamente cuántos ciclos de longitud 3 ($u - v - w - u$) existen en un grafo simple no dirigido con $N, M \le 2 \cdot 10^5$.
- **Cuándo no usar / alternativa:** Si el grafo tiene multiaristas o lazos, limpiar primero `edges` con `sort` + `unique` y descartar `u == v`.
- **Contrato:** Nodos 1-indexed (`1..n`). Orienta cada arista desde el nodo de menor grado hacia el de mayor grado (desempatando por índice), garantizando grado de salida $\le \sqrt{2M}$ en el DAG resultante.
- **Complejidad:** $\mathcal{O}(M \sqrt{M})$ tiempo y $\mathcal{O}(N + M)$ memoria.

-- count_triangles.cpp

- **Trampas y adaptaciones:** (1) La misma orientación por grado sirve para enumerar ciclos de longitud 4 ($C_4$) en $\mathcal{O}(M \sqrt{M})$.

------------------------------------------------------------------------

### 6.3 Caminos Más Cortos: Dijkstra, Bellman-Ford / SPFA (Ciclo Negativo) y Floyd-Warshall

#### 6.3.1 Dijkstra con Min-Heap y Reconstrucción (`dijkstra`, `get_path_dij`)

- **Cuándo usar:** Caminos más cortos desde una fuente `s` en grafos dirigidos o no dirigidos con pesos **no negativos** ($w_e \ge 0$).
- **Cuándo no usar / alternativa:** Si existe alguna arista con peso negativo ($w_e < 0$), Dijkstra falla; usar **Bellman-Ford** (§6.3.2).
- **Contrato:** Nodos 1-indexed (`1..n`) en `adj_dij`. Inicializa `dist_dij` con `INF` ($10^{18}$). `get_path_dij(t)` devuelve `{}` si `t` es inalcanzable.
- **Complejidad:** $\mathcal{O}((V + E) \log V)$ tiempo y $\mathcal{O}(V + E)$ memoria.

-- dijkstra.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); limpiar `adj_dij[1..n]`, cargar aristas `{v, w}` y llamar `dijkstra(s, n)`:
```cpp
int n, m, s, t; cin >> n >> m >> s >> t;
for1(i, n) adj_dij[i].clear();
forn(i, m) {
  int u, v; ll w; cin >> u >> v >> w;
  adj_dij[u].pb({v, w}); // + adj_dij[v].pb({u, w})
}
dijkstra(s, n); // dist_dij[t], ruta = get_path_dij(t)
```
- **Trampas y adaptaciones:** (1) La guarda `if (d > dist_dij[u]) continue;` es obligatoria para mantener complejidad $\mathcal{O}(E \log V)$ sin `decrease-key`.

#### 6.3.2 Bellman-Ford con Reconstrucción de Ciclo Negativo (`bellman_ford_cycle`)

- **Cuándo usar:** Caminos más cortos con pesos negativos y detección/reconstrucción explícita de un ciclo negativo alcanzable desde `s`.
- **Cuándo no usar / alternativa:** Si todos los pesos son $\ge 0$, usar `dijkstra` (§6.3.1).
- **Contrato:** Nodos 1-indexed (`1..n`). Satura `d[e.v] = max(-INF, d[e.u] + e.w)` para evitar overflow a `LLONG_MIN`. Si retorna `true`, `cycle` contiene los vértices del ciclo negativo en orden dirigido.
- **Complejidad:** $\mathcal{O}(V \cdot E)$ tiempo y $\mathcal{O}(V + E)$ memoria.

-- bellman_ford.cpp

- **Lectura de entrada y uso:** Leer las $M$ aristas dirigidas en `vector<BFEdge> ed(m)` (nodos 1-based) y llamar `bellman_ford_cycle`:
```cpp
int n, m, s; cin >> n >> m >> s;
vector<BFEdge> ed(m);
forn(i, m) cin >> ed[i].u >> ed[i].v >> ed[i].w;
vll d; vi cyc;
bool has_neg = bellman_ford_cycle(s, n, ed, d, cyc);
```
- **Trampas y adaptaciones:** (1) Retroceder `n` veces con `x = p[x]` antes de reconstruir el ciclo es indispensable porque el último nodo relajado en la iteración $N$ puede estar en una cola que sale del ciclo negativo y no dentro del ciclo mismo; (2) para detectar cualquier ciclo negativo global sin importar `s`, inicializar `d[1..n] = 0`.

#### 6.3.3 Floyd-Warshall Todos contra Todos (`floyd_warshall`)

- **Cuándo usar:** Distancias mínimas entre **todos los pares** de vértices $(i, j)$ en grafos pequeños ($V \le 400$–$500$), o clausura transitiva.
- **Cuándo no usar / alternativa:** Si $V > 500$ y los pesos son no negativos, correr `dijkstra` desde cada nodo cuesta $\mathcal{O}(V(V + E)\log V)$.
- **Contrato:** Matriz `d` 1-indexed de tamaño `(n + 1) x (n + 1)`, inicializada con `d[i][i] = 0` y `d[i][j] = INF` si no hay arista. Al finalizar, `d[i][i] < 0` indica que `i` pertenece a un ciclo negativo.
- **Complejidad:** $\mathcal{O}(V^3)$ tiempo y $\mathcal{O}(V^2)$ memoria.

-- floyd_warshall.cpp

- **Lectura de entrada y uso:** Inicializar matriz `d[1..n][1..n]` con `INF` y `d[i][i] = 0`, cargar aristas con `min` y llamar `floyd_warshall(n, d)`:
```cpp
int n, m; cin >> n >> m;
vector<vll> d(n + 1, vll(n + 1, INF));
for1(i, n) d[i][i] = 0;
forn(i, m) {
  int u, v; ll w; cin >> u >> v >> w;
  d[u][v] = min(d[u][v], w); // y d[v][u] si es no dirigido
}
floyd_warshall(n, d);
```
- **Trampas y adaptaciones:** (1) El nodo intermedio `k` **siempre** debe ser el bucle más externo (`for k ... for i ... for j`); (2) al leer la entrada, hacer `d[u][v] = min(d[u][v], w)` por si vienen aristas paralelas.

------------------------------------------------------------------------

### 6.4 Árboles: Diámetro (Doble BFS), MST (Kruskal y Prim), LCA Binary Lifting y Centroid Decomposition

#### 6.4.1 Diámetro de un Árbol por Doble BFS (`bfs_farthest`, `get_diameter_path`)

- **Cuándo usar:** Hallar el camino simple más largo (y su centro) en un árbol sin pesos (o con pesos no negativos).
- **Cuándo no usar / alternativa:** Si el árbol tiene aristas de **peso negativo**, el truco de los 2 BFS falla; usar **DP en árboles** (§8.3) guardando las dos mayores ramas hacia abajo desde cada nodo.
- **Contrato:** Árbol 1-indexed (`1..n`) en `adj_tr`. `get_diameter_path(n)` devuelve todos los nodos en la ruta del diámetro; su longitud en aristas es `sz(path) - 1`.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y memoria.

-- tree_diameter.cpp

- **Lectura de entrada y uso:** Cargar las $N - 1$ aristas en `adj_tr[1..n]` y llamar `get_diameter_path(n)`:
```cpp
int n; cin >> n;
for1(i, n) adj_tr[i].clear();
forn(i, n - 1) {
  int u, v; cin >> u >> v;
  adj_tr[u].pb(v); adj_tr[v].pb(u);
}
vi path = get_diameter_path(n); // diam = sz(path) - 1
```
- **Trampas y adaptaciones:** (1) El centro del árbol (nodo que minimiza la máxima distancia al resto) es `path[sz(path) / 2]`.

#### 6.4.2 Árbol Generador Mínimo de Kruskal (`kruskal_mst`)

- **Cuándo usar:** Hallar el subgrafo conexo acíclico de menor peso total (MST) en un grafo no dirigido pesado.
- **Cuándo no usar / alternativa:** Si el grafo es dirigido y buscas una arborescencia desde una raíz fija, Kruskal no aplica; usar `directed_mst` (§6.7).
- **Contrato:** Nodos 1-indexed (`1..n`). Retorna el costo total del MST y llena `mst` con las $N - 1$ aristas elegidas, o retorna `-1` si el grafo es desconexo. Dependencias: `DSU` (§4.1.1).
- **Complejidad:** $\mathcal{O}(E \log E)$ tiempo y $\mathcal{O}(V + E)$ memoria.

-- mst_kruskal_prim.cpp

- **Lectura de entrada y uso:** Leer $M$ aristas en `vector<KrEdge> ed(m)` (nodos 1-based) y llamar `kruskal_mst(n, ed, mst)`:
```cpp
int n, m; cin >> n >> m;
vector<KrEdge> ed(m), mst;
forn(i, m) cin >> ed[i].u >> ed[i].v >> ed[i].w;
ll cost = kruskal_mst(n, ed, mst); // -1 si es desconexo
```
- **Trampas y adaptaciones:** (1) Para Maximum Spanning Tree, invertir el comparador (`w > o.w`); (2) si el problema pide un *Minimum Spanning Forest*, ignorar el chequeo `dsu.num_comp == 1` y usar `cost` directamente.

#### 6.4.3 LCA, $K$-ésimo Ancestro y Distancia por Binary Lifting (`LCA`)

- **Cuándo usar:** Responder consultas de Ancestro Común Más Bajo (`lca(u, v)`), $k$-ésimo ancestro (`kth_ancestor(u, k)`) y distancia en aristas (`dist(u, v)`) en un árbol estático.
- **Cuándo no usar / alternativa:** Si los pesos o valores de los nodos cambian dinámicamente y necesitas consultar agregados en caminos, usar **HLD** (§4.8).
- **Contrato:** Árbol 1-indexed (`1..n`). El padre de la raíz `root` se define como `root` mismo, por lo que pedir un ancestro más arriba de la raíz se satura en `root`.
- **Complejidad:** Construcción $\mathcal{O}(N \log N)$ tiempo y memoria; cada consulta en $\mathcal{O}(\log N)$.

-- lca_binary_lifting.cpp

- **Lectura de entrada y uso:** Leer las $N - 1$ aristas en `vector<vi> adj(n + 1)` (1-based) y construir `LCA lca(n, root, adj)`:
```cpp
int n, q; cin >> n >> q;
vector<vi> adj(n + 1);
forn(i, n - 1) {
  int u, v; cin >> u >> v; adj[u].pb(v); adj[v].pb(u);
}
LCA lca(n, 1, adj); // lca.lca(u, v), lca.dist(u, v)
```
- **Trampas y adaptaciones:** (1) Para encontrar el nodo en el camino de `u` a `v` a distancia `d` desde `u` ($0 \le d \le \text{dist}(u, v)$): sea $w = \text{lca}(u, v)$; si $d \le \text{dep}[u] - \text{dep}[w]$ es `kth_ancestor(u, d)`, si no es `kth_ancestor(v, dist(u, v) - d)`.

#### 6.4.4 Descomposición por Centroides (`CentroidDecomp`) [opcional]

- **Cuándo usar:** Contar caminos en un árbol que cumplen una propiedad global (longitud exacta $K$, XOR igual a $X$, etc.) o mantener el nodo marcado más cercano en $\mathcal{O}(\log N)$ por actualización/consulta.
- **Cuándo no usar / alternativa:** Si las consultas son simplemente sobre un camino entre `u` y `v` dados en la entrada, usar `LCA` (§6.4.3) o `HLD` (§4.8).
- **Contrato:** Árbol 1-indexed (`1..n`). Construye el árbol de centroides de altura $\le \lceil \log_2 N \rceil$, donde `par_c[c]` es el padre de `c` en el árbol de centroides (`0` en la raíz).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- centroid_decomposition.cpp

- **Trampas y adaptaciones:** (1) En el árbol de centroides, cualquier camino original $u \leftrightarrow v$ pasa por su ancestro común más bajo en el árbol de centroides, que está a lo sumo a $\mathcal{O}(\log N)$ saltos por `par_c`.

------------------------------------------------------------------------

### 6.5 Conectividad, DAGs y 2-SAT: Topological Sort, Puentes/Articulación, SCC (Tarjan) y 2-SAT

#### 6.5.1 Orden Topológico de Kahn por BFS (`topological_sort`)

- **Cuándo usar:** Obtener una permutación lineal de los nodos de un DAG donde toda arista $u \to v$ cumple que $u$ aparece antes que $v$ (ideal para DP sobre DAGs o verificar si existe un ciclo dirigido).
- **Cuándo no usar / alternativa:** Si el grafo tiene ciclos y necesitas procesarlo por bloques, primero comprimirlo en un DAG con `TarjanSCC` (§6.5.3).
- **Contrato:** Nodos 1-indexed (`1..n`). Si `sz(order) < n`, el grafo dirigido **tiene al menos un ciclo** y no admite orden topológico.
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- toposort.cpp

- **Lectura de entrada y uso:** Leer aristas dirigidas $u \to v$ en `adj[1..n]` y verificar `sz(ord) == n` (si es menor, hay ciclo):
```cpp
int n, m; cin >> n >> m;
vector<vi> adj(n + 1);
forn(i, m) { int u, v; cin >> u >> v; adj[u].pb(v); }
vi ord = topological_sort(n, adj); // ciclo si sz(ord) < n
```
- **Trampas y adaptaciones:** (1) Si el problema pide el **orden topológico lexicográficamente menor**, cambiar `queue<int> q` por `priority_queue<int, vi, greater<int>> q` ($\mathcal{O}((V+E)\log V)$).

#### 6.5.2 Puentes y Puntos de Articulación de Tarjan (`BridgesAndCuts`)

- **Cuándo usar:** Encontrar todas las aristas cuya eliminación desconecta el grafo (**puentes**, `low[v] > tin[u]`) y los vértices cuya eliminación desconecta el grafo (**puntos de articulación**, `low[v] >= tin[u]`).
- **Cuándo no usar / alternativa:** Si necesitas el árbol comprimido de componentes biconexas por vértices, usar `BlockCutTree` (§6.8).
- **Contrato:** Nodos 1-indexed (`1..n`). Maneja correctamente **aristas paralelas** ignorando únicamente la primera ocurrencia de la arista hacia el padre (`skipped_parent`), e ignora auto-bucles (`v == u`).
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- bridges_and_cuts.cpp

- **Lectura de entrada y uso:** Leer grafo no dirigido en `adj[1..n]` y construir `BridgesAndCuts bc(n, adj)`:
```cpp
int n, m; cin >> n >> m;
vector<vi> adj(n + 1);
forn(i, m) {
  int u, v; cin >> u >> v; adj[u].pb(v); adj[v].pb(u);
}
BridgesAndCuts bc(n, adj); // bc.bridges, bc.is_cut[1..n]
```
- **Trampas y adaptaciones:** (1) La raíz del árbol DFS (`p == -1`) es punto de articulación si y solo si tiene `children > 1` en el árbol DFS (no en la lista de adyacencia).

#### 6.5.3 Componentes Fuertemente Conexas de Tarjan (`TarjanSCC`)

- **Cuándo usar:** Particionar un grafo dirigido en sus Componentes Fuertemente Conexas maximales (`comp_id[u]`) en una sola pasada DFS para condensarlo en un DAG.
- **Cuándo no usar / alternativa:** Si el grafo es no dirigido, las componentes se hallan con BFS/DFS o `DSU` (§4.1.1).
- **Contrato:** Nodos 1-indexed (`1..n`). `scc_cnt` es la cantidad de SCCs (`1..scc_cnt`). Tarjan emite las SCCs en **orden topológico inverso** (los pozos/sinks reciben menor `comp_id` y las fuentes mayor `comp_id`).
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- tarjan_scc.cpp

- **Lectura de entrada y uso:** Leer grafo dirigido en `adj[1..n]` y construir `TarjanSCC scc(n, adj)`:
```cpp
int n, m; cin >> n >> m;
vector<vi> adj(n + 1);
forn(i, m) { int u, v; cin >> u >> v; adj[u].pb(v); }
TarjanSCC scc(n, adj); // scc.scc_cnt, scc.comp_id[1..n]
```
- **Trampas y adaptaciones:** (1) Al actualizar `low[u]` con un vecino ya visitado `v`, verificar obligatoriamente `if (in_st[v])` (si `v` ya salió de la pila en otra SCC cerrada, ignorarlo).

#### 6.5.4 Solucionador 2-SAT en Tiempo Lineal (`TwoSAT`) [opcional]

- **Cuándo usar:** Asignar valores booleanos a $N$ variables ($x_0, \dots, x_{N-1}$) sujetas a cláusulas de disyunción de 2 literales $(a \lor b)$, implicación $(a \Rightarrow b)$, igualdad $(a = b)$ o desigualdad $(a \neq b)$.
- **Cuándo no usar / alternativa:** Si las cláusulas tienen 3 o más literales (3-SAT), el problema es NP-completo.
- **Contrato:** Variables **0-indexed** (`0..n-1`). `solve()` retorna `false` si alguna variable e inverso caen en la misma SCC; si retorna `true`, la asignación válida queda en `ans[0..n-1]`. Dependencias: `TarjanSCC` (§6.5.3).
- **Complejidad:** $\mathcal{O}(N + M)$ tiempo y memoria.

-- twosat.cpp

- **Lectura de entrada y uso:** Variables **0-based** (`0..n-1`); agregar cláusulas y llamar `sat.solve()`:
```cpp
TwoSAT sat(n);
sat.add_or(u, true, v, false); // x_u OR !x_v
sat.add_imp(u, true, v, true); // x_u => x_v
if (sat.solve()) { /* sat.ans[0..n-1] tiene asignacion */ }
```
- **Trampas y adaptaciones:** (1) Para prohibir que $x$ e $y$ sean ambas verdaderas ($\neg(x \land y)$), llamar `add_or(x, false, y, false)`.

------------------------------------------------------------------------

### 6.6 Flujos y Matching: Dinic, Min-Cost Max-Flow (MCMF), Kuhn + König, Algoritmo Húngaro y Blossom

#### 6.6.1 Flujo Máximo y Corte Mínimo de Dinic (`Dinic`)

- **Cuándo usar:** Calcular el flujo máximo $s \to t$ y reconstruir el corte mínimo $s-t$ (`left_of_min_cut`), emparejamiento bipartito en $\mathcal{O}(E\sqrt{V})$, o problemas de selección de proyectos (*Project Selection*).
- **Cuándo no usar / alternativa:** Si cada arista tiene además un costo por unidad de flujo y buscas costo mínimo, usar `MCMF` (§6.6.2).
- **Contrato:** Nodos 1-indexed (`1..n`). Para aristas no dirigidas de capacidad `cap`, cambiar el `0` de la arista reversa en `add_edge` por `cap`. Llamar `left_of_min_cut(u)` después de `max_flow()`.
- **Complejidad:** $\mathcal{O}(V^2 E)$ en redes generales; $\mathcal{O}(E \sqrt{V})$ en redes unitarias / matching bipartito.

-- dinic.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); crear `Dinic mf(n, s, t)`, agregar aristas `(u, v, cap)` y llamar `mf.max_flow()`:
```cpp
int n, m, s, t; cin >> n >> m >> s >> t;
Dinic mf(n, s, t);
forn(i, m) {
  int u, v; ll c; cin >> u >> v >> c;
  mf.add_edge(u, v, c);
}
ll flow = mf.max_flow(); // corte min: mf.left_of_min_cut(u)
```
- **Trampas y adaptaciones:** (1) El puntero `int& cid = ptr[u]` (*current arc optimization*) es indispensable: sin referencia `&`, Dinic degrada a $\mathcal{O}(V E^2)$ y da TLE.

#### 6.6.2 Flujo Máximo de Costo Mínimo (`MCMF`) [opcional]

- **Cuándo usar:** Enviar flujo desde `s` hasta `t` (o a lo sumo `max_f` unidades) minimizando el costo total $\sum f_e \cdot \text{cost}_e$ en una red sin ciclos de costo negativo.
- **Cuándo no usar / alternativa:** Si solo necesitas asignación 1-a-1 entre $N$ filas y $M$ columnas en grafo bipartito denso, el `hungarian` (§6.6.4) es más rápido y corto.
- **Contrato:** Nodos 1-indexed (`1..n`). La arista reversa se crea automáticamente con capacidad `0` y costo `-cost`. `solve(s, t, max_f)` retorna `{flujo_enviado, costo_minimo}`.
- **Complejidad:** $\mathcal{O}(F \cdot k E)$ práctico con SPFA (donde $F$ es el flujo total).

-- mcmf.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); agregar aristas `(u, v, cap, cost)` y llamar `mcmf.solve(s, t)`:
```cpp
int n, m, s, t; cin >> n >> m >> s >> t;
MCMF mcmf(n);
forn(i, m) {
  int u, v; ll cap, cost; cin >> u >> v >> cap >> cost;
  mcmf.add_edge(u, v, cap, cost);
}
auto [flow, min_cost] = mcmf.solve(s, t);
```
- **Trampas y adaptaciones:** (1) Para **Max-Cost Max-Flow** en un DAG, insertar los costos negados (`-cost`) y negar el costo retornado.

#### 6.6.3 Matching Bipartito de Kuhn y Reconstrucción de König (`BipartiteMVC`)

- **Cuándo usar:** Emparejamiento máximo en grafos bipartitos pequeños/medianos ($N_1 \cdot E \le 10^7$) y reconstrucción explícita de los vértices del **Minimum Vertex Cover** (o Maximum Independent Set) mediante el Teorema de König.
- **Cuándo no usar / alternativa:** Si $N_1, N_2 \approx 5 \cdot 10^4$, Kuhn $\mathcal{O}(N_1 E)$ puede dar TLE; usar `Dinic` (§6.6.1) en $\mathcal{O}(E\sqrt{V})$.
- **Contrato:** Lado izquierdo `1..n1`, lado derecho `1..n2`. `min_vertex_cover()` retorna `{cov_l, cov_r}` donde $|\text{cov}_l| + |\text{cov}_r| = \text{max\_matching}()$.
- **Complejidad:** $\mathcal{O}(N_1 \cdot E)$ para el matching; $\mathcal{O}(N_1 + N_2 + E)$ para la reconstrucción de König.

-- bipartite_mvc.cpp

- **Lectura de entrada y uso:** Lado izq `1..n1`, lado der `1..n2`; agregar aristas `bip.add_edge(u, v)` y llamar `bip.max_matching()` o `bip.min_vertex_cover()`:
```cpp
int n1, n2, m; cin >> n1 >> n2 >> m;
BipartiteMVC bip(n1, n2);
forn(i, m) { int u, v; cin >> u >> v; bip.add_edge(u, v); }
auto [cov_l, cov_r] = bip.min_vertex_cover();
// Pareja de u: bip.mt_l[u]; de v: bip.mt_r[v] (0 si libre)
```
- **Trampas y adaptaciones:** (1) El **Maximum Independent Set** bipartito es exactamente el complemento del Minimum Vertex Cover: los nodos con `vis_l[u] == 1` a la izquierda y `vis_r[v] == 0` a la derecha.

#### 6.6.4 Algoritmo Húngaro de Asignación de Costo Mínimo (`hungarian`) [opcional]

- **Cuándo usar:** Asignar cada una de las $N$ filas a una columna distinta entre $M$ columnas ($N \le M \le 500$) minimizando la suma de costos $\sum_{i=1}^N a[i][\text{match}[i]]$.
- **Cuándo no usar / alternativa:** Si el grafo bipartito es muy disperso con capacidades $> 1$, usar `MCMF` (§6.6.2).
- **Contrato:** Matriz `a` **1-indexed** de dimensión `(n + 1) x (m + 1)` con `n <= m`.admite costos negativos o cero. Retorna `{costo_minimo, match_row}`.
- **Complejidad:** $\mathcal{O}(N^2 M)$ tiempo y $\mathcal{O}(N M)$ memoria.

-- hungarian.cpp

- **Lectura de entrada y uso:** Leer matriz **1-based** `a[1..n][1..m]` con $n \le m$ y llamar `hungarian(n, m, a)`:
```cpp
int n, m; cin >> n >> m;
vector<vll> a(n + 1, vll(m + 1, 0));
for1(i, n) for1(j, m) cin >> a[i][j];
auto [min_cost, col_of_row] = hungarian(n, m, a);
```
- **Trampas y adaptaciones:** (1) Si $N > M$, transponer la matriz antes de llamar; (2) para **maximizar** la ganancia total, negar todas las entradas `a[i][j] = -a[i][j]` y tomar `-cost`.

#### 6.6.5 Matching Máximo en Grafo General: Algoritmo Blossom de Edmonds (`Blossom`) [opcional]

- **Cuándo usar:** Hallar el máximo número de parejas disjuntas en un grafo no dirigido **general (no bipartito, con posibles ciclos impares)** para $V \le 500$.
- **Cuándo no usar / alternativa:** Si el grafo es bipartito, usar `Dinic` (§6.6.1) o `BipartiteMVC` (§6.6.3); en grafos no bipartitos el teorema de König NO se cumple.
- **Contrato:** Nodos 1-indexed (`1..n`). `solve()` retorna el número de parejas emparejadas y deja en `match[u]` la pareja del nodo `u` (`0` si quedó libre).
- **Complejidad:** $\mathcal{O}(V^3)$ tiempo y $\mathcal{O}(V + E)$ memoria.

-- blossom.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); llamar `bl.add_edge(u, v)` una vez por arista y `bl.solve()`:
```cpp
int n, m; cin >> n >> m;
Blossom bl(n);
forn(i, m) { int u, v; cin >> u >> v; bl.add_edge(u, v); }
int pairs = bl.solve(); // pareja de u en bl.match[u]
```
- **Trampas y adaptaciones:** (1) No agregar dos veces la arista en la entrada porque `add_edge(u, v)` ya inserta ambas direcciones.

------------------------------------------------------------------------

### 6.7 Árbol Generador Mínimo Dirigido (Directed MST / Arborescence) [opcional]

- **Cuándo usar:** Hallar la arborescencia dirigida de peso mínimo enraizada en `root` que alcance todos los demás nodos en un grafo **dirigido** con pesos.
- **Cuándo no usar / alternativa:** Si el grafo es no dirigido, usar `kruskal_mst` (§6.4.2).
- **Contrato:** Nodos **0-indexed** (`0..n-1`). Retorna `{costo_total, parent_edge}`, o `{-1, {}}` si algún nodo es inalcanzable desde `root`. Usa *Skew Heaps* con propagación perezosa y `RollbackDSU` (§4.1.2). Dependencias: `RollbackDSU` (§4.1.2).
- **Complejidad:** $\mathcal{O}(E \log V)$ tiempo y $\mathcal{O}(V + E)$ memoria.

-- directed_mst.cpp

- **Lectura de entrada y uso:** Nodos **0-based** (`0..n-1`); leer aristas en `vector<DirEdge> ed(m)` y llamar `directed_mst(n, root, ed)`:
```cpp
int n, m, root; cin >> n >> m >> root; --root;
vector<DirEdge> ed(m);
forn(i, m) {
  cin >> ed[i].u >> ed[i].v >> ed[i].w;
  --ed[i].u; --ed[i].v;
}
auto [cost, par] = directed_mst(n, root, ed);
```
- **Trampas y adaptaciones:** (1) A diferencia del resto del bloque de grafos, esta implementación de Edmonds es **0-indexed** (`0..n-1`); restar `1` a `u, v, root` si la entrada viene 1-indexed.

### 6.8 Componentes Biconexas y Block-Cut Tree [opcional]

- **Cuándo usar:** Comprimir un grafo no dirigido general en el árbol bipartito entre nodos originales (`1..N`) y bloques biconexos (`N+1..N+sz(blocks)`), por ejemplo para saber si todo camino simple entre `u` y `v` debe pasar por un punto de articulación `c`.
- **Cuándo no usar / alternativa:** Si solo necesitas las componentes 2-arista-conexas (sin aristas puente), basta colapsar componentes separadas por `BridgesAndCuts::bridges` (§6.5.2).
- **Contrato:** Nodos 1-indexed (`1..n`). Tras `build()`, `bct_adj` tiene `bct_nodes = n + sz(blocks)` vértices: una arista $(u, N + b)$ en `bct_adj` indica que el vértice original `u` pertenece al bloque `b`. Maneja correctamente nodos aislados y aristas paralelas.
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- block_cut_tree.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); agregar aristas y llamar `bct.build()`:
```cpp
int n, m; cin >> n >> m;
BlockCutTree bct(n);
forn(i, m) { int u, v; cin >> u >> v; bct.add_edge(u, v); }
bct.build(); // arbol en bct.bct_adj[1..bct.bct_nodes]
```
- **Trampas y adaptaciones:** (1) Un vértice `c` es obligatorio en todo camino simple entre `u` y `v` ($c \neq u, v$) si y solo si `c` es punto de articulación (`is_cut[c] == 1`) y `c` está en el camino del árbol `bct_adj` entre `u` y `v`.

### 6.9 Camino y Ciclo Euleriano (Hierholzer) [opcional]

- **Cuándo usar:** Encontrar un recorrido que utilice **exactamente una vez cada arista** de un grafo dirigido (con posibles multiaristas y auto-bucles), como en reconstrucción de cadenas por $k$-mers (grafos de De Bruijn).
- **Cuándo no usar / alternativa:** Si buscas visitar cada **vértice** exactamente una vez (Camino Hamiltoniano), el problema es NP-completo y requiere `tsp_subset_dp` (§8.2.2) para $N \le 20$.
- **Contrato:** Nodos 1-indexed (`1..n`). `find_tour()` verifica grados de entrada/salida, reinicia su estado interno y retorna los $M + 1$ vértices del recorrido en orden, o `{}` si el grafo no es euleriano (o tiene aristas en componentes desconectadas).
- **Complejidad:** $\mathcal{O}(V + E)$ tiempo y memoria.

-- eulerian_directed.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); agregar las $M$ aristas dirigidas y llamar `eu.find_tour()`:
```cpp
int n, m; cin >> n >> m;
EulerianDirected eu(n);
forn(i, m) { int u, v; cin >> u >> v; eu.add_edge(u, v); }
vi tour = eu.find_tour(); // vacio si no existe
```
- **Trampas y adaptaciones:** (1) Para grafo **no dirigido**, existe camino euleriano si a lo sumo 2 vértices tienen grado impar y todas las aristas pertenecen a una sola componente conexa; marcar `used_edge[id] = true` para ambas direcciones de la arista `id`.

### 6.10 Flujo Máximo en Redes Densas: Push-Relabel (Highest-Label + Gap) [opcional]

- **Cuándo usar:** Flujo máximo $s \to t$ en redes muy densas ($V \le 5000, E \le 10^6$) con capacidades grandes donde Dinic sufre demasiadas fases de aumento.
- **Cuándo no usar / alternativa:** En redes unitarias o matching bipartito, `Dinic` (§6.6.1) es más corto y corre en $\mathcal{O}(E\sqrt{V})$.
- **Contrato:** Nodos 1-indexed (`1..n`). Implementa preflujo con selección *Highest-Label* (`B[b_max]`) y heurística *Gap*.
- **Complejidad:** $\mathcal{O}(V^2 \sqrt{E})$ peor caso teórico (muy rápido en la práctica).

-- push_relabel.cpp

- **Lectura de entrada y uso:** Nodos **1-based** (`1..n`); agregar aristas `pr.add_edge(u, v, cap)` y llamar `pr.max_flow(s, t)`:
```cpp
int n, m, s, t; cin >> n >> m >> s >> t;
PushRelabel pr(n);
forn(i, m) {
  int u, v; ll c; cin >> u >> v >> c; pr.add_edge(u, v, c);
}
ll flow = pr.max_flow(s, t);
```
- **Trampas y adaptaciones:** (1) Para capacidades no dirigidas, agregar dos llamadas `add_edge(u, v, cap)` y `add_edge(v, u, cap)`.

------------------------------------------------------------------------

### 6.11 Grafos Funcionales (Successors): Ciclos, Colas y Binary Lifting [opcional]

- **Cuándo usar:** Grafos dirigidos donde **cada nodo tiene grado de salida exactamente 1** ($u \to \text{to}[u]$): hallar el nodo tras $K \le 10^{18}$ pasos (`jump`), el mínimo número de pasos de `u` a `v` (`min_steps`), o la longitud e índice del ciclo de cada componente.
- **Cuándo no usar / alternativa:** Si los nodos tienen múltiples aristas de salida, usar `TarjanSCC` (§6.5.3) o `LCA` en árboles (§6.4.3).
- **Contrato:** Nodos 1-indexed (`1..n`); `to[u] \in [1, n]`. `LOG = 62` soporta $K \le 2^{61} > 2 \cdot 10^{18}$. `min_steps(u, v)` retorna `-1` si `v` es inalcanzable desde `u`.
- **Complejidad:** Construcción $\mathcal{O}(N \log K_{\max})$ tiempo y memoria; `jump` y `min_steps` en $\mathcal{O}(\log K_{\max})$.

-- functional_graph.cpp

- **Lectura de entrada y uso:** Leer `nxt[1..n]` (1-based) y construir `FunctionalGraph fg(n, nxt)`:
```cpp
int n, q; cin >> n >> q;
vi nxt(n + 1); for1(i, n) cin >> nxt[i];
FunctionalGraph fg(n, nxt);
// fg.jump(u, k), fg.min_steps(u, v)
```
- **Trampas y adaptaciones:** (1) Un nodo en la cola (`dist_cyc > 0`) nunca puede ser alcanzado desde un nodo que ya entró al ciclo (`dist_cyc == 0`).
