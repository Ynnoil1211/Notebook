------------------------------------------------------------------------

## Bloque 5: Algoritmos de Strings

### 5.1 Rotación Cíclica Mínima / Minimum Expression (Duval/Booth $\mathcal{O}(N)$) [opcional]

- **Cuándo usar:** Encontrar el índice de inicio de la rotación cíclica lexicográficamente menor de una cadena $S$ en $\mathcal{O}(|S|)$ (canonización de collares o polígonos).
- **Cuándo no usar / alternativa:** Si necesitas ordenar **todas** las rotaciones cíclicas de $S$, usar `SuffixArray` sobre $S + S$ (§5.6.1).
- **Contrato:** `min_cyclic_rotation(s)` retorna el índice **0-based** $i \in [0, |S|-1]$ donde empieza la menor rotación. La cadena rotada es `s.substr(i) + s.substr(0, i)`.
- **Complejidad:** $\mathcal{O}(|S|)$ tiempo y $\mathcal{O}(|S|)$ memoria.

-- min_cyclic_rotation.cpp

- **Trampas y adaptaciones:** (1) Si todas las rotaciones mínimas empatan, el algoritmo de Duval retorna la primera aparición del bloque mínimo.

------------------------------------------------------------------------

### 5.2 Double Rolling Hash Polinomial y LCP con Búsqueda Binaria

- **Cuándo usar:** Comparar subcadenas `s[l1..r1]` y `s[l2..r2]` por igualdad en $\mathcal{O}(1)$ y hallar su Mayor Prefijo Común (`lcp`) u orden lexicográfico (`cmp_substr`) en $\mathcal{O}(\log N)$.
- **Cuándo no usar / alternativa:** Si solo necesitas comparar prefijos de la misma cadena con el prefijo global `s[0..n-1]`, usar **Z-Algorithm** (§5.4.1) que es determinista y $\mathcal{O}(N)$.
- **Contrato:** Índices **0-based** (`0 <= l <= r < n`). `get(l, r)` retorna el par de hashes `(h1, h2)` de `s[l..r]`. El doble módulo reduce drásticamente la tasa de colisión accidental, pero con base fija un adversario que conozca `BASE` y `M1, M2` puede construir colisiones (aleatorizar `BASE` con `rd(257, 1e9)` si hay *open hacking*).
- **Complejidad:** Construcción $\mathcal{O}(N)$; `get(l, r)` en $\mathcal{O}(1)$; `lcp` y `cmp_substr` en $\mathcal{O}(\log N)$.

-- rolling_hash.cpp

- **Lectura de entrada y uso:** Construir `PolyHash rh(s)` y consultar rangos **0-based** `[l, r]`:
```cpp
string s; int q; cin >> s >> q;
PolyHash rh(s);
while (q--) {
    int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2;
    bool eq = (rh.get(l1, r1) == rh.get(l2, r2)); // 0-based
}
```
- **Trampas y adaptaciones:** (1) Nunca mapear `'a' -> 0` (si `'a' == 0`, entonces `"a"`, `"aa"` y `"aaa"` tendrían todas hash `0`).

------------------------------------------------------------------------

### 5.3 KMP (Función Prefijo $\pi$, Pattern Matching, Periodo) y Autómata KMP

#### 5.3.1 KMP: Función Prefijo $\pi$ y Búsqueda de Patrón (`prefix_function`, `kmp_search`)

- **Cuándo usar:** Buscar todas las ocurrencias exactas de un patrón `pat` dentro de `text` en $\mathcal{O}(|T| + |P|)$ determinista, o hallar el periodo mínimo de una cadena ($k = n - \pi[n - 1]$).
- **Cuándo no usar / alternativa:** Si hay **múltiples patrones** simultáneos ($P_1, \dots, P_k$) que buscar sobre el mismo texto, usar **Aho-Corasick** (§5.5.3).
- **Contrato:** Índices **0-based**. `pi[i]` es la longitud del mayor prefijo propio de `s[0..i]` que también es sufijo de `s[0..i]`. `kmp_search(text, pat)` retorna los índices de inicio 0-based en `text` (vacío si no ocurre).
- **Complejidad:** $\mathcal{O}(|T| + |P|)$ tiempo y $\mathcal{O}(|P|)$ memoria.

-- kmp.cpp

- **Trampas y adaptaciones:** (1) Si las cadenas pueden contener espacios, leer con `getline` en vez de `cin >>`; (2) el periodo $k = n - \pi[n - 1]$ es un periodo exacto completo si y solo si $n \bmod k == 0$.

#### 5.3.2 Autómata KMP (`build_kmp_automaton`) [opcional]

- **Cuándo usar:** Construir el DFA completo del patrón `p` (`aut[estado][letra]`) para hacer **DP o exponenciación de matrices** contando cuántas cadenas de longitud $L$ contienen (o no contienen) a `p` como subcadena.
- **Cuándo no usar / alternativa:** Si solo buscas `p` una vez en un texto fijo, `kmp_search` (§5.3.1) ahorra el factor $26$ de tiempo y memoria.
- **Contrato:** Requiere alfabeto `'a'..'z'` minúsculas. `aut` tiene dimensión `(|p| + 1) x 26`; el estado `|p|` representa haber completado el patrón `p`. Dependencias: `prefix_function` (§5.3.1).
- **Complejidad:** $\mathcal{O}(26 \cdot |P|)$ tiempo y memoria.

-- kmp_automaton.cpp

- **Trampas y adaptaciones:** (1) Si el alfabeto incluye mayúsculas o dígitos, ampliar la segunda dimensión de `26` al tamaño del alfabeto y ajustar `c - 'a'`.

------------------------------------------------------------------------

### 5.4 Z-Algorithm y Algoritmo de Manacher (Palíndromos Pares e Impares)

#### 5.4.1 Z-Algorithm (`z_function`)

- **Cuándo usar:** Calcular en $\mathcal{O}(N)$ para todo $i \in [1, N-1]$ la longitud del Mayor Prefijo Común (`z[i]`) entre `s` y el sufijo `s[i..N-1]`.
- **Cuándo no usar / alternativa:** Si necesitas el LCP entre **cualquier** par de sufijos `s[i..]` y `s[j..]`, usar `SuffixArray` + Kasai (§5.6.1) o `PolyHash` (§5.2).
- **Contrato:** Índices **0-based**. Por convención `z[0] = 0` (algunos problemas definen `z[0] = n`; asignarlo manualmente si se requiere).
- **Complejidad:** $\mathcal{O}(|S|)$ tiempo y $\mathcal{O}(|S|)$ memoria.

-- z_algorithm.cpp

- **Trampas y adaptaciones:** (1) Para buscar `pat` en `text` con Z-Algorithm, construir `pat + '#' + text` con un carácter separador `'#'` que no aparezca en ninguna de las dos cadenas.

#### 5.4.2 Algoritmo de Manacher para Palíndromos Pares e Impares (`Manacher`) [opcional]

- **Cuándo usar:** Encontrar en $\mathcal{O}(|S|)$ el radio del mayor palíndromo centrado en cada carácter y entre cada par de caracteres adyacentes, y consultar si cualquier subcadena `s[l..r]` es palíndromo en $\mathcal{O}(1)$ (`is_pal(l, r)`).
- **Cuándo no usar / alternativa:** Si necesitas contar cuántos palíndromos **distintos** existen o sus frecuencias como estados de un autómata, usar `PalindromicTree` / Eertree (§5.6.3).
- **Contrato:** Índices **0-based** (`0 <= l <= r < |s|`). `is_pal(l, r)` responde en $\mathcal{O}(1)$; `max_pal_len()` retorna la longitud del mayor palíndromo en `s`.
- **Complejidad:** Construcción $\mathcal{O}(|S|)$ tiempo y memoria; `is_pal(l, r)` en $\mathcal{O}(1)$.

-- manacher.cpp

- **Trampas y adaptaciones:** (1) La cadena transformada interna `t` tiene tamaño `2 * |s| + 3` con centinelas `^` y `$`; usar siempre el método `is_pal(l, r)` para evitar errores de mapeo de índices.

------------------------------------------------------------------------

### 5.5 Trie (Strings y Trie Binario para Max XOR) y Aho-Corasick (Múltiples Patrones)

#### 5.5.1 Trie de Strings: Prefijos y Palabras Exactas (`StringTrie`)

- **Cuándo usar:** Insertar un diccionario de cadenas y consultar cuántas palabras comparten un prefijo dado (`count_prefix(s)`) o cuántas veces se insertó exactamente `s` (`count_exact(s)`).
- **Cuándo no usar / alternativa:** Si necesitas buscar todas las palabras del diccionario como subcadenas dentro de un texto largo, usar **Aho-Corasick** (§5.5.3) que agrega enlaces de fallo al Trie.
- **Contrato:** Alfabeto `'a'..'z'` minúsculas. `count_prefix(s)` y `count_exact(s)` retornan `0` si el camino no existe.
- **Complejidad:** `insert`, `count_prefix`, `count_exact` en $\mathcal{O}(|S|)$ tiempo; memoria $\mathcal{O}(26 \cdot \sum |S|)$.

-- string_trie.cpp

- **Lectura de entrada y uso:** Insertar las $N$ cadenas con `tr.insert(s)` y consultar con `tr.count_prefix(p)` / `tr.count_exact(p)`:
```cpp
int n, q; cin >> n >> q;
StringTrie tr;
forn(i, n) { string s; cin >> s; tr.insert(s); }
while (q--) {
    string p; cin >> p; cout << tr.count_prefix(p) << "\n";
}
```
- **Trampas y adaptaciones:** (1) Si la memoria es ajustada y el alfabeto es grande, reemplazar `int nxt[26]` por `map<char, int>` (paga $\mathcal{O}(\log \Sigma)$ pero ahorra memoria).

#### 5.5.2 Trie Binario para Max/Min XOR Dinámico (`BitTrie`) [opcional]

- **Cuándo usar:** Mantener un multiconjunto dinámico de enteros bajo inserción (`update(x, +1)`), borrado (`update(x, -1)`) y consultar en $\mathcal{O}(B)$ qué elemento $y$ del conjunto maximiza `x ^ y` (`max_xor(x)`) o minimiza `x ^ y` (`min_xor(x)`).
- **Cuándo no usar / alternativa:** Si el problema pide maximizar el XOR de un **subconjunto de elementos** (no de un único elemento $y$), usar **Base de XOR / Gauss Binario** (`XorBasis`, §7.6.2).
- **Contrato:** Precondición obligatoria: el Trie **no debe estar vacío** al llamar `max_xor(x)` o `min_xor(x)`, y solo se debe borrar (`delta = -1`) un valor que haya sido insertado previamente. `B = 30` cubre $0 \le x < 2^{30}$ (cambiar `B = 61` para `ll`).
- **Complejidad:** $\mathcal{O}(B)$ tiempo por `update`, `max_xor`, `min_xor`; memoria $\mathcal{O}(N \cdot B)$.

-- bit_trie.cpp

- **Lectura de entrada y uso:** `update(x, +1)` inserta $x$, `update(x, -1)` borra una copia de $x$:
```cpp
BitTrie bt; bt.update(0, +1); // ej: prefijo vacio
bt.update(x, +1);             // insertar x
ll mx = bt.max_xor(q);        // max(q ^ y)
bt.update(x, -1);             // borrar una copia de x
```
- **Trampas y adaptaciones:** (1) Para contar cuántos elementos $y$ cumplen $(x \oplus y) < K$, recorrer de `B` a `0`: si el bit de $K$ es `1`, sumar `cnt` de la rama donde `(x ^ y)` tiene bit `0` y bajar por la rama donde tiene bit `1`.

#### 5.5.3 Autómata de Aho-Corasick para Múltiples Patrones (`AhoCorasick`) [opcional]

- **Cuándo usar:** Buscar simultáneamente un diccionario de patrones $P_1, \dots, P_k$ dentro de un texto $T$ en tiempo lineal, o hacer DP sobre el autómata para contar cadenas que evitan todas las palabras prohibidas.
- **Cuándo no usar / alternativa:** Para un solo patrón, KMP (§5.3.1) no requiere la matriz `26 * sum(|P|)`.
- **Contrato:** Alfabeto `'a'..'z'`. Llamar a `insert(s, id)` para cada patrón y **obligatoriamente** llamar a `build()` una sola vez antes de procesar textos o usar las transiciones `t[u].next[c]`.
- **Complejidad:** `insert` en $\mathcal{O}(\sum |P_i|)$; `build` en $\mathcal{O}(26 \sum |P_i|)$; `count_matches(text)` en $\mathcal{O}(|T|)$.

-- aho_corasick.cpp

- **Lectura de entrada y uso:** Insertar todos los patrones, llamar **una vez** a `ac.build()` y evaluar el texto:
```cpp
int k; cin >> k;
AhoCorasick ac;
forn(i, k) { string p; cin >> p; ac.insert(p, i); }
ac.build();
string txt; cin >> txt;
cout << ac.count_matches(txt) << "\n";
```
- **Trampas y adaptaciones:** (1) Si necesitas la frecuencia individual de **cada** patrón $P_i$ (en vez de la suma total), acumular visitas en los nodos durante el recorrido de `text` y propagar en orden inverso de BFS hacia `t[u].link`.

------------------------------------------------------------------------

### 5.6 Suffix Array + LCP, Suffix Automaton (SAM) y Palindromic Tree (Eertree) [opcional]

#### 5.6.1 Suffix Array y Arreglo LCP de Kasai (`SuffixArray`) [opcional]

- **Cuándo usar:** Ordenar lexicográficamente todos los sufijos de $S$, calcular el LCP entre sufijos adyacentes (`lcp[i]`) y contar subcadenas distintas ($\frac{N(N+1)}{2} - \sum \text{lcp}[i]$) o buscar patrones con búsqueda binaria.
- **Cuándo no usar / alternativa:** Si necesitas consultar el número de ocurrencias de subcadenas por longitud o el estado mínimo que las acepta, `SuffixAutomaton` (§5.6.2) es más directo.
- **Contrato:** `sa` tiene tamaño `N + 1` porque incluye el sufijo vacío centinela en `sa[0] = N`; los sufijos reales están en `sa[1..N]`. `lcp[i]` es el LCP entre `sa[i]` y `sa[i - 1]`.
- **Complejidad:** Construcción de `sa` en $\mathcal{O}(N \log N)$ y `lcp` (Kasai) en $\mathcal{O}(N)$; memoria $\mathcal{O}(N)$.

-- suffix_array.cpp

- **Lectura de entrada y uso:** Sufijos reales en `sa[1..n]`; `lcp[i] = LCP(sa[i-1], sa[i])` para $i \in [2..n]$:
```cpp
string s; cin >> s; int n = sz(s);
SuffixArray sa(s);
ll dist_subs = 1LL * n * (n + 1) / 2;
for (int i = 2; i <= n; ++i) dist_subs -= sa.lcp[i];
```
- **Trampas y adaptaciones:** (1) Para el LCP entre dos sufijos cualesquiera `i` y `j` con `rk[i] < rk[j]`, construir una `SparseTable` (§4.2) sobre `lcp` y consultar el mínimo en `[rk[i] + 1, rk[j]]`.

#### 5.6.2 Suffix Automaton (`SuffixAutomaton`) [opcional]

- **Cuándo usar:** Representar de forma comprimida todas las subcadenas de $S$ en a lo sumo $2|S| - 1$ estados: contar subcadenas distintas, hallar la **subcadena contigua común más larga** (`longest_common_substring`) entre $S$ y $T$, o contar cuántas veces aparece cada subcadena (`propagate_occurrences`).
- **Cuándo no usar / alternativa:** No confundir `longest_common_substring` (subcadena **contigua** en $\mathcal{O}(|S| + |T|)$) con `lcs_string` (§8.1.2, subsecuencia no necesariamente contigua en $\mathcal{O}(|S| \cdot |T|)$).
- **Contrato:** Estado raíz = `0`. Cada estado `u` representa todas las subcadenas de longitud en `[st[st[u].link].len + 1, st[u].len]`. `propagate_occurrences()` ordena por longitud para acumular `cnt` hacia `link`.
- **Complejidad:** Construcción $\mathcal{O}(|S| \log \Sigma)$ (con `map`); `propagate_occurrences` en $\mathcal{O}(|S| \log |S|)$ por el `sort`; `longest_common_substring` en $\mathcal{O}(|T| \log \Sigma)$.

-- suffix_automaton.cpp

- **Lectura de entrada y uso:** Construir `SuffixAutomaton sam(s)` y llamar `sam.prepare_occurrences()` si necesitas frecuencias `st[u].cnt`:
```cpp
string s, t; cin >> s >> t;
SuffixAutomaton sam(s);
sam.prepare_occurrences();
ll sub = sam.count_distinct_substrings();
string lcs = sam.longest_common_substring(t);
```
- **Trampas y adaptaciones:** (1) Si el alfabeto es fijo `'a'..'z'` y el tiempo es muy ajustado, cambiar `map<char, int> next` por `int next[26]` inicializado en `-1` para quitar el factor $\log \Sigma$.

#### 5.6.3 Palindromic Tree / Eertree (`PalindromicTree`) [opcional]

- **Cuándo usar:** Construir en $\mathcal{O}(|S|)$ el árbol de todas las subcadenas palindrómicas **distintas** de $S$ (a lo sumo $|S|$ palíndromos distintos), manteniendo sus longitudes, enlaces sufijos (`link`) y frecuencias.
- **Cuándo no usar / alternativa:** Si solo necesitas saber si `s[l..r]` es palíndromo o el palíndromo más largo global, `Manacher` (§5.4.2) es más corto.
- **Contrato:** Alfabeto `'a'..'z'`. El nodo `1` es la raíz imaginaria de longitud `-1` y el nodo `2` es la raíz vacía de longitud `0`; los palíndromos reales son los nodos `3..sz(t)-1` (por lo que hay `sz(t) - 3` palíndromos distintos).
- **Complejidad:** $\mathcal{O}(|S|)$ tiempo y $\mathcal{O}(26 \cdot |S|)$ memoria.

-- palindromic_tree.cpp

- **Lectura de entrada y uso:** Construir `PalindromicTree pt(s)`; cantidad de palíndromos distintos es `sz(pt.st) - 2`:
```cpp
string s; cin >> s;
PalindromicTree pt(s);
int dist_pal = sz(pt.st) - 2;
for (int i = sz(pt.st) - 1; i >= 2; --i)
    pt.st[pt.st[i].link].cnt += pt.st[i].cnt;
```
- **Trampas y adaptaciones:** (1) Para obtener el número total de ocurrencias de cada palíndromo en $S$, recorrer los nodos desde `sz(pt.st) - 1` hacia `2` haciendo `st[st[i].link].cnt += st[i].cnt`.
