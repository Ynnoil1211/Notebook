------------------------------------------------------------------------

## Bloque 3: Búsquedas, Two Pointers, Prefix Sum y Difference Array

### 3.1 Binary Search (Discreta Max/Min y Continua `double`)

#### 3.1.1 Binary Search Discreta sobre la Respuesta (`bs_max` y `bs_min`)

- **Cuándo usar:** Problemas de «maximizar el mínimo» (`bs_max`, predicado `V, V, ..., F, F`) o «minimizar el máximo» (`bs_min`, predicado `F, F, ..., V, V`) sobre un dominio entero `[low, high]`.
- **Cuándo no usar / alternativa:** Si el predicado `check(mid)` **no es monótono** en `[low, high]`, la búsqueda binaria descartará la mitad equivocada.
- **Contrato:** `bs_max(low, high, check)` retorna el mayor entero en `[low, high]` con `check(mid) == true`, o `low - 1` si ninguno cumple. `bs_min(low, high, check)` retorna el menor entero con `check(mid) == true`, o `high + 1` si ninguno cumple.
- **Complejidad:** $\mathcal{O}(\log(\text{high} - \text{low} + 1) \cdot T_{\text{check}})$ tiempo, $\mathcal{O}(1)$ memoria.

-- binary_search_discrete.cpp

- **Lectura de entrada y uso:** Leer el arreglo, definir `check(mid)` con lambda y llamar `bs_max` (último `true`) o `bs_min` (primer `true`):
```cpp
int n; ll k; cin >> n >> k;
vll a(n); forn(i, n) cin >> a[i]; sort(all(a));
auto check = [&](ll d) { // ej: colocar k puntos a dist >= d
    int cnt = 1; ll last = a[0];
    forn(i, n) if (a[i] - last >= d) cnt++, last = a[i];
    return cnt >= k;
};
cout << bs_max(0, a.back() - a[0], check) << "\n";
```
- **Trampas y adaptaciones:** (1) Nunca evaluar `L = 0` si `check(L)` divide entre `L` (`low >= 1`); (2) elegir `high` suficientemente grande pero sin causar overflow en `mid * algo` dentro de `check(mid)`.

#### 3.1.2 Binary Search Continua en Punto Flotante (`bs_double`)

- **Cuándo usar:** Hallar un umbral real $x \in [l, r]$ cuando `check(x)` es monótono (tiempo mínimo de encuentro, promedio máximo $0/1$ fraccionario).
- **Cuándo no usar / alternativa:** No usar `while (r - l > EPS)` como condición de parada: si $l, r \approx 10^9$ y `EPS = 1e-9`, la distancia entre `double` consecutivos supera `EPS` y produce un **bucle infinito (TLE)**.
- **Contrato:** `bs_double(l, r, check, it = 90)` ejecuta `it` iteraciones fijas reduciendo el intervalo en $2^{-90}$. Asume `check(mid) == true` en la mitad izquierda (`[True, False]`); si es `[False, True]`, intercambiar `l = mid` y `r = mid`.
- **Complejidad:** $\mathcal{O}(\text{it} \cdot T_{\text{check}})$ con `it = 90`.

-- binary_search_real.cpp

- **Trampas y adaptaciones:** (1) Recordar que `double` tiene 53 bits de mantisa ($\approx 15$ dígitos significativos): la precisión real es relativa a la escala ($|x| \cdot 2^{-53}$); usar `long double` (`ld`) si el rango supera $10^9$.

------------------------------------------------------------------------

### 3.2 Ternary Search (Discreta y Continua para Funciones Unimodales)

#### 3.2.1 Ternary Search Continua (`ternary_continuous`)

- **Cuándo usar:** Minimizar (o maximizar) una función continua **estrictamente unimodal** $f(x)$ en $[l, r]$ (p. ej., suma de distancias convexas).
- **Cuándo no usar / alternativa:** Si $f(x)$ tiene tramos planos constantes (mesetas) o múltiples mínimos locales, la búsqueda ternaria puede converger a un punto subóptimo.
- **Contrato:** `ternary_continuous(l, r, f, it = 100)` retorna el **valor mínimo** $f(x^*)$ en $[l, r]$. Para maximizar, cambiar `<` por `>` o minimizar `-f(x)`.
- **Complejidad:** $\mathcal{O}(\text{it} \cdot T_f)$ con `it = 100` (reduce el intervalo por $(2/3)^{100} \approx 2.4 \cdot 10^{-18}$).

-- ternary_search_real.cpp

- **Trampas y adaptaciones:** (1) Toda suma de funciones convexas (como distancias euclidianas a puntos o rectas) es convexa y por tanto unimodal; (2) si necesitas la abscisa $x^*$ en vez de $f(x^*)$, retornar `l` al final.

#### 3.2.2 Ternary Search Discreta en Enteros (`ternary_discrete`)

- **Cuándo usar:** Minimizar una función entera estrictamente unimodal $f(x)$ para $x \in [\text{low}, \text{high}] \cap \mathbb{Z}$.
- **Cuándo no usar / alternativa:** Si existen valores consecutivos con $f(x) = f(x+1)$ fuera del óptimo global, la ternaria falla; si $f(x+1) - f(x)$ es monótona (función convexa), también puedes hacer búsqueda binaria sobre la derivada discreta $\Delta f(x) = f(x+1) - f(x) \ge 0$.
- **Contrato:** `ternary_discrete(low, high, f)` retorna $\min_{x \in [\text{low}, \text{high}]} f(x)$, evaluando exhaustivamente cuando el rango se reduce a $\le 3$ elementos.
- **Complejidad:** $\mathcal{O}(\log_{3/2}(\text{high} - \text{low} + 1) \cdot T_f)$.

-- ternary_search_discrete.cpp

- **Trampas y adaptaciones:** (1) Nunca reducir directamente a un solo punto sin el bucle `while (high - low >= 3)` + barrido final de 3 enteros, o ciclará por división entera.

------------------------------------------------------------------------

### 3.3 Two Pointers, Monotonic Stack (Next Greater/Smaller) y Monotonic Deque (Sliding Window Minimum)

#### 3.3.1 Two Pointers / Ventana Deslizante (`max_subarray_sum_bound`)

- **Cuándo usar:** Hallar el subarreglo más largo (o más corto, o contar cuántos subarreglos) que cumple una condición **monótona** al expandir `r` y contraer `l` (p. ej., suma $\le S$ cuando $a[i] \ge 0$).
- **Cuándo no usar / alternativa:** **Jamás** usar esta ventana deslizante para suma de subarreglos si existen números **negativos** ($a[i] < 0$), porque quitar `a[l]` aumentaría la suma y rompería la monotonía (ver §0.3 y §3.4.2).
- **Contrato:** Precondición obligatoria: $a[i] \ge 0$ para todo $i$ y `max_sum >= 0`. Retorna la mayor longitud de un subarreglo contiguo con suma $\le \text{max\_sum}$ (`0` si ninguno cumple).
- **Complejidad:** $\mathcal{O}(N)$ tiempo (`l` y `r` avanzan a lo sumo $N$ veces) y $\mathcal{O}(1)$ memoria.

-- two_pointers.cpp

- **Trampas y adaptaciones:** (1) Para contar **cuántos** subarreglos tienen suma $\le \text{max\_sum}$ (con $a[i] \ge 0$), en cada `r` sumar `(r - l + 1)` a un acumulador `ll` después del `while`.

#### 3.3.2 Monotonic Stack: Primer Mayor/Menor a la Derecha o Izquierda (`next_greater_right`)

- **Cuándo usar:** Encontrar para cada índice $i$ el primer índice $j > i$ (o $j < i$) con $a[j] > a[i]$ (o $<, \ge, \le$). Base de mayor rectángulo en histograma y técnica de contribuciones (§7.11).
- **Cuándo no usar / alternativa:** Si los elementos cambian dinámicamente y se pide el primer índice $\ge X$ en un rango $[L, R]$, usar **Segment Tree walk** en $\mathcal{O}(\log N)$.
- **Contrato:** Índices `0..N-1`. `next_greater_right(a)` retorna `nxt[0..N-1]` donde `nxt[i]` es el menor $j > i$ con `a[j] > a[i]`, o `-1` si no existe.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- monotonic_stack.cpp

- **Trampas y adaptaciones:** (1) Para hallar el primer elemento **menor** a la derecha, cambiar `a[st.top()] < a[i]` por `a[st.top()] > a[i]`; (2) para el primer mayor a la **izquierda**, recorrer de derecha a izquierda o consultar el tope de la pila antes de hacer `push(i)`.

#### 3.3.3 Monotonic Deque: Mínimo/Máximo en Ventana Deslizante (`sliding_window_min`)

- **Cuándo usar:** Calcular el mínimo (o máximo) de todas las subventanas contiguas de longitud fija $K$ en $\mathcal{O}(N)$, o acelerar DP con transiciones en rango $[i - K, i - 1]$.
- **Cuándo no usar / alternativa:** Si la longitud de la ventana no es monótona (los extremos `L` y `R` retroceden arbitrariamente), usar **Sparse Table** (§4.2) en $\mathcal{O}(1)$ por consulta.
- **Contrato:** Precondición: $1 \le K \le N$. Retorna un vector de tamaño $N - K + 1$ donde la posición $i$ contiene $\min(a[i..i+K-1])$.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- monotonic_deque.cpp

- **Trampas y adaptaciones:** (1) La `deque` guarda **índices** (no valores) para poder detectar cuándo `dq.front() <= i - k` sale de la ventana; (2) para ventana de **máximo**, cambiar `a[dq.back()] >= a[i]` por `<=`.

------------------------------------------------------------------------

### 3.4 Prefix Sum 1D (Arreglos y Cadenas) y Prefix Sum 2D (Submatrices en $\mathcal{O}(1)$)

#### 3.4.1 Prefix Sum 1D (`PrefixSum`)

- **Cuándo usar:** Responder $Q$ consultas de suma en un rango $[l, r]$ sobre un arreglo **estático** en $\mathcal{O}(1)$ por consulta.
- **Cuándo no usar / alternativa:** Si hay actualizaciones del arreglo intercaladas con las consultas de suma, usar **Fenwick Tree** (§4.3.1) en $\mathcal{O}(\log N)$.
- **Contrato:** El constructor recibe `a` (0-indexed), pero las consultas `query(l, r)` son **1-indexed inclusivas** ($1 \le l \le r \le N$). Si $l > r$, retorna `0`.
- **Complejidad:** Construcción $\mathcal{O}(N)$, consulta $\mathcal{O}(1)$, memoria $\mathcal{O}(N)$.

-- prefix_sum_1d.cpp

- **Lectura de entrada y uso:** Leer `a[0..n-1]` (0-based) y responder $Q$ consultas `l r` **1-based inclusivas**:
```cpp
int n, q; cin >> n >> q;
vll a(n); forn(i, n) cin >> a[i];
PrefixSum ps(a);
while (q--) {
    int l, r; cin >> l >> r; cout << ps.query(l, r) << "\n";
}
```
- **Trampas y adaptaciones:** (1) Los prefijos deben guardarse siempre en `ll` (`vll pref`) aunque el arreglo original quepa en `int`; (2) si las consultas del enunciado son 0-based `[l, r]`, llamar `ps.query(l + 1, r + 1)`.

#### 3.4.2 Prefix Sum 2D en Submatrices (`PrefixSum2D`)

- **Cuándo usar:** Consultar la suma de cualquier submatriz rectangular $[r_1..r_2] \times [c_1..c_2]$ en una grilla estática en $\mathcal{O}(1)$.
- **Cuándo no usar / alternativa:** Si la matriz recibe actualizaciones puntuales entre consultas, usar **Fenwick Tree 2D** (§4.3.2).
- **Contrato:** El constructor recibe `mat` 0-indexed de dimensión $N \times M$; `query(r1, c1, r2, c2)` usa coordenadas **1-indexed inclusivas** ($1 \le r_1 \le r_2 \le N$, $1 \le c_1 \le c_2 \le M$).
- **Complejidad:** Construcción $\mathcal{O}(N \cdot M)$, consulta $\mathcal{O}(1)$, memoria $\mathcal{O}(N \cdot M)$.

-- prefix_sum_2d.cpp

- **Lectura de entrada y uso:** Leer grilla `mat` $N 	imes M$ (0-based) y consultar rectángulos `r1 c1 r2 c2` **1-based**:
```cpp
int n, m, q; cin >> n >> m >> q;
vector<vll> mat(n, vll(m));
forn(i, n) forn(j, m) cin >> mat[i][j];
PrefixSum2D ps(mat);
while (q--) {
    int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
    cout << ps.query(r1, c1, r2, c2) << "\n";
}
```
- **Trampas y adaptaciones:** (1) Al aplicar inclusión-exclusión 2D, recordar sumar de vuelta `+ p[r1 - 1][c1 - 1]` que fue restado dos veces.

#### 3.4.3 Patrones de Prefijos (Suma $K$, Divisibles por $M$, Prefix XOR) y Kadane

- **Cuándo usar:** Contar subarreglos con suma exacta $K$ (incluso con **negativos**), suma divisible por $M$, XOR igual a $K$, o hallar la máxima suma de un subarreglo contiguo no vacío (`kadane_max_subarray`).
- **Cuándo no usar / alternativa:** Para subarreglo de suma $\le K$ con negativos, `count_subarrays_sum_k` solo cuenta igualdades exactas; usar árbol de orden (`ordered_multiset`, §2.6) o Fenwick con compresión sobre los prefijos.
- **Contrato:** `kadane_max_subarray(a)` requiere `!a.empty()` y retorna el mayor elemento si todos son negativos. `count_subarrays_div_m(a, m)` requiere $m > 0$ y normaliza negativos con `((pref + x) % m + m) % m`. Dependencias: `safe_umap` (§2.5).
- **Complejidad:** $\mathcal{O}(N)$ tiempo y $\mathcal{O}(N)$ memoria ($\mathcal{O}(1)$ memoria en Kadane).

-- prefix_patterns.cpp

- **Trampas y adaptaciones:** (1) Inicializar siempre `freq[0] = 1` antes de recorrer el arreglo para contar los subarreglos que empiezan en el índice 0; (2) no inicializar `best = 0` en Kadane si el subarreglo no puede ser vacío y todos los números son negativos.

------------------------------------------------------------------------

### 3.5 Difference Array 1D, Doble Difference Array (Greg and Array) y Difference Array 2D

#### 3.5.1 Difference Array 1D y Doble Difference Array (`DiffArray1D`, `apply_double_diff`)

- **Cuándo usar:** Aplicar $Q$ sumas en rango (`sumar val a [l, r]`) en $\mathcal{O}(1)$ cada una cuando **todas las consultas ocurren al final**, o aplicar rangos de operaciones sobre un arreglo (`apply_double_diff`, estilo *Greg and Array*).
- **Cuándo no usar / alternativa:** Si hay consultas de suma intercaladas con actualizaciones de rango, usar **Lazy Segment Tree** (§4.5.1) o dos Fenwick Trees.
- **Contrato:** Indexación **1-based** (`1..n`). `add_range(l, r, val)` suma `val` a `[l..r]`; `build()` devuelve `res[1..n]`.
- **Complejidad:** $\mathcal{O}(1)$ por actualización; $\mathcal{O}(N)$ para `build()`.

-- diff_array_1d.cpp

- **Lectura de entrada y uso:** Aplicar $Q$ sumas en rango **1-based** `[l, r]` y reconstruir `res[1..n]` al final con `build()`:
```cpp
int n, q; cin >> n >> q;
DiffArray1D da(n);
while (q--) {
    int l, r; ll v; cin >> l >> r >> v;
    da.add_range(l, r, v);
}
vll res = da.build(); // res[1..n]
```
- **Trampas y adaptaciones:** (1) El vector interno `diff` debe tener tamaño `n + 2` porque `add_range` accede a `diff[r + 1]` cuando `r == n`.

#### 3.5.2 Difference Array 2D en Submatrices (`DiffArray2D`)

- **Cuándo usar:** Sumar `val` a $Q$ rectángulos $[r_1..r_2] \times [c_1..c_2]$ en $\mathcal{O}(1)$ cada uno y reconstruir la matriz final una sola vez al terminar.
- **Cuándo no usar / alternativa:** Si se intercalan actualizaciones rectangulares y consultas de submatriz, no basta un Difference Array estático.
- **Contrato:** Coordenadas **1-based** ($1 \le r_1 \le r_2 \le N$, $1 \le c_1 \le c_2 \le M$). `build()` retorna matriz `(n + 1) x (m + 1)`.
- **Complejidad:** $\mathcal{O}(1)$ por `add_rect`; $\mathcal{O}(N \cdot M)$ en `build()`.

-- diff_array_2d.cpp

- **Lectura de entrada y uso:** Aplicar $Q$ sumas rectangulares **1-based** y reconstruir `mat[1..n][1..m]` con `build()`:
```cpp
int n, m, q; cin >> n >> m >> q;
DiffArray2D da(n, m);
while (q--) {
    int r1, c1, r2, c2; ll v;
    cin >> r1 >> c1 >> r2 >> c2 >> v;
    da.add_rect(r1, c1, r2, c2, v);
}
auto mat = da.build(); // mat[1..n][1..m]
```
- **Trampas y adaptaciones:** (1) Recordar los signos de las 4 esquinas: `+val` en `(r1, c1)` y `(r2+1, c2+1)`, y `-val` en `(r1, c2+1)` y `(r2+1, c1)`.

------------------------------------------------------------------------

### 3.6 Algoritmos Greedy: Interval Scheduling, Greedy con Heap (Regret) y Argumento de Intercambio

#### 3.6.1 Interval Scheduling (Selección Máxima de Intervalos Disjuntos)

- **Cuándo usar:** Seleccionar la máxima cantidad de intervalos mutuamente disjuntos ordenando por menor extremo derecho $r_i$.
- **Cuándo no usar / alternativa:** Si cada intervalo tiene un **peso/ganancia** arbitraria y quieres maximizar la suma de pesos, el greedy por extremo derecho falla; usar **DP con búsqueda binaria / Segment Tree** en $\mathcal{O}(N\log N)$.
- **Contrato:** `interval_scheduling(ivs, strict_disjoint)` recibe pares `{l, r}`. Si `strict_disjoint = true`, intervalos que comparten borde ($[1, 2]$ y $[2, 3]$) chocan (`l > last_r`); si `false`, pueden tocarse en el borde (`l >= last_r`).
- **Complejidad:** $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- interval_scheduling.cpp

- **Trampas y adaptaciones:** (1) Nunca ordenar por extremo izquierdo $l_i$ ni por menor longitud $r_i - l_i$: ambos contraejemplos clásicos fallan.

#### 3.6.2 Greedy con arrepentimiento (Moore-Hodgson con Max-Heap) [opcional]

- **Cuándo usar:** Maximizar la cantidad de tareas completadas a tiempo cuando cada tarea tiene duración $t_i \ge 0$ y fecha límite $d_i$.
- **Cuándo no usar / alternativa:** Si cada tarea tiene una ganancia distinta $w_i$ además de $t_i$ y $d_i$, requiere DP tipo Knapsack sobre el tiempo ordenado por $d_i$.
- **Contrato:** Ordena por `deadline` $d_i$ ascendente y mantiene las duraciones elegidas en un max-heap; si el tiempo acumulado supera $d_i$, descarta la tarea aceptada más larga (`pq.top()`).
- **Complejidad:** $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- regret_greedy_heap.cpp

- **Trampas y adaptaciones:** (1) El acumulador `cur_time` debe ser `ll` para no desbordar `int` cuando $t_i \le 10^9$.

#### 3.6.3 Argumento de Intercambio de Smith (`exchange_argument_order`) [opcional]

- **Cuándo usar:** Ordenar tareas con duración $t_i$ y penalización por unidad de tiempo $w_i$ para minimizar $\sum w_i C_i$, donde $C_i$ es el tiempo de finalización acumulado.
- **Cuándo no usar / alternativa:** Si hay precedencias (DAG de dependencias generales), el orden plano por $t_i / w_i$ no respeta topología (en árbol requiere fusión con DSU + heap).
- **Contrato:** Compara dos tareas $a, b$ con producto cruzado en 128 bits `(i128)a.t * b.w < (i128)b.t * a.w` sin usar punto flotante. Retorna `{costo_minimo, orden_indices}`.
- **Complejidad:** $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- exchange_argument.cpp

- **Trampas y adaptaciones:** (1) Tratar $w_i = 0, t_i > 0$ enviándolos al final; (2) el comparador debe usar `<` estricto (nunca `<=`) para no producir UB en `std::sort`.

------------------------------------------------------------------------

### 3.7 Barrido de Eventos / Intervalos 1D (Sweep-Line)

#### 3.7.1 Máximo Solapamiento de Intervalos por Sweep-Line (`max_interval_overlap`)

- **Cuándo usar:** Hallar la máxima cantidad de intervalos simultáneamente activos y el primer punto donde se alcanza dicho máximo.
- **Cuándo no usar / alternativa:** Si necesitas actualizar y consultar solapamientos de rangos dinámicamente con inserciones y borrados intercalados sobre coordenadas fijas, usar **Lazy Segment Tree** (§4.5.1).
- **Contrato:** Si `closed_ends = true` (intervalos cerrados $[l, r]$), la entrada `+1` procesa **antes** que la salida `-1` en la misma coordenada; si `false` (semiabiertos $[l, r)$), la salida `-1` procesa **antes** que la entrada `+1`. Retorna `{max_solapamiento, coordenada}`.
- **Complejidad:** $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- sweep_line_1d.cpp

- **Trampas y adaptaciones:** (1) El orden de desempate en `a.fi == b.fi` entre eventos de apertura y cierre es la causa #1 de WA en sweep-line.

#### 3.7.2 Unión de Intervalos Cerrados y Longitud Cubierta (`merge_intervals`)

- **Cuándo usar:** Colapsar una lista de intervalos cerrados $[l_i, r_i]$ posiblemente solapados en una lista ordenada de intervalos disjuntos y medir la longitud continua cubierta.
- **Cuándo no usar / alternativa:** Si buscas intersección de todos los intervalos en vez de su unión, basta `[max(l_i), min(r_i)]` en $\mathcal{O}(N)$.
- **Contrato:** Fusiona intervalos cuando `ivs[i].fi <= res.back().se`. Retorna `{intervalos_unidos, longitud_continua_total}` donde cada intervalo aporta `r - l`.
- **Complejidad:** $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- sweep_line_union_length.cpp

- **Trampas y adaptaciones:** (1) Si se cuenta la cantidad de **puntos enteros** cubiertos por intervalos cerrados $[l, r]$, cada intervalo aporta `(r - l + 1)` y dos intervalos contiguos `[1, 2]` y `[3, 4]` pueden fusionarse con `ivs[i].fi <= res.back().se + 1`.
