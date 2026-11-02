------------------------------------------------------------------------

## Bloque 4: Estructuras de Datos Avanzadas (Consultas en Rango y Árboles)

### 4.1 Disjoint Set Union (DSU Clásico) y DSU con Rollback + Conectividad Dinámica Offline

#### 4.1.1 Disjoint Set Union Clásico (`DSU`)

- **Cuándo usar:** Mantener componentes conexas al agregar aristas, consultar si dos nodos pertenecen al mismo componente (`same(u, v)`), conocer el tamaño del componente (`get_size(u)`) o el total de componentes (`num_comp`).
- **Cuándo no usar / alternativa:** Si el problema también **elimina** aristas, el DSU clásico con compresión de caminos no puede deshacer uniones; usar `RollbackDSU` + `DynCon` (§4.1.2) o `LinkCut` (§4.9).
- **Contrato:** Nodos `1..N` (también admite `0` porque reserva `n + 1`). `unite(u, v)` retorna `true` si unió dos componentes distintos y `false` si ya estaban unidos.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `find`, `unite`, `same`, `get_size` en $\mathcal{O}(\alpha(N))$ amortizado; $\mathcal{O}(N)$ memoria.

-- dsu.cpp

- **Trampas y adaptaciones:** (1) Acceder siempre a las propiedades del componente a través de su representante `dsu.find(u)` (nunca `sz[u]` directo); (2) `num_comp` se inicializa en `n` (si usas nodos `1..n`, ignora el índice `0`).

#### 4.1.2 DSU con Rollback y Conectividad Dinámica Offline (`RollbackDSU`, `DynCon`) [opcional]

- **Cuándo usar:** Responder el número de componentes conexas (o propiedades de DSU) tras una secuencia de **inserciones y eliminaciones de aristas** conocidas de antemano (offline) dividiendo en el árbol de intervalos de tiempo.
- **Cuándo no usar / alternativa:** Si las operaciones son estrictamente **online** (forzadas por codificación de la respuesta anterior), usar **Link-Cut Tree** (§4.9) cuando el grafo se mantiene acíclico.
- **Contrato:** Nodos `1..N`. En `DynCon(n)`, `add_edge(u, v)` agrega una arista activa y `rem_edge(u, v)` elimina una arista **actualmente activa**; `add_query()` registra una consulta y `solve()` retorna el vector de respuestas en orden cronológico.
- **Complejidad:** `RollbackDSU`: `unite`/`find` en $\mathcal{O}(\log N)$ (sin compresión de caminos), `rollback` en $\mathcal{O}(1)$ por unión revertida. `DynCon::solve()`: $\mathcal{O}(Q \log Q \log N)$ tiempo y $\mathcal{O}(N + Q)$ memoria.

-- rollback_dsu.cpp

- **Trampas y adaptaciones:** (1) No agregar compresión de caminos a `RollbackDSU::find`, porque destruiría el historial de `rollback`; (2) si pueden insertarse aristas paralelas idénticas, guardar un `vector<int>` de tiempos de inicio en `active[{u, v}]` en vez de un solo `int`.

------------------------------------------------------------------------

### 4.2 Sparse Table (RMQ e Idempotentes en $\mathcal{O}(1)$)

- **Cuándo usar:** Responder muchas consultas ($Q \le 10^6$) de **Mínimo, Máximo, GCD, AND u OR** en un rango $[L, R]$ sobre un arreglo **100% estático** en $\mathcal{O}(1)$ puro.
- **Cuándo no usar / alternativa:** **No funciona** en $\mathcal{O}(1)$ para operaciones **no idempotentes** como la **suma** o **XOR** porque los dos bloques de longitud $2^j$ se solapan (usar `PrefixSum` §3.4.1). Si hay modificaciones en el arreglo, usar `IterSegTree` (§4.4.1).
- **Contrato:** Arreglo y consultas **0-indexed** ($0 \le L \le R < N$, precondición: `N >= 1`). Cambiar `op(a, b)` a `max(a, b)` o `std::gcd(a, b)` según el problema.
- **Complejidad:** Construcción $\mathcal{O}(N\log N)$ tiempo y memoria; consulta `query(L, R)` en $\mathcal{O}(1)$.

-- sparse_table.cpp

- **Trampas y adaptaciones:** (1) Verificar siempre `L <= R` antes de llamar a `query(L, R)` (de lo contrario `R - L + 1 <= 0` indexa fuera de `lg`).

------------------------------------------------------------------------

### 4.3 Fenwick Tree / BIT 1D (con Binary Lifting `lower_bound`) y Fenwick Tree 2D

#### 4.3.1 Fenwick Tree / BIT 1D con Binary Lifting (`Fenwick`)

- **Cuándo usar:** Actualizaciones puntuales (`add(idx, delta)`) y sumas de rango (`query(l, r)`) dinámicas con constante mínima, o hallar en $\mathcal{O}(\log N)$ el primer índice cuyo prefijo suma $\ge K$ (`lower_bound(k)`).
- **Cuándo no usar / alternativa:** No sirve para mínimo/máximo en rango arbitrario $[L, R]$ con actualizaciones arbitrarias (usar `IterSegTree` §4.4.1). `lower_bound(k)` exige que todos los valores almacenados sean **no negativos**.
- **Contrato:** Indexación estrictamente **1-based** (`1 <= idx <= n`). `lower_bound(k)` retorna el menor índice $p \in [1, n]$ con $\text{query}(p) \ge k$, o `n + 1` si la suma total es $< k$.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `add`, `query`, `lower_bound` en $\mathcal{O}(\log N)$ tiempo; $\mathcal{O}(N)$ memoria.

-- fenwick_1d.cpp

- **Trampas y adaptaciones:** (1) **Jamás** llamar `ft.add(0, delta)`: como `0 & -0 == 0`, produce un **bucle infinito**; si tus datos son 0-based, suma `+ 1` al índice.

#### 4.3.2 Fenwick Tree 2D (`Fenwick2D`) [opcional]

- **Cuándo usar:** Actualizaciones puntuales `add(r, c, delta)` y consultas de suma en submatrices `[r1..r2] x [c1..c2]` dinámicas cuando $N, M \le 2500$.
- **Cuándo no usar / alternativa:** Si $N, M = 10^5$ la matriz $N \times M$ da MLE; usar compresión 2D offline o CDQ Divide & Conquer.
- **Contrato:** Coordenadas **1-based** (`1 <= r <= n`, `1 <= c <= m`).
- **Complejidad:** $\mathcal{O}(\log N \log M)$ por `add` y `query`; $\mathcal{O}(N \cdot M)$ memoria.

-- fenwick_2d.cpp

- **Trampas y adaptaciones:** (1) Verificar que $r, c \ge 1$ antes de llamar a `add(r, c, delta)` para evitar bucle infinito en 0.

------------------------------------------------------------------------

### 4.4 Segment Tree Clásico (Recursivo) y Segment Tree Iterativo

#### 4.4.1 Segment Tree Iterativo (`IterSegTree`)

- **Cuándo usar:** Actualizaciones puntuales (`a[p] = val`) y consultas en rango `[l, r]` de cualquier operación asociativa (suma, min, max, gcd) con mitad de memoria (`2*N`) y $3\times$ menor constante que el recursivo.
- **Cuándo no usar / alternativa:** Si la operación no es conmutativa (p. ej. multiplicación de matrices o concatenación de hashes), en el bucle `query` cuidar el orden `res_left = op(res_left, tree[l++])` y `res_right = op(tree[--r], res_right)` (o usar `SegTree` recursivo §4.4.2).
- **Contrato:** Indexación **0-based** (`0 <= p < n`, consulta **inclusiva** `query(l, r)` en `[l, r]`). Ajustar `NEUT` (`0` para suma/gcd, `INF` para min, `-INF` para max) y `op(a, b)`.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `update` y `query` en $\mathcal{O}(\log N)$ tiempo; $\mathcal{O}(N)$ memoria.

-- segtree_iter.cpp

- **Trampas y adaptaciones:** (1) Aunque internamente el bucle convierte a semiabierto `[l, r + 1)`, la interfaz pública `query(l, r)` recibe un rango **cerrado** `[l, r]` 0-based.

#### 4.4.2 Segment Tree Recursivo Clásico (`SegTree`)

- **Cuándo usar:** Consultas y actualizaciones puntuales cuando la operación es **no conmutativa** (matrices, subarreglo de suma máxima en rango) o cuando necesitas hacer búsqueda binaria dentro del árbol (*Segment Tree walk*).
- **Cuándo no usar / alternativa:** Si también hay actualizaciones en rango `[l, r]`, usar `LazySegTree` (§4.5.1).
- **Contrato:** Indexación **0-based** (`0 <= l <= r < n`).Reserva `4 * n` nodos.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `update` y `query` en $\mathcal{O}(\log N)$; memoria $\mathcal{O}(4N)$.

-- segtree_rec.cpp

- **Trampas y adaptaciones:** (1) Nunca dimensionar el vector del árbol recursivo como `2 * n` (cuando `n` no es potencia de 2 puede acceder hasta casi `4 * n`); usar siempre `4 * n`.

------------------------------------------------------------------------

### 4.5 Segment Tree con Lazy Propagation, Segment Tree 2D y Persistent Segment Tree

#### 4.5.1 Segment Tree con Lazy Propagation (`LazySegTree`)

- **Cuándo usar:** Actualizaciones en rango (`add_range(l, r, val)`) y consultas en rango (`query_sum(l, r)`, `query_min(l, r)`) intercaladas dinámicamente.
- **Cuándo no usar / alternativa:** Si todas las actualizaciones de rango ocurren antes de cualquier consulta, basta un `DiffArray1D` + `PrefixSum` (§3.5.1) en $\mathcal{O}(N)$.
- **Contrato:** Indexación **0-based** (`0 <= l <= r < n`). `apply(node, l, r, v)` suma `v * (r - l + 1)` a `sum` y `v` a `mn` y `lazy`.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `add_range`, `query_sum`, `query_min` en $\mathcal{O}(\log N)$; memoria $\mathcal{O}(4N)$.

-- lazy_segtree.cpp

- **Trampas y adaptaciones:** (1) Para soportar asignación en rango (`a[l..r] = v`) además de suma, mantener un booleano `has_set` y al aplicar un `set` limpiar el `lazy` de suma previo.

#### 4.5.2 Segment Tree 2D Iterativo (`SegTree2D`) [opcional]

- **Cuándo usar:** Actualizaciones puntuales `update(x, y, val)` (reemplazo) y consultas rectangulares `[x0..x1] x [y0..y1]` de operaciones como suma, `min`, `max` o `gcd` donde Fenwick 2D no aplica por no ser invertible.
- **Cuándo no usar / alternativa:** Si la operación es suma (`a[x][y] += delta`), `Fenwick2D` (§4.3.2) usa $4\times$ menos memoria y código.
- **Contrato:** Coordenadas **0-based** (`0 <= x0 <= x1 < n`, `0 <= y0 <= y1 < m`).
- **Complejidad:** Construcción $\mathcal{O}(N \cdot M)$; `update` y `query` en $\mathcal{O}(\log N \log M)$; memoria $\mathcal{O}(4 N M)$.

-- segtree_2d.cpp

- **Trampas y adaptaciones:** (1) `update(x, y, val)` **reemplaza** el valor de la celda `(x, y)` por `val`; si quieres sumar `delta`, hacer `t[x + n][y + m] += val`.

#### 4.5.3 Persistent Segment Tree de Sumas en Versiones Históricas (`PersistentST`) [opcional]

- **Cuándo usar:** Clonar el arreglo en el tiempo al hacer actualizaciones puntuales (`create_version`) y consultar la suma en `[ql, qr]` sobre cualquier versión histórica en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** Si necesitas hallar el **$k$-ésimo menor elemento en un subarreglo $[L, R]$**, esta estructura de sumas por posición no basta: usar `PersistentFreqST` (§4.5.4).
- **Contrato:** Índices **0-based** (`0 <= pos < n`). `roots[0]` es la versión inicial; `create_version(ver, pos, val)` crea una nueva raíz en `roots.back()` y retorna su ID de versión.
- **Complejidad:** Construcción $\mathcal{O}(N)$; cada actualización crea $\mathcal{O}(\log N)$ nodos nuevos en tiempo y memoria.

-- persistent_segtree.cpp

- **Trampas y adaptaciones:** (1) Con $Q$ actualizaciones se crean $\approx Q (\lceil \log_2 N \rceil + 2)$ nodos; `reserve` evita realocaciones.

#### 4.5.4 Persistent Segment Tree de Frecuencias: $k$-ésimo en Subarreglo (`PersistentFreqST`) [opcional]

- **Cuándo usar:** Responder online cuál es el **$k$-ésimo menor valor** en cualquier subarreglo estático `a[l..r]` (0-indexed, $k \ge 1$) en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** Si el arreglo también sufre modificaciones puntuales entre consultas de $k$-ésimo, un árbol persistente de prefijos estático no basta (requiere Fenwick de Segment Trees dinámicos en $\mathcal{O}(\log^2 N)$).
- **Contrato:** Arreglo `a` **0-indexed** (`0 <= l <= r < n`) y orden `1 <= k <= r - l + 1`. Comprime coordenadas internamente y devuelve el valor real `vals[idx]`.
- **Complejidad:** Construcción $\mathcal{O}(N \log N)$ tiempo y memoria; `query_kth(l, r, k)` en $\mathcal{O}(\log N)$.

-- persistent_freq_segtree.cpp

- **Trampas y adaptaciones:** (1) Verificar siempre `1 <= k && k <= r - l + 1` antes de llamar a `query_kth(l, r, k)`.

------------------------------------------------------------------------

### 4.6 SQRT Decomposition y Algoritmo de Mo (con Optimización de Bloques) [opcional]

#### 4.6.1 SQRT Decomposition en Bloques (`SqrtDecomp`) [opcional]

- **Cuándo usar:** Actualizar elementos y responder consultas en rango cuando la estructura por bloques de tamaño $B \approx \sqrt{N}$ es mucho más simple de adaptar que un Segment Tree (p. ej., mantener bloques ordenados para contar $< X$).
- **Cuándo no usar / alternativa:** Para suma/mínimo estándar, `Fenwick` (§4.3.1) o `IterSegTree` (§4.4.1) son $\mathcal{O}(\log N)$ y más veloces.
- **Contrato:** Indexación **0-based** (`0 <= l <= r < n`). `B = sqrt(n) + 1`.
- **Complejidad:** Construcción $\mathcal{O}(N)$; `update` en $\mathcal{O}(1)$; `query` en $\mathcal{O}(\sqrt{N})$.

-- sqrt_decomp.cpp

- **Trampas y adaptaciones:** (1) En lugar de iterar `i` y comprobar `i % B == 0`, manejar por separado `b_l == b_r` y luego los bordes más los bloques completos `b_l + 1 .. b_r - 1` es $3\times$ más rápido.

#### 4.6.2 Algoritmo de Mo Offline (`MoQuery`, `solve_mo`) [opcional]

- **Cuándo usar:** Responder $Q$ consultas **offline** sobre rangos `[l, r]` cuando no hay forma sencilla de fusionar dos mitades en un Segment Tree, pero agregar o quitar 1 elemento en los extremos del rango toma $\mathcal{O}(1)$ (frecuencias, elementos distintos, pares iguales).
- **Cuándo no usar / alternativa:** Si las consultas son online o si agregar/quitar un elemento cuesta $\mathcal{O}(\log N)$ con $N, Q = 2 \cdot 10^5$ ($\mathcal{O}(N\sqrt{Q}\log N)$ suele dar TLE).
- **Contrato:** Arreglo `a` y consultas `queries` son **0-indexed** (`0 <= l <= r < n`). Los valores de `a[i]` deben estar en `[0, MAX_VAL - 1]` (comprimir coordenadas con `CoordComp` §2.1 si superan $10^6$).
- **Complejidad:** $\mathcal{O}((N + Q)\sqrt{N})$ tiempo y $\mathcal{O}(N + Q + V_{\max})$ memoria.

-- mo_algorithm.cpp

- **Trampas y adaptaciones:** (1) **Orden de los 4 `while`**: siempre expandir (`--cur_l`, `++cur_r`) **antes** de contraer (`cur_l++`, `cur_r--`); de lo contrario `cur_l` puede cruzar a `cur_r + 2` y causar conteos negativos.

------------------------------------------------------------------------

### 4.7 Treap (BST Aleatorizado) e Implicit Treap (Reversión y Suma en Rango) [opcional]

- **Cuándo usar:** Mantener un arreglo dinámico que soporte **revertir** un subarreglo `[l, r]`, **sumar** una constante a `[l, r]`, **cortar y mover** bloques a otra posición y consultar la **suma** de `[l, r]` en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** Si no hay reversiones ni cortes/inserciones de posición (solo sumas y consultas en rango sobre índices fijos), usar `LazySegTree` (§4.5.1), que tiene constante mucho menor.
- **Contrato:** Posiciones **0-based** (`0 <= l <= r < cnt(root)`). Llamar siempre a `push(t)` al inicio de `split` y `merge`, y a `pull(t)` antes de retornar. Dependencias: `rng` (§1.2).
- **Complejidad:** $\mathcal{O}(\log N)$ tiempo esperado por operación; $\mathcal{O}(N)$ memoria.

-- implicit_treap.cpp

- **Trampas y adaptaciones:** (1) En `split(t, l, r, k)`, el parámetro `k` es la **cantidad de elementos** que quedan en el árbol izquierdo `l` (es decir, índices `0..k-1`), no un valor de llave.

------------------------------------------------------------------------

### 4.8 Consultas en Árboles: Heavy-Light Decomposition (HLD) y DSU on Tree (Sack) [opcional]

#### 4.8.1 Heavy-Light Decomposition (`HLD`) [opcional]

- **Cuándo usar:** Resolver consultas en **caminos** (`query_path(u, v)`) y en **subárboles** (`query_subtree(u)`) con actualizaciones dinámicas de nodos cuando la operación no es invertible (como `max`, `min`, `gcd`) o cuando se combinan caminos y subárboles.
- **Cuándo no usar / alternativa:** Si el árbol es estático y la operación es suma/XOR, basta `LCA` (§6.4.3) con sumas a la raíz; si solo hay updates en nodos y sumas en subárboles, basta Euler Tour + Fenwick (§0.3).
- **Contrato:** Nodos **1-based** (`1..n`), valores iniciales `vals[1..n]`. Esta implementación incluye `update_node(u, val)` puntual; para actualizar todo un camino `u..v`, reemplazar el `IterSegTree` interno por `LazySegTree` (§4.5.1) e invocar `st.add_range(pos[head[u]], pos[u], val)` en el bucle de subida. Dependencias: `IterSegTree` (§4.4.1).
- **Complejidad:** Construcción $\mathcal{O}(N \log N)$ (por las $N$ llamadas iniciales a `st.update`); `update_node` y `query_subtree` en $\mathcal{O}(\log N)$; `query_path` en $\mathcal{O}(\log^2 N)$.

-- hld.cpp

- **Trampas y adaptaciones:** (1) Si los valores están en las **aristas** en lugar de los nodos, asignar el peso de cada arista al nodo hijo (el más profundo) y en la última etapa de `query_path` consultar `st.query(pos[u] + 1, pos[v])` para excluir el LCA `u`.

#### 4.8.2 DSU on Tree / Small-to-Large en Subárboles (`pre_sack`, `dfs_sack`) [opcional]

- **Cuándo usar:** Responder consultas **offline** sobre todos los subárboles (p. ej., color más frecuente en el subárbol de $u$ o cantidad de valores distintos) en $\mathcal{O}(N \log N)$ sin estructuras pesadas.
- **Cuándo no usar / alternativa:** Si hay actualizaciones de colores intercaladas con consultas online, Sack no aplica.
- **Contrato:** Nodos **1-based** (`1..n`). Llenar `adj_s[1..n]` y `col_s[1..n]`, llamar `pre_sack(1, 0)` y luego `dfs_sack(1, 0, false)`. Respuestas en `ans_sack[1..n]`.
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo total y $\mathcal{O}(N)$ memoria.

-- dsu_on_tree.cpp

- **Trampas y adaptaciones:** (1) Entre casos de prueba múltiples, limpiar `adj_s[1..n]`, `big_c[1..n] = 0`, `timer_s = 0` y asegurarse de que la raíz se llame con `keep = false` si quieres dejar `cnt_col` limpio en 0 al terminar.

------------------------------------------------------------------------

### 4.9 Link-Cut Tree (LCT — Bosques Dinámicos Online) [opcional]

- **Cuándo usar:** Mantener un bosque de árboles **online** bajo inserción de aristas (`link(u, v)`), borrado de aristas (`cut(u, v)`), prueba de conectividad (`connected(u, v)`), actualizaciones puntuales y sumas en el camino entre $u$ y $v$ (`query_path(u, v)`).
- **Cuándo no usar / alternativa:** Si el árbol nunca cambia su topología de aristas, usar `HLD` (§4.8.1); si solo se pide conectividad y todas las operaciones se conocen de antemano, usar `DynCon` (§4.1.2). Link-Cut Tree solo mantiene **bosques** (sin ciclos).
- **Contrato:** Nodos **1-based** (`1..n`). `link(u, v)` requiere `!connected(u, v)`; `cut(u, v)` requiere que la arista `(u, v)` exista actualmente.
- **Complejidad:** $\mathcal{O}(\log N)$ amortizado por operación; $\mathcal{O}(N)$ memoria.

-- link_cut_tree.cpp

- **Trampas y adaptaciones:** (1) Verificar siempre `!lct.connected(u, v)` antes de `lct.link(u, v)` porque crear un ciclo rompe la invariante de los Splay Trees auxiliares.
