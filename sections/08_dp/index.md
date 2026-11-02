------------------------------------------------------------------------

## Bloque 8: Programación Dinámica (DP Clásica, Subconjuntos, Árboles y Optimizaciones)

### 8.1 DP Clásica: Knapsack 0/1 y Unbounded, LCS, Edit Distance, LIS $\mathcal{O}(N\log N)$, Digit DP y SOS DP

#### 8.1.1 Mochila 0/1 en Memoria Lineal (`knapsack_01`)

- **Cuándo usar:** Seleccionar un subconjunto de ítems con peso $w_i$ y valor $v_i$ cuya suma de pesos no supere $W$ ($N \cdot W \le 10^8$) maximizando el valor total.
- **Cuándo no usar / alternativa:** Si la capacidad $W$ es enorme ($W \le 10^9$) pero la suma total de valores $V = \sum v_i \le 10^5$ es pequeña, **invertir el estado**: `dp[v] = menor peso para lograr valor v`.
- **Contrato:** Recorre `w` **hacia atrás** (`W` hacia `weight[i]`) para usar cada ítem a lo sumo una vez.
- **Complejidad:** $\mathcal{O}(N \cdot W)$ tiempo y $\mathcal{O}(W)$ memoria.

-- knapsack.cpp

- **Trampas y adaptaciones:** (1) Para **Unbounded Knapsack** (cada ítem se puede repetir infinitas veces), cambiar únicamente el bucle interno para que avance hacia adelante (`for (int w = weight[i]; w <= W; ++w)`).

#### 8.1.2 Subsecuencia Común Más Larga con Reconstrucción (`lcs_string`)

- **Cuándo usar:** Hallar y reconstruir la cadena más larga que aparece como subsecuencia (no necesariamente contigua) tanto en `a` como en `b` ($|a|, |b| \le 5000$).
- **Cuándo no usar / alternativa:** Si buscas la **subcadena contigua** común más larga, usar `SuffixAutomaton::longest_common_substring` (§5.6.2) en $\mathcal{O}(|a| + |b|)$.
- **Contrato:** Construye la tabla `dp` $(n+1) \times (m+1)$ y reconstruye la cadena retrocediendo desde `(n, m)`.
- **Complejidad:** $\mathcal{O}(N \cdot M)$ tiempo y memoria.

-- lcs.cpp

- **Trampas y adaptaciones:** (1) Si uno de los arreglos es una permutación sin elementos repetidos, renombrar sus valores por sus índices convierte el LCS en un **LIS** en $\mathcal{O}(N \log N)$ (§8.1.4).

#### 8.1.3 Distancia de Edición de Levenshtein (`edit_distance`)

- **Cuándo usar:** Mínimo número de inserciones, borrados y sustituciones de un carácter para transformar la cadena `a` en la cadena `b`.
- **Cuándo no usar / alternativa:** Si no se permiten sustituciones (solo insertar y borrar), la respuesta es directamente $|a| + |b| - 2 \cdot |\text{LCS}(a, b)|$.
- **Contrato:** Casos base: `dp[i][0] = i` (borrar `i` caracteres) y `dp[0][j] = j` (insertar `j` caracteres).
- **Complejidad:** $\mathcal{O}(N \cdot M)$ tiempo y memoria.

-- edit_distance.cpp

- **Trampas y adaptaciones:** (1) Si solo se pide el número de operaciones y la memoria es ajustada, mantener solo dos filas (`dp_prev` y `dp_cur`) reduce la memoria a $\mathcal{O}(\min(N, M))$.

#### 8.1.4 Subsecuencia Creciente Más Larga en $\mathcal{O}(N \log N)$ (`lis_length`)

- **Cuándo usar:** Calcular la longitud de la mayor subsecuencia estrictamente creciente ($a_{i_1} < a_{i_2} < \dots < a_{i_k}$) en $\mathcal{O}(N \log N)$.
- **Cuándo no usar / alternativa:** Si la condición de transición entre elementos consecutivos no es un orden total sino un predicado general, usar Segment Tree o CDQ Divide & Conquer.
- **Contrato:** `tails[k]` almacena el menor valor final posible de una subsecuencia creciente de longitud $k + 1$. Usa `lower_bound` para estrictamente creciente (`<`).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- lis.cpp

- **Trampas y adaptaciones:** (1) Para subsecuencia **no decreciente** ($\le$), cambiar `lower_bound` por `upper_bound`; (2) el vector `tails` **no** es necesariamente la subsecuencia en sí, pero su tamaño `sz(tails)` siempre es la longitud exacta del LIS.

#### 8.1.5 Digit DP por Prefijos Decimales (`dp_dig`, `solve_digit_dp`) [opcional]

- **Cuándo usar:** Contar cuántos enteros en un rango enorme $[L, R]$ ($R \le 10^{18}$ o dado como string de $10^5$ dígitos) cumplen una propiedad sobre sus dígitos (suma divisible por $K$, sin dígitos adyacentes iguales, etc.).
- **Cuándo no usar / alternativa:** Para contar en $[L, R]$, nunca intentar meter simultáneamente cota inferior y superior si no es necesario; calcular simplemente `solve(R) - solve(L - 1)`.
- **Contrato:** Estado `(pos, rem, tight)` donde `tight` indica si el prefijo actual sigue empatado con los dígitos de `R`.
- **Complejidad:** $\mathcal{O}(10 \cdot \text{dígitos}(R) \cdot K)$ tiempo y $\mathcal{O}(\text{dígitos}(R) \cdot K)$ memoria.

-- digit_dp.cpp

- **Trampas y adaptaciones:** (1) Si la propiedad depende de los ceros a la izquierda (ej. contar ceros o exigir dígitos distintos), agregar una bandera booleana `started` al estado.

#### 8.1.6 SOS DP / Suma Sobre Submáscaras (`sos_dp`) [opcional]

- **Cuándo usar:** Calcular para toda máscara `mask` de $N$ bits la suma $F[\text{mask}] = \sum_{\text{sub} \subseteq \text{mask}} A[\text{sub}]$ en $\mathcal{O}(N \cdot 2^N)$ en lugar de iterar submáscaras en $\mathcal{O}(3^N)$.
- **Cuándo no usar / alternativa:** Si la transición a `mask` depende de valores ya calculados de `dp[sub]` que no son una suma fija independiente, usar iteración de submáscaras `for (int sub = mask; sub; sub = (sub - 1) & mask)` en $\mathcal{O}(3^N)$.
- **Contrato:** `A` debe tener tamaño $2^N$. El bucle externo **debe** ser el bit `i` (`0..N-1`) para no sumar la misma submáscara dos veces.
- **Complejidad:** $\mathcal{O}(N \cdot 2^N)$ tiempo y $\mathcal{O}(2^N)$ memoria.

-- sos_dp.cpp

- **Trampas y adaptaciones:** (1) Para sumar sobre **supermáscaras** ($\text{sup} \supseteq \text{mask}$), cambiar la condición `if (mask & (1 << i))` por `if (!(mask & (1 << i))) F[mask] += F[mask | (1 << i)];`.

------------------------------------------------------------------------

### 8.2 DP de Subconjuntos con Máscaras: Asignación `dp[mask]` y Caminos / TSP `dp[mask][last]`

#### 8.2.1 DP de Asignación por Máscaras de Bits (`subset_assignment_dp`)

- **Cuándo usar:** Asignar $N$ elementos a $N$ posiciones/turnos ($N \le 20$) cuando el paso actual está determinado por la cantidad de bits encendidos `step = __builtin_popcount(mask)`, ahorrando un factor $N$ en estado.
- **Cuándo no usar / alternativa:** Si $N > 22$ y se trata de una asignación 1-a-1 de costo aditivo puro sin dependencia de subconjunto, usar el **Algoritmo Húngaro** (§6.6.4) en $\mathcal{O}(N^3)$.
- **Contrato:** Matriz `cost` 0-indexed de tamaño $N \times N$. Retorna el costo mínimo de asignar el paso `step` al elemento `j` para todo `step = 0..N-1`.
- **Complejidad:** $\mathcal{O}(N \cdot 2^N)$ tiempo y $\mathcal{O}(2^N)$ memoria.

-- subset_dp_assignment.cpp

- **Trampas y adaptaciones:** (1) Iterar solo los bits apagados o encendidos con `__builtin_ctz` reduce la constante práctica a la mitad.

#### 8.2.2 DP de Caminos / Travelling Salesperson Problem con Reconstrucción (`tsp_subset_dp`)

- **Cuándo usar:** Encontrar el ciclo o camino Hamiltoniano de menor peso que visita todos los $N$ nodos exactamente una vez ($N \le 20$) cuando la transición depende del último nodo visitado `last`.
- **Cuándo no usar / alternativa:** Si quieres recorrer todas las **aristas** en vez de los vértices, es un Ciclo Euleriano en $\mathcal{O}(V + E)$ (§6.9).
- **Contrato:** Nodos 0-indexed (`0..n-1`), empezando en `0`. Si `close_cycle = true`, exige regresar al nodo `0` al final. Retorna `{costo_minimo, ruta_reconstruida}`.
- **Complejidad:** $\mathcal{O}(N^2 \cdot 2^N)$ tiempo y $\mathcal{O}(N \cdot 2^N)$ memoria.

-- subset_dp_tsp.cpp

- **Trampas y adaptaciones:** (1) Para camino Hamiltoniano que puede empezar en **cualquier** nodo, inicializar `dp[1 << i][i] = 0` para todo $i \in [0, N-1]$ y pasar `close_cycle = false`.

------------------------------------------------------------------------

### 8.3 DP en Árboles y Técnica de Rerooting (Cambio de Raíz en $\mathcal{O}(N)$) [opcional]

- **Cuándo usar:** Calcular una función de árbol (suma de distancias, máxima distancia, DP de subárbol) **para todas las raíces posibles $u \in [1..N]$** en dos pasadas DFS (`dfs_down` y `dfs_up`).
- **Cuándo no usar / alternativa:** Si la respuesta solo se pide para una raíz fija, basta un solo DFS abajo-arriba.
- **Contrato:** Árbol 1-indexed (`1..n`). `dfs_down` calcula `sub_sz[u]` y `down[u]`; `dfs_up` propaga hacia cada hijo `v` usando `ans[v] = ans[u] + (n - sub_sz[v]) - sub_sz[v]`.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y memoria.

-- rerooting_dp.cpp

- **Trampas y adaptaciones:** (1) Cuando la operación que combina hijos no es invertible (como `max` o producto módulo $M$), precalcular prefijos y sufijos sobre la lista de hijos de `u` para excluir la rama de `v` en $\mathcal{O}(1)$.

------------------------------------------------------------------------

### 8.4 Optimizaciones de DP: Convex Hull Trick (Estático y Dinámico), Divide & Conquer DP y Knuth’s Optimization [opcional]

#### 8.4.1 Convex Hull Trick Dinámico (`LineContainer`) [opcional]

- **Cuándo usar:** Optimizar transiciones de DP de la forma $dp[i] = \max_j (k_j x_i + m_j)$ (o $\min_j$) cuando las pendientes $k_j$ y los puntos de consulta $x_i$ llegan en **orden arbitrario**.
- **Cuándo no usar / alternativa:** Si tanto las pendientes como los puntos de consulta son monótonos, `MonotoneCHT` (§8.4.2) corre en $\mathcal{O}(N)$ sin `std::multiset`.
- **Contrato:** `add(k, m)` inserta $y = k x + m$; `query(x)` devuelve el **máximo** en `x`. Para consultar el **mínimo**, insertar `add(-k, -m)` y tomar `-query(x)`.
- **Complejidad:** $\mathcal{O}(\log N)$ amortizado por inserción y consulta; $\mathcal{O}(N)$ memoria.

-- cht_dynamic.cpp

- **Trampas y adaptaciones:** (1) No llamar `query(x)` con el contenedor vacío (`assert(!empty())`); insertar primero el caso base `dp[0]`.

#### 8.4.2 Convex Hull Trick Estático / Monótono con Deque (`MonotoneCHT`) [opcional]

- **Cuándo usar:** Consultar el **mínimo** de rectas $y = m x + b$ en $\mathcal{O}(1)$ amortizado cuando las pendientes $m$ se insertan en orden estrictamente **decreciente** y las consultas $x$ se hacen en orden **creciente**.
- **Cuándo no usar / alternativa:** Si las pendientes o las consultas no son monótonas, usar `LineContainer` (§8.4.1).
- **Contrato:** Compara intersecciones con multiplicación cruzada en `i128` sin división ni punto flotante.
- **Complejidad:** $\mathcal{O}(1)$ amortizado por `add` y `query_monotone`; $\mathcal{O}(N)$ memoria.

-- cht_monotone.cpp

- **Trampas y adaptaciones:** (1) Si tienes pendientes iguales con distinta ordenada al origen, conservar solo la de menor `b` antes de llamar a `add`.

#### 8.4.3 Optimización Divide & Conquer DP (`compute_dc`, `solve_dc_dp`) [opcional]

- **Cuándo usar:** Particionar un arreglo `1..N` en exactamente `K` subarreglos contiguos minimizando $\sum C(j, i)$ cuando la función de costo cumple la **desigualdad cuadrangular** $C(a, c) + C(b, d) \le C(a, d) + C(b, c)$ (lo que implica monotonía del punto óptimo $\text{opt}[k][i] \le \text{opt}[k][i + 1]$).
- **Cuándo no usar / alternativa:** Si el costo no cumple monotonía del óptimo pero es bilineal $f(j) g(i)$, usar CHT (§8.4.1).
- **Contrato:** Índices **1-based** (`1..n`). `cost_fn(j, i)` devuelve el costo del bloque cerrado `[j, i]`. Ignora estados inalcanzables (`dp_prev[j - 1] >= INF`) sin corromper `opt`.
- **Complejidad:** $\mathcal{O}(K \cdot N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- divide_and_conquer_dp.cpp

- **Trampas y adaptaciones:** (1) En problemas donde evaluar $C(l, r)$ cuesta $\mathcal{O}(1)$ con dos punteros tipo Mo (ej. pares iguales en `[l, r]`), mantener una ventana `[cur_l, cur_r]` global dentro de `compute_dc` sigue costando $\mathcal{O}(K N \log N)$.

#### 8.4.4 Optimización de Knuth para DP de Intervalos (`solve_knuth`) [opcional]

- **Cuándo usar:** Reducir de $\mathcal{O}(N^3)$ a $\mathcal{O}(N^2)$ la DP de intervalos $dp[l][r] = \min_{l \le k < r}(dp[l][k] + dp[k+1][r]) + C[l][r]$ (como *Optimal Binary Search Tree* o partir una barra/cuerda) cuando $C[l][r]$ cumple desigualdad cuadrangular y monotonía en inclusión.
- **Cuándo no usar / alternativa:** En problemas como multiplicación de cadenas de matrices o *Burst Balloons*, el término sumado depende de `k` ($a_l a_k a_r$), por lo que Knuth no aplica.
- **Contrato:** Matriz `C` 1-indexed de tamaño $(n+1) \times (n+1)$. Acota la búsqueda de `k` a `opt[l][r - 1] <= k <= min(r - 1, opt[l + 1][r])`.
- **Complejidad:** $\mathcal{O}(N^2)$ tiempo y memoria.

-- knuth_dp.cpp

- **Trampas y adaptaciones:** (1) Iterar siempre por longitud creciente `for (int len = 2; len <= n; ++len)` para que `opt[l][r - 1]` y `opt[l + 1][r]` ya estén calculados.
