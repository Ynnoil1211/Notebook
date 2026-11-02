------------------------------------------------------------------------

## Bloque 2: Contenedores STL, PBDS y sus Métodos Más Importantes

### 2.1 `std::vector` y `<algorithm>`: Métodos, `sort`, Búsqueda Binaria (`lower_bound`/`upper_bound`) y Compresión de Coordenadas

- **Cuándo usar:** Mapear valores arbitrarios en $[-10^{18}, 10^{18}]$ a rangos densos $[0, K-1]$ (`CoordComp`) para usarlos como índices de Fenwick/Segment Tree, y realizar búsquedas binarias en arreglos ordenados.
- **Cuándo no usar / alternativa:** Si el conjunto de llaves cambia dinámicamente con inserciones de valores nuevos no conocidos al inicio y necesitas orden, usar `std::set`, `ordered_set` (§2.6) o Segment Tree dinámico.
- **Contrato:** `CoordComp::init(a)` ordena y elimina duplicados; `get_id(x)` retorna el índice 0-based en $[0, K-1]$ del primer valor $\ge x$; `get_val(id)` recupera el valor original.
- **Complejidad:** `init` en $\mathcal{O}(N\log N)$ tiempo y $\mathcal{O}(N)$ memoria; `get_id` en $\mathcal{O}(\log N)$.

-- coord_comp.cpp

- **Lectura de entrada y uso:** Leer $N$ valores en `vll a(n)`, llamar `cc.init(a)` y mapear cada $a[i]$ a $[0, K-1]$ con `cc.get_id(a[i])`:
```cpp
int n; cin >> n;
vll a(n); forn(i, n) cin >> a[i];
CoordComp cc; cc.init(a);
forn(i, n) a[i] = cc.get_id(a[i]); // +1 si va a Fenwick
```
- **Trampas y adaptaciones:** (1) En `accumulate(all(v), 0LL)`, pasar siempre `0LL` para evitar overflow en `int`; (2) si el Fenwick Tree es 1-indexed, sumar `+ 1` al resultado de `cc.get_id(x)`.

------------------------------------------------------------------------

### 2.2 Contenedores Lineales: `std::deque`, `std::queue` y `std::stack`

- **Cuándo usar:** `deque` para inserción/borrado en ambos extremos y acceso aleatorio `dq[i]` en $\mathcal{O}(1)$ (0-1 BFS, ventana deslizante); `queue` para BFS FIFO; `stack` para LIFO y pilas monótonas.
- **Cuándo no usar / alternativa:** No llamar nunca a `front()`, `back()`, `top()` o `pop()` sobre un contenedor vacío (causa comportamiento indefinido / Segmentation Fault).
- **Contrato:** Operaciones sobre extremos en `deque`, `queue` y `stack`. Verificar siempre `!q.empty()` antes de consultar o extraer.
- **Complejidad:** $\mathcal{O}(1)$ tiempo por operación; $\mathcal{O}(N)$ memoria.

-- linear_containers.cpp

- **Trampas y adaptaciones:** (1) `std::queue` y `std::stack` no tienen método `.clear()`; para vaciarlos rápido reasignar `q = queue<int>()` o declararlos locales al caso de prueba; (2) `deque` tiene mayor constante de memoria que `vector`.

------------------------------------------------------------------------

### 2.3 Colas de Prioridad / Heaps (`std::priority_queue`): Max-Heap, Min-Heap, Custom y **Deletable Heap**

- **Cuándo usar:** Extraer el mínimo o máximo dinámico en $\mathcal{O}(\log N)$, y usar `DeletableMinHeap` cuando además de `push`/`top` necesites borrar valores arbitrarios presentes sin pagar la constante de `std::multiset`.
- **Cuándo no usar / alternativa:** En `DeletableMinHeap`, **nunca** llamar `erase(x)` con un valor `x` que no esté actualmente presente en `pq` (corrompería la cola `del` al cancelar inserciones futuras).
- **Contrato:** `DeletableMinHeap<T>`: `push(x)`, `erase(x)` (precondición: `x` está presente), `top()` y `pop()` (precondición: `!empty()`). En `struct State`, `operator<` invierte el signo (`cost > o.cost`) porque `std::priority_queue` es un Max-Heap por defecto.
- **Complejidad:** `push`, `pop`, `erase` en $\mathcal{O}(\log N)$ amortizado; `top` en $\mathcal{O}(1)$ amortizado; $\mathcal{O}(N)$ memoria.

-- priority_queue_deletable_heap.cpp

- **Trampas y adaptaciones:** (1) Recordar que `priority_queue<T>` por defecto saca el **mayor** elemento primero; para Min-Heap usar `priority_queue<T, vector<T>, greater<T>>`; (2) verificar `!dh.empty()` antes de `dh.top()`.

------------------------------------------------------------------------

### 2.4 Árboles Balanceados (`std::set`, `std::multiset`, `std::map`): Métodos, Búsqueda Binaria en `set` y Borrado Seguro en `multiset`

- **Cuándo usar:** Mantener un conjunto o multiconjunto ordenado dinámico con consultas de sucesor (`lower_bound`, `upper_bound`) y predecesor (`prev(it)`) en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** Si no necesitas orden ni consultas de rango/vecinos, usar `safe_umap` / `safe_uset` (§2.5) en $\mathcal{O}(1)$ promedio.
- **Contrato:** Usar siempre el método miembro `s.lower_bound(x)` ($\mathcal{O}(\log N)$) y jamás `std::lower_bound(all(s), x)` ($\mathcal{O}(N)$). En `multiset`, `ms.erase(ms.find(x))` borra **una sola** copia (verificando `it != ms.end()`).
- **Complejidad:** `insert`, `erase(it)`, `find`, `lower_bound`, `upper_bound` en $\mathcal{O}(\log N)$.

-- set_multiset_map.cpp

- **Trampas y adaptaciones:** (1) `ms.count(x)` en un `multiset` tarda $\mathcal{O}(k + \log N)$ donde $k$ es la multiplicidad (usar `ms.find(x) != ms.end()` para existencia); (2) consultar `mp[k]` en `std::map` **inserta** la llave `k` con valor 0 si no existía (usar `mp.find(k)`).

------------------------------------------------------------------------

### 2.5 Tablas Hash (`unordered_map`, `unordered_set`) con `custom_hash` Anti-Hack (SplitMix64) y Pares Iguales

- **Cuándo usar:** Conteo de frecuencias, memorización dispersa o búsqueda de llaves (incluyendo `pair<T1, T2>`) en $\mathcal{O}(1)$ promedio protegiendo contra colisiones maliciosas (*anti-hash tests*).
- **Cuándo no usar / alternativa:** Si necesitas recorrer las llaves en orden creciente o consultar predecesor/sucesor, usar `std::map` / `std::set` (§2.4); si las llaves son enteros en $[0, N]$, usar un `vector` directo.
- **Contrato:** `safe_umap<K, V>` y `safe_uset<K>` aceptan tipos enteros y `pair<T1, T2>`. `count_equal_pairs(a)` cuenta pares $(i < j)$ con $a[i] = a[j]$.
- **Complejidad:** $\mathcal{O}(1)$ tiempo esperado por operación; $\mathcal{O}(N)$ para `count_equal_pairs`.

-- custom_hash.cpp

- **Trampas y adaptaciones:** (1) Llamar a `mp.reserve(2 * N)` y `mp.max_load_factor(0.25f)` reduce los *rehashes* y acelera $2\times$ la tabla; (2) para `string` usar el hash nativo o Rolling Hash (§5.2).

------------------------------------------------------------------------

### 2.6 Extensiones GNU: `ordered_set` / `ordered_multiset` (PBDS) y `__gnu_cxx::rope`

- **Cuándo usar:** `ordered_set` / `ordered_multiset` para responder en $\mathcal{O}(\log N)$ el $k$-ésimo elemento (`find_by_order(k)`, 0-based) y cuántos elementos son $< x$ (`order_of_key(x)`); `rope` para cortar y pegar subarreglos en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** **Prohibido** usar `less_equal<T>` en PBDS para simular un multiset: rompe la asimetría del comparador y hace que `find`, `lower_bound` y `erase` fallen silenciosamente. Usar siempre `ordered_multiset<T>` con `pair<T, int>` e `id` único.
- **Contrato:** `find_by_order(k)` requiere $0 \le k < \text{size()}$. En `ordered_multiset<T>`, insertar `{x, uid++}`; `order_of_key({x, -1})` cuenta elementos $< x$ y `order_of_key({x, INF_INT})` cuenta elementos $\le x$.
- **Complejidad:** $\mathcal{O}(\log N)$ tiempo por operación; $\mathcal{O}(N)$ memoria.

-- pbds_ordered_set.cpp

- **Lectura de entrada y uso:** En `ordered_multiset`, acompañar cada valor con un `uid++` único; borrar con `lower_bound({x, 0})`:
```cpp
ordered_multiset<ll> oms; int uid = 0;
oms.insert({x, uid++}); // insertar x
auto it = oms.lower_bound({x, 0});
if (it != oms.end() && it->fi == x) oms.erase(it);
ll men = oms.order_of_key({x, -1}); // cuantos < x
ll kth = oms.find_by_order(k)->fi;  // k-esimo (0-based)
```
- **Trampas y adaptaciones:** (1) Verificar siempre `k < (int)oms.size()` antes de desreferenciar `find_by_order(k)`; (2) `rope` tiene constante alta; si el límite de tiempo es ajustado o hay reversiones de rango, usar `ImplicitTreap` (§4.7).
