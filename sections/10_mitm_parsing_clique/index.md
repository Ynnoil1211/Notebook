------------------------------------------------------------------------

## Bloque 10: Búsqueda en Mitades (MITM), Parsing y Clique Máximo

### 10.1 Meet in the Middle (MITM): Subset Sum Exacto, Conteo con Multiplicidades y Máximo $\le S$

#### 10.1.1 Meet in the Middle: Conteo de Subconjuntos con Suma Exacta (`get_subset_sums`, `mitm_subset_sum_count`)

- **Cuándo usar:** Contar cuántos de los $2^N$ subconjuntos suman exactamente `target` cuando $N \in [25, 42]$ y los valores $a_i \le 10^{12}$ no permiten DP por suma.
- **Cuándo no usar / alternativa:** Si $N \le 20$, basta una sola mitad; si $\sum a_i \le 10^6$, usar Knapsack DP (§8.1.1).
- **Contrato:** Usa `inplace_merge` para generar las $2^{N/2}$ sumas de cada mitad **ya ordenadas en $\mathcal{O}(2^{N/2})$** sin pagar el factor $\log(2^{N/2})$ de `std::sort`. Cuenta multiplicidades con `equal_range`.
- **Complejidad:** $\mathcal{O}(2^{N/2} \cdot (N/2))$ tiempo y $\mathcal{O}(2^{N/2})$ memoria.

-- mitm_subset_sum_count.cpp

- **Trampas y adaptaciones:** (1) Los subconjuntos se distinguen por sus **índices** (si `a = {5, 5}` y `target = 5`, cuenta 2 subconjuntos).

#### 10.1.2 Meet in the Middle: Mayor Suma de Subconjunto $\le S$ (`mitm_max_subset_sum_le`)

- **Cuándo usar:** Encontrar la mayor suma de un subconjunto que no exceda `limit` ($N \le 42, \text{limit} \le 10^{18}$) combinando las dos mitades ordenadas con dos punteros en $\mathcal{O}(2^{N/2})$.
- **Cuándo no usar / alternativa:** Asume `limit >= 0` (pues el subconjunto vacío suma `0`).
- **Contrato:** Genera `L` y `R` ordenados con `get_subset_sums` y barre `x` creciente en `L` contra `j` decreciente en `R` en tiempo lineal sobre las listas. Dependencias: `get_subset_sums` (§10.1.1).
- **Complejidad:** $\mathcal{O}(2^{N/2})$ tiempo y memoria.

-- mitm_max_subset_sum.cpp

- **Lectura de entrada y uso:** Leer `a[0..n-1]` ($N \le 40$) y el objetivo $S$:
```cpp
int n; ll S; cin >> n >> S;
vll a(n); forn(i, n) cin >> a[i];
ll ways = mitm_subset_sum_count(a, S);   // suma == S
ll best = mitm_max_subset_sum_le(a, S);  // max suma <= S
```
- **Trampas y adaptaciones:** (1) Gracias a `inplace_merge` + dos punteros, este algoritmo corre en $\mathcal{O}(2^{N/2})$ puro sin ningún factor logarítmico.

------------------------------------------------------------------------

### 10.2 Evaluador de Expresiones Aritméticas (Shunting-Yard Parser)

- **Cuándo usar:** Evaluar en $\mathcal{O}(|S|)$ una cadena con números enteros, paréntesis `()`, operadores binarios `+ - * /` y signos unarios `+x` y `-x` respetando precedencia y asociatividad.
- **Cuándo no usar / alternativa:** Si el problema permite usar Python y la expresión no excede límites de seguridad ni tiene división `/` con semántica hacia cero para negativos, ver §12.15.
- **Contrato:** Usa el centinela `UNARY_NEG = '~'` (compatible con `-funsigned-char`) con prioridad `3` y asociatividad a derecha (`- - 5 == 5`).
- **Complejidad:** $\mathcal{O}(|S|)$ tiempo y memoria.

-- shunting_yard.cpp

- **Lectura de entrada y uso:** Leer la línea completa (con posibles espacios) usando `getline(cin >> ws, s)` y llamar `evaluate_expr(s)`:
```cpp
string s; getline(cin >> ws, s);
cout << evaluate_expr(s) << "\n";
```
- **Trampas y adaptaciones:** (1) Para evaluar expresiones modulares, reemplazar `a / b` en `apply_op` por `mul_mod(a, inv_fermat(b))` (§7.4.1).

### 10.3 Clique Máximo (Bron-Kerbosch Branch & Bound) [opcional]

- **Cuándo usar:** Hallar el mayor subconjunto de vértices mutuamente adyacentes (**Maximum Clique**) o el mayor conjunto independiente (**Maximum Independent Set**, pasándole el grafo complemento) para $N \le 60$.
- **Cuándo no usar / alternativa:** Si el grafo es bipartito, el Maximum Independent Set se halla en tiempo polinomial con König (`BipartiteMVC`, §6.6.3).
- **Contrato:** Nodos **0-indexed** (`0..n-1`) con $N \le 64$ representados en máscaras `ull`. Usa pivoteo de Tomita (`u = __builtin_ctzll(P | X)`) y poda por cota superior (`sz(cur) + popcount(P) <= sz(best_clique)`).
- **Complejidad:** $\mathcal{O}(3^{N/3})$ peor caso teórico (extremadamente rápido con poda en grafos de competencia hasta $N \approx 55$).

-- max_clique.cpp

- **Lectura de entrada y uso:** Nodos **0-based** (`0..n-1`); restar 1 si el input viene 1-based:
```cpp
int n, m; cin >> n >> m;
MaxClique mc(n);
forn(i, m) {
  int u, v; cin >> u >> v; mc.add_edge(u - 1, v - 1);
}
int sz_clq = mc.solve(); // nodos en mc.best_clique
```
- **Trampas y adaptaciones:** (1) El **Maximum Independent Set** de $G$ es exactamente el `MaxClique` del grafo complemento $\overline{G}$ (agregar arista $(u, v)$ si y solo si $u \neq v$ y $(u, v) \notin E(G)$).
