------------------------------------------------------------------------

## Bloque 1: C++ Base, Strings STL, Bitmasks y Cambios de Base

### 1.1 Template Base ICPC y Comparador `EPS`

- **Cuándo usar:** Esqueleto base para todo programa C++ en competencia: fast I/O, alias de tipos, macros de iteración (`forn`, `for1`) y comparación segura con `EPS` (`eps_sgn`, `eps_eq`, `eps_lt`) más cuantización para claves de contenedores.
- **Cuándo no usar / alternativa:** En problemas interactivos, no llamar a `sync_with_stdio(false)` y vaciar la salida (`flush`) tras cada escritura.
- **Contrato:** Define alias `ll`, `ull`, `ld`, `pii`, `pll`, `vi`, `vll`, macros `all`, `sz`, `pb`, `fi`, `se`, `el`, `forn`, `for1`, y constantes `INF_INT` ($10^9$, distancias en `int`), `INF` ($10^{18}$), `EPS` y `PI`. Trae las 8 direcciones de grilla `dr`/`dc`/`dir_char` en orden horario (los índices pares son las 4 rectas, útiles para reconstruir pasos) y un `operator<<` de `pii` como `(x, y)` para depurar. `eps_eq(a, b)` comprueba `|a - b| <= EPS`.
- **Complejidad:** Fast I/O y comparadores en $\mathcal{O}(1)$.

-- cpp_template_base.cpp

- **Trampas y adaptaciones:** (1) Jamás mezclar `scanf`/`printf` con `cin`/`cout` tras `sync_with_stdio(false)`; (2) no usar `eps_lt` como comparador directo de `std::set` porque la cercanía `|a-b| <= EPS` no es transitiva (usar `quantize_eps`); (3) para imprimir reales, `cout << fixed << setprecision(10) << x`.

------------------------------------------------------------------------

### 1.2 Generador Aleatorio (`mt19937_64`), `shuffle` y `rd(l, r)`

- **Cuándo usar:** Generar enteros o reales uniformes en 64 bits (`rd(l, r)`), barajar arreglos antes de `sort`/QuickSelect o alimentar algoritmos aleatorizados (Pollard's Rho, Treap).
- **Cuándo no usar / alternativa:** Nunca usar `rand()` en Windows/MinGW porque `RAND_MAX = 32767` deja sin cubrir índices $> 32767$.
- **Contrato:** Rango cerrado `[l, r]` con `l <= r`. Para pruebas deterministas reproducibles, fijar semilla `rng.seed(1337)`.
- **Complejidad:** $\mathcal{O}(1)$ por llamada a `rd(l, r)`; $\mathcal{O}(N)$ para `shuffle`.

-- rng_mt19937.cpp

- **Trampas y adaptaciones:** (1) Declarar `rng` una sola vez como global (no recrearlo dentro de una función en cada llamada); (2) asegurar `l <= r` antes de llamar a `uniform_int_distribution`.

------------------------------------------------------------------------

### 1.3 `std::string`: Métodos Clave (`find`, `rfind`, `substr`, `erase`, `insert`, `replace`, `stringstream`, `stoll`)

- **Cuándo usar:** Separar líneas por un delimitador fijo (CSV, horas `HH:MM:SS`) o por espacios arbitrarios, buscar subcadenas (`find`), extraer rangos (`substr`) y convertir entre strings y enteros (`stoll`).
- **Cuándo no usar / alternativa:** Si necesitas buscar un patrón millones de veces o sobre textos de $10^6$ caracteres con garantías lineales en el peor caso, no usar `s.find` repetido ($\mathcal{O}(N \cdot M)$); usar KMP (§5.3) o Z-Algorithm (§5.4).
- **Contrato:** `split_delim(s, delim)` y `split_spaces(s)` retornan `vector<string>`. En `s.substr(pos, len)`, el segundo parámetro es la **longitud**, no el índice final.
- **Complejidad:** `split_delim` y `split_spaces` en $\mathcal{O}(|S|)$; `find`, `substr`, `erase`, `insert` en $\mathcal{O}(|S|)$.

-- string_methods.cpp

- **Trampas y adaptaciones:** (1) Al alternar `cin >> x` con `getline`, usar `getline(cin >> ws, s)` si deseas ignorar saltos de línea previos, pero si los espacios iniciales de la línea son significativos consumir solo `cin.ignore()`; (2) comparar el retorno de `find` siempre con `string::npos`.

------------------------------------------------------------------------

### 1.4 Manipulación de Bits (`__builtin`, LSO, Submáscaras) y `std::bitset`

- **Cuándo usar:** Operaciones sobre máscaras de bits ($N \le 60$), conteo de bits encendidos (`__builtin_popcountll`), bit menos/más significativo, iteración de submáscaras en $\mathcal{O}(3^N)$ y conjuntos booleanos acelerados por hardware (`bitset<MAXN>`).
- **Cuándo no usar / alternativa:** Si el tamaño del `bitset` no se conoce en tiempo de compilación y varía mucho por caso, o si solo hay pocos bits encendidos dispersos en $[0, 10^9]$ (usar `vector<int>` ordenado o `safe_uset`).
- **Contrato:** `__builtin_ctzll(x)` y `__builtin_clzll(x)` son **comportamiento indefinido (UB) si `x == 0`**; verificar siempre `x > 0`. Para desplazamientos $\ge 31$ bits usar siempre `1LL << k` o `1ULL << k`.
- **Complejidad:** Operaciones `__builtin_*` en $\mathcal{O}(1)$; recorrer todas las submáscaras de una máscara $m$ toma $\mathcal{O}(2^{\text{popcount}(m)})$; operaciones de `bitset<N>` toman $\mathcal{O}(N/64)$.

-- bitmask_tricks.cpp

- **Trampas y adaptaciones:** (1) Usar siempre los sufijos `ll` (`__builtin_popcountll`, `__builtin_clzll`, `__builtin_ctzll`) para tipos `long long`; sin `ll` truncan a 32 bits; (2) `1 << 35` hace overflow en `int`, escribir `1LL << 35`.

------------------------------------------------------------------------

### 1.5 Cambios de Base (Base $A \leftrightarrow B$, Binario, Base Negativa) y Números Gigantes en String

- **Cuándo usar:** Convertir números entre bases $B \in [2, 36]$ (`decimal_to_base`, `base_to_decimal`), representar en base negativa como $-2$ (`to_negative_base`), o dividir/calcular el módulo de un entero de $10^5$ dígitos leído como `string` (`divide_large_number`, `mod_large_number`).
- **Cuándo no usar / alternativa:** Si se requiere multiplicación repetida de enteros gigantes sin límite de tiempo estricto y Python está permitido, usar BigInt nativo de Python (§12.2–§12.3).
- **Contrato:** `to_binary_string(num, n)` pone el LSB en `s[0]`; `decimal_to_base` pone el MSB en `s[0]`. `divide_large_number(s, n)` y `mod_large_number(s, m)` asumen `s` decimal no negativo y divisor `n, m > 0`.
- **Complejidad:** Cambio de base en $\mathcal{O}(\log_B(\text{num}))$; división y módulo de string en $\mathcal{O}(|S|)$.

-- base_conversion.cpp

- **Trampas y adaptaciones:** (1) `to_binary_string` devuelve el bit menos significativo en el índice 0, mientras que `decimal_to_base(x, 2)` devuelve el más significativo en el índice 0; (2) en C++, `rem % neg_base` puede ser negativo, por lo que `to_negative_base` suma `-neg_base` e incrementa el cociente.
