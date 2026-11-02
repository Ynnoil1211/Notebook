------------------------------------------------------------------------

## Bloque 7: Matemáticas, Teoría de Números, Álgebra y Juegos

### 7.1 Cribas (Eratóstenes, Divisores, Lineal SPF, Segmentada) y Primalidad $\mathcal{O}(\sqrt{N})$

#### 7.1.1 Criba de Eratóstenes (`sieve`)

- **Cuándo usar:** Precalcular primalidad `is_prime[0..MAX_P-1]` en $\mathcal{O}(N \log \log N)$ para consultas en $\mathcal{O}(1)$ hasta $N \approx 10^7$.
- **Cuándo no usar / alternativa:** Si además necesitas factorizar muchos números rápido en $\mathcal{O}(\log X)$, usar `linear_sieve_spf` (§7.1.3).
- **Contrato:** Marca `is_prime[0] = is_prime[1] = false` y desde `i = 2` tacha múltiplos empezando en `i * i`.
- **Complejidad:** $\mathcal{O}(N \log \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- sieve_eratosthenes.cpp

- **Trampas y adaptaciones:** (1) El bucle interno `for (ll j = i * i; ...)` debe usar `ll` si `MAX_P` supera $46340$, pues `i * i` desbordaría `int`.

#### 7.1.2 Criba Armónica de Divisores (`harmonic_sieve`)

- **Cuándo usar:** Precalcular la cantidad de divisores `num_div[x]` ($\tau(x)$) y la suma de divisores `sum_div[x]` ($\sigma(x)$) para todos los enteros $x \in [1, N]$.
- **Cuándo no usar / alternativa:** Si solo necesitas los divisores de un único $N \le 10^{12}$, iterar hasta $\sqrt{N}$ en $\mathcal{O}(\sqrt{N})$.
- **Contrato:** Recorre cada divisor `i` y suma a todos sus múltiplos `j = i, 2i, 3i, ... <= n`. Dependencias: `MAX_P` (§7.1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- harmonic_sieve.cpp

- **Trampas y adaptaciones:** (1) La serie armónica $\sum_{i=1}^N \lfloor N/i \rfloor = \mathcal{O}(N \log N)$ es mucho más rápida que factorizar cada número por separado.

#### 7.1.3 Criba Lineal SPF y Factorización en $\mathcal{O}(\log X)$ (`linear_sieve_spf`, `get_prime_factors`)

- **Cuándo usar:** Factorizar hasta $Q \le 10^6$ números con $X \le 10^7$ en $\mathcal{O}(\log X)$ por consulta dividiendo repetidamente por el menor factor primo `spf[x]`.
- **Cuándo no usar / alternativa:** Si $X \le 10^{18}$ no cabe en memoria para precalcular `spf`, usar `factorize_64` con Pollard's Rho (§7.2.2).
- **Contrato:** Cada compuesto $m$ es tachado **exactamente una vez** por su menor factor primo `spf[m]`. `get_prime_factors(x)` retorna los factores primos en orden no decreciente con multiplicidad.
- **Complejidad:** Precomputación $\mathcal{O}(N)$; factorización de $X$ en $\mathcal{O}(\log X)$.

-- linear_sieve_spf.cpp

- **Lectura de entrada y uso:** Llamar `linear_sieve_spf()` **una sola vez** al inicio de `main()` antes del bucle de consultas:
```cpp
linear_sieve_spf(); // una vez en main()
int q; cin >> q;
while (q--) {
  int x; cin >> x;
  vi pf = get_prime_factors(x); // ej: 12 -> {2, 2, 3}
}
```
- **Trampas y adaptaciones:** (1) No olvidar llamar `linear_sieve_spf(max_val)` una vez al inicio de `main()` antes de invocar `get_prime_factors(x)`.

#### 7.1.4 Criba Segmentada en Rango $[L, R]$ (`segmented_sieve`)

- **Cuándo usar:** Encontrar todos los primos en un intervalo desplazado $[L, R]$ cuando $R \le 10^{12}$ es demasiado grande para un arreglo global pero el ancho $R - L + 1 \le 10^7$ es pequeño.
- **Cuándo no usar / alternativa:** Si solo necesitas verificar unos pocos números aislados en $[L, R]$, usar `miller_rabin` (§7.2.1).
- **Contrato:** Retorna `vector<bool> is_p(R - L + 1)` donde `is_p[x - L]` indica si `x` es primo. Maneja explícitamente los bordes `L = 0` y `L = 1`.
- **Complejidad:** $\mathcal{O}(\sqrt{R} \log \log \sqrt{R} + (R - L + 1) \log \log R)$ tiempo y $\mathcal{O}(\sqrt{R} + R - L + 1)$ memoria.

-- segmented_sieve.cpp

- **Trampas y adaptaciones:** (1) El primer múltiplo de `p` a tachar en $[L, R]$ debe ser `max(p * p, ((L + p - 1) / p) * p)` para no tachar a `p` mismo cuando $L \le p$.

#### 7.1.5 Test de Primalidad Individual en $\mathcal{O}(\sqrt{N})$ (`is_prime_single`)

- **Cuándo usar:** Verificar primalidad de un único entero $N \le 10^{12}$ sin necesidad de tablas ni de `__uint128_t`.
- **Cuándo no usar / alternativa:** Si $N$ llega hasta $10^{18}$ o hay miles de consultas grandes, $\mathcal{O}(\sqrt{N})$ da TLE; usar `miller_rabin` (§7.2.1).
- **Contrato:** Salta múltiplos de $2$ y $3$ e inspecciona divisores de la forma $6k \pm 1$ hasta $\sqrt{N}$.
- **Complejidad:** $\mathcal{O}(\sqrt{N})$ tiempo y $\mathcal{O}(1)$ memoria.

-- is_prime_single.cpp

- **Trampas y adaptaciones:** (1) La condición `i * i <= n` con `ll i` es segura hasta $N \le 3 \cdot 10^{18}$.

------------------------------------------------------------------------

### 7.2 Primalidad y Factorización Grande: Miller-Rabin (64-bit) y Pollard’s Rho [opcional]

#### 7.2.1 Test de Primalidad Determinista Miller-Rabin 64-bit (`miller_rabin`) [opcional]

- **Cuándo usar:** Determinar con 100% de exactitud si un entero de 64 bits ($N < 2^{64} \approx 1.84 \cdot 10^{19}$) es primo.
- **Cuándo no usar / alternativa:** Para $N \le 10^6$ con millones de consultas, precalcular `sieve()` (§7.1.1) es $\mathcal{O}(1)$ por consulta.
- **Contrato:** Usa `u128` (`__uint128_t`) en `mul_mod64` para evitar overflow con signo cuando $N \ge 2^{63}$, y evalúa las 7 bases deterministas de Jim Sinclair.
- **Complejidad:** $\mathcal{O}(7 \log N)$ tiempo y $\mathcal{O}(1)$ memoria.

-- miller_rabin.cpp

- **Trampas y adaptaciones:** (1) Nunca usar `__int128` con signo para módulos $\ge 2^{63}$; usar siempre `u128` (`__uint128_t`).

#### 7.2.2 Factorización de 64-bit con Pollard's Rho (`pollard_rho`, `factorize_64`) [opcional]

- **Cuándo usar:** Obtener todos los factores primos de un entero $N \le 10^{18}$ en aproximadamente $\mathcal{O}(N^{1/4})$ operaciones.
- **Cuándo no usar / alternativa:** Si todos los números a factorizar son $\le 10^7$, `linear_sieve_spf` (§7.1.3) es mucho más rápido.
- **Contrato:** `factorize_64(n, fac)` llena `fac` con todos los factores primos de `n` (con multiplicidad). Si `n <= 1`, no agrega factores. Dependencias: `miller_rabin`, `mul_mod64` (§7.2.1), `rd` (§1.2.1).
- **Complejidad:** $\mathcal{O}(N^{1/4} \log N)$ esperado en tiempo; $\mathcal{O}(\log N)$ memoria.

-- pollard_rho.cpp

- **Trampas y adaptaciones:** (1) `factorize_64` no ordena automáticamente el vector `fac`; llamar `sort(all(fac))` después si necesitas los factores en orden creciente.

------------------------------------------------------------------------

### 7.3 GCD, Diferencia de Cuadrados, Euclides Extendido, Ecuaciones Diofánticas, Ecuación Modular y CRT

#### 7.3.1 GCD de Arreglo tras Sumar $X$ (`array_diff_gcd`)

- **Cuándo usar:** Calcular $\gcd(a_0 + X, a_1 + X, \dots, a_{n-1} + X)$ para múltiples consultas de $X$ en $\mathcal{O}(\log V)$ por consulta tras un preprocesamiento $\mathcal{O}(N \log V)$.
- **Cuándo no usar / alternativa:** Si el arreglo cambia con actualizaciones en rango además de consultar GCD en subarreglos, combinar diferencias adyacentes con un `SegTree` (§4.4) + `Fenwick` (§4.3.1).
- **Contrato:** Por propiedad de Euclides, $\gcd(a_0 + X, \dots, a_{n-1} + X) = \gcd(a_0 + X, |a_1 - a_0|, \dots, |a_{n-1} - a_0|)$.
- **Complejidad:** $\mathcal{O}(N + \log V)$ tiempo y $\mathcal{O}(1)$ memoria.

-- array_diff_gcd.cpp

- **Trampas y adaptaciones:** (1) Usar `abs(a[i] - a[0])` y `abs(a[0] + x)` para garantizar que el GCD retornado sea siempre no negativo.

#### 7.3.2 Representación como Diferencia de Cuadrados $N = a^2 - b^2$ (`solve_diff_of_squares`)

- **Cuándo usar:** Encontrar todos los pares de enteros no negativos $(a, b)$ tales que $a^2 - b^2 = (a - b)(a + b) = N$.
- **Cuándo no usar / alternativa:** Si $N \equiv 2 \pmod 4$, no existe ninguna solución entera porque $(a-b)$ y $(a+b)$ tienen la misma paridad.
- **Contrato:** Factoriza $N = d_1 \cdot d_2$ con $d_1 \le d_2$ y $d_1 \equiv d_2 \pmod 2$, dando $a = (d_1 + d_2)/2$ y $b = (d_2 - d_1)/2$.
- **Complejidad:** $\mathcal{O}(\sqrt{N})$ tiempo y $\mathcal{O}(\tau(N))$ memoria.

-- diff_of_squares.cpp

- **Trampas y adaptaciones:** (1) Si el problema exige enteros **estrictamente positivos** ($b > 0$), descartar los pares donde $d_1 = d_2$ ($N$ cuadrado perfecto).

#### 7.3.3 Algoritmo de Euclides Extendido (`extgcd`)

- **Cuándo usar:** Hallar $g = \gcd(a, b)$ junto con coeficientes de Bézout $(x, y)$ tales que $a x + b y = g$, o calcular inversos modulares para módulos compuestos.
- **Cuándo no usar / alternativa:** Si solo necesitas $\gcd(a, b)$, usar `std::gcd(a, b)` de `<numeric>`.
- **Contrato:** Para $a, b \ge 0$ no ambos nulos, produce $|x| \le b/g$ y $|y| \le a/g$.
- **Complejidad:** $\mathcal{O}(\log(\min(a, b)))$ tiempo y $\mathcal{O}(1)$ memoria.

-- extgcd.cpp

- **Trampas y adaptaciones:** (1) Si `a` o `b` pueden ser negativos, normalizar signos afuera (como hace `find_diophantine` en §7.3.4).

#### 7.3.4 Ecuación Diofántica Lineal $a x + b y = c$ (`find_diophantine`)

- **Cuándo usar:** Encontrar una solución entera particular $(x_0, y_0)$ y el paso de la familia general de soluciones de $a x + b y = c$.
- **Cuándo no usar / alternativa:** Si $a = 0$ y $b = 0$, la función retorna `false` (tratar el caso $0x + 0y = 0$ por separado si puede ocurrir).
- **Contrato:** Existe solución entera si y solo si $g = \gcd(|a|, |b|)$ divide a $c$. La familia general es $x = x_0 + k \cdot (b/g)$, $y = y_0 - k \cdot (a/g)$ para $k \in \mathbb{Z}$. Usa `i128` al escalar por $c/g$ para evitar overflow. Dependencias: `extgcd` (§7.3.3).
- **Complejidad:** $\mathcal{O}(\log(\min(|a|, |b|)))$ tiempo y $\mathcal{O}(1)$ memoria.

-- diophantine.cpp

- **Trampas y adaptaciones:** (1) Para hallar la solución con el menor $x \ge 0$ (cuando $b > 0$),Sea `step = b / g`; hacer `x = (x % step + step) % step` y recalcular `y = (c - a * x) / b` en `i128`.

#### 7.3.5 Teorema Chino del Resto General (`crt_general`) [opcional]

- **Cuándo usar:** Resolver un sistema de congruencias simultáneas $x \equiv r_i \pmod{m_i}$ cuando los módulos $m_i$ **no son necesariamente coprimos** entre sí.
- **Cuándo no usar / alternativa:** Si el MCM acumulado `m1 * (m2 / g)` supera $9 \cdot 10^{18}$, el resultado no cabe en `ll`.
- **Contrato:** Retorna `{x, lcm}` con $0 \le x < \text{lcm}$, o `{-1, -1}` si dos congruencias son incompatibles ($r_1 \not\equiv r_2 \pmod{\gcd(m_1, m_2)}$). Usa `i128` en el producto intermedio para evitar overflow de `ll`. Dependencias: `extgcd` (§7.3.3).
- **Complejidad:** $\mathcal{O}(K \log(\text{LCM}))$ tiempo y $\mathcal{O}(1)$ memoria.

-- crt_general.cpp

- **Lectura de entrada y uso:** Leer $K$ congruencias $x \equiv r_i \pmod{m_i}$ en `r[0..k-1]`, `m[0..k-1]`:
```cpp
int k; cin >> k;
vll r(k), m(k);
forn(i, k) cin >> r[i] >> m[i];
auto [x, lcm] = crt_general(r, m); // x = -1 si incompatible
```
- **Trampas y adaptaciones:** (1) Toda ecuación lineal modular $a x \equiv b \pmod m$ puede reducirse a $x \equiv r' \pmod{m/g}$ con `extgcd` antes de meterla en `crt_general`.

------------------------------------------------------------------------

### 7.4 Aritmética Modular (`mint`), Combinatoria ($nCr$), Teorema de Lucas y Fórmula de Legendre ($n!$)

#### 7.4.1 Aritmética Modular Segura e Inverso Modular (`norm_mod`, `binpow`, `inv_fermat`, `inv_extgcd`)

- **Cuándo usar:** Operaciones básicas $(+, -, \times, a^e, a^{-1}) \pmod m$ inmunes a operandos negativos y a overflow cuando $m \le 3 \cdot 10^{18}$.
- **Cuándo no usar / alternativa:** No usar `inv_fermat(a, m)` si el módulo `m` **no es primo**; para `m` compuesto usar siempre `inv_extgcd(a, m)`.
- **Contrato:** `norm_mod` lleva cualquier `ll` (incluso negativo) a $[0, m-1]$. `inv_extgcd(a, m)` retorna `-1` si $m \le 1$ o $\gcd(a, m) \neq 1$. Dependencias: `extgcd` (§7.3.3).
- **Complejidad:** $\mathcal{O}(1)$ para suma/resta/producto; $\mathcal{O}(\log E)$ para `binpow` e inversos.

-- modular_arith.cpp

- **Trampas y adaptaciones:** (1) En C++, `-4 % 5 == -4`; `add_mod` y `sub_mod` normalizan ambos operandos antes de operar para que nunca retornen residuos negativos.

#### 7.4.2 Combinatoria en $\mathcal{O}(1)$, Producto Recurrente, Lucas y Legendre (`init_fact`, `nCr`, `nCr_iter`, `lucas_nCr`, `legendre_prime_in_fact`)

- **Cuándo usar:** Calcular $\binom{n}{r} \pmod p$ en $\mathcal{O}(1)$ con factoriales recurrentes (`nCr`), en $\mathcal{O}(r)$ exacto mediante productoria $\binom{n}{r} = \prod_{i=1}^r \frac{n-r+i}{i}$ (`nCr_iter`), con dígitos en base $p$ $\binom{n}{r} \equiv \prod_{i \ge 0} \binom{n_i}{r_i} \pmod p$ (`lucas_nCr`), o sumar $v_p(n!) = \sum_{k \ge 1} \lfloor n/p^k \rfloor$ (`legendre_prime_in_fact`).
- **Cuándo no usar / alternativa:** Si el módulo $M$ no es primo y $n \le 2000$, usar la recurrencia aditiva de Pascal $C[n][r] = (C[n-1][r-1] + C[n-1][r]) \bmod M$ sin divisiones.
- **Contrato:** Llamar `init_fact(max_n, mod)` una vez antes de consultar `nCr` o `nPr`. Para `lucas_nCr(n, r, p)` con $p$ primo, llamar antes `init_fact(p - 1, p)`. Dependencias: `inv_fermat` (§7.4.1).
- **Complejidad:** Precomputación $\mathcal{O}(N)$; `nCr` en $\mathcal{O}(1)$; `nCr_iter` en $\mathcal{O}(r)$; `lucas_nCr` y `legendre_prime_in_fact` en $\mathcal{O}(\log_p N)$.

-- combinatorics.cpp

- **Lectura de entrada y uso:** Llamar `init_fact()` una sola vez en `main()` para consultas $\mathcal{O}(1)$, o `nCr_iter(n, r)` cuando $N \le 10^{18}, R \le 10^6$:
```cpp
init_fact(); // una sola vez al inicio de main()
int q; cin >> q;
while (q--) {
  ll n, r; cin >> n >> r;
  cout << nCr(n, r) << "\n"; // o nCr_iter(n, r)
}
```
- **Trampas y adaptaciones:** (1) Por el Teorema de Lucas en base 2, $\binom{n}{r}$ es **impar** si y solo si $(n \text{ AND } r) == r$ ($r$ es submáscara de bits de $n$).

------------------------------------------------------------------------

### 7.5 Funciones Multiplicativas: Totient de Euler ($\phi$) y Función de Möbius ($\mu$)

#### 7.5.1 Totient de Euler Individual $\phi(N)$ en $\mathcal{O}(\sqrt{N})$ (`euler_phi`)

- **Cuándo usar:** Contar cuántos enteros en $[1, N]$ son coprimos con $N$ para un único $N \le 10^{12}$, o aplicar el Teorema de Euler $a^{\phi(m)} \equiv 1 \pmod m$ cuando $\gcd(a, m) = 1$.
- **Cuándo no usar / alternativa:** Si necesitas $\phi(x)$ para todos los $x \in [1, N]$, usar la criba `init_phi_and_mobius` (§7.5.2).
- **Contrato:** Aplica $\phi(n) = n \prod_{p \mid n} (1 - 1/p)$ usando `ans -= ans / d` para no desbordar.
- **Complejidad:** $\mathcal{O}(\sqrt{N})$ tiempo y $\mathcal{O}(1)$ memoria.

-- euler_phi.cpp

- **Trampas y adaptaciones:** (1) Para reducir exponentes gigantes cuando $\gcd(a, m) > 1$ (Exponenciación Generalizada de Euler) y $e \ge \phi(m)$: $a^e \equiv a^{(e \bmod \phi(m)) + \phi(m)} \pmod m$.

#### 7.5.2 Criba de Euler $\phi[1..N]$ y Función de Möbius $\mu[1..N]$ (`init_phi_and_mobius`) [opcional]

- **Cuándo usar:** Problemas de inversión de Möbius y conteo de pares/tuplas con $\gcd(x, y) = k$ usando $\sum_{d \mid g} \mu(d) = [g = 1]$ y $\sum_{d \mid g} \phi(d) = g$.
- **Cuándo no usar / alternativa:** Si solo necesitas $\phi(N)$ para un único número grande, usar `euler_phi(N)` (§7.5.1).
- **Contrato:** Llena `phi_arr[1..n]` y `mu_arr[1..n]` ($n < \text{MAX\_P}$). Dependencias: `MAX_P` (§7.1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- phi_mobius_sieve.cpp

- **Trampas y adaptaciones:** (1) La cantidad de pares ordenados $(x, y) \in [1, N] \times [1, M]$ con $\gcd(x, y) = 1$ es $\sum_{d=1}^{\min(N, M)} \mu(d) \lfloor N/d \rfloor \lfloor M/d \rfloor$.

------------------------------------------------------------------------

### 7.6 Álgebra Lineal y Polinomios: Exponenciación de Matrices, Eliminación Gaussiana (`double`, Modular, XOR Basis), FFT, FWT, Logaritmo Discreto y Fracciones

#### 7.6.1 Exponenciación de Matrices y Fibonacci Rápido (`Matrix`, `mat_pow`, `fib_fast`)

- **Cuándo usar:** Acelerar recurrencias lineales de dimensión fija $D \le 100$ o contar caminos de longitud $K \le 10^{18}$ en un grafo pequeño en $\mathcal{O}(D^3 \log K)$.
- **Cuándo no usar / alternativa:** Si la recurrencia tiene orden $D \approx 2000$ o no la conoces de antemano, usar `BerlekampMassey` (§7.9) en $\mathcal{O}(D^2 \log K)$.
- **Contrato:** Matriz 0-indexed de dimensión `d x d` módulo `MOD`. `fib_fast(n)` retorna `{F_n, F_{n+1}}` mod `MOD` por duplicación rápida sin crear matriz. Dependencias: `MOD` (§7.4.1).
- **Complejidad:** $\mathcal{O}(D^3 \log E)$ para `mat_pow`; $\mathcal{O}(\log N)$ para `fib_fast`.

-- matrix_exp.cpp

- **Lectura de entrada y uso:** Cargar matriz de adyacencia/transición 0-based `Matrix T(n)` y elevar a la $K$:
```cpp
int n, m; ll k; cin >> n >> m >> k;
Matrix T(n);
forn(i, m) {
  int u, v; cin >> u >> v; --u; --v;
  T.m[u][v] = (T.m[u][v] + 1) % MOD;
}
Matrix Tk = mat_pow(T, k); // Tk.m[u][v]: caminos u->v
```
- **Trampas y adaptaciones:** (1) En el triple bucle de `operator*`, poner `k` como segundo bucle (`for i ... for k ... if (a[i][k]) for j`) mejora drásticamente la localidad de caché.

#### 7.6.2 Base Lineal de XOR / Eliminación Gaussiana en $\mathbb{F}_2$ (`XorBasis`) [opcional]

- **Cuándo usar:** Mantener un conjunto de enteros de hasta 62 bits bajo la operación XOR: consultar el máximo XOR obtenible (`max_xor`), si un valor `x` puede formarse, o cuántos subconjuntos tienen XOR igual a `x` ($2^{N - \text{sz}}$).
- **Cuándo no usar / alternativa:** Si buscas el máximo XOR entre `x` y **un único** elemento del conjunto (no un subconjunto arbitrario), usar `BitTrie` (§5.5.2).
- **Contrato:** Soporta enteros de hasta 62 bits (`B = 62`). El número de valores distintos generables es $2^{\text{sz}}$.
- **Complejidad:** $\mathcal{O}(B)$ por inserción y consulta ($B = 62$).

-- xor_basis.cpp

- **Trampas y adaptaciones:** (1) El menor XOR no nulo obtenible es simplemente el menor `b[i] > 0` de la base reducida.

#### 7.6.3 Eliminación Gaussiana en Punto Flotante (`gauss_double`) [opcional]

- **Cuándo usar:** Resolver sistemas de $N$ ecuaciones lineales con $M$ incógnitas en reales ($\text{ld}$), como sistemas de esperanzas o cadenas de Markov con ciclos.
- **Cuándo no usar / alternativa:** Si el sistema es módulo un primo $P$, usar `gauss_mod` (§7.6.4).
- **Contrato:** Recibe la matriz aumentada `a` de tamaño $N \times (M + 1)$ (donde la columna `M` es el término independiente $b_i$). Usa pivoteo parcial por mayor valor absoluto. Retorna `0` (sin solución), `1` (única en `ans`), o `2` (infinitas soluciones, dejando una particular en `ans`). Dependencias: `EPS` (§1.1).
- **Complejidad:** $\mathcal{O}(N \cdot M \cdot \min(N, M))$ tiempo y $\mathcal{O}(N M)$ memoria.

-- gauss_double.cpp

- **Lectura de entrada y uso:** Leer matriz aumentada `a[0..n-1][0..m]` (la columna `m` es el lado derecho $b_i$):
```cpp
int n, m; cin >> n >> m;
vector<vector<double>> a(n, vector<double>(m + 1));
forn(i, n) forn(j, m + 1) cin >> a[i][j];
vector<double> ans;
int st = gauss_double(a, ans); // 0:sin sol, 1:unica, 2:inf
```
- **Trampas y adaptaciones:** (1) El pivoteo parcial (`abs(a[i][col]) > abs(a[sel][col])`) es obligatorio; elegir el primer no nulo causa catástrofe numérica.

#### 7.6.4 Eliminación Gaussiana Modular (`gauss_mod`) [opcional]

- **Cuándo usar:** Resolver sistemas lineales $A x \equiv b \pmod p$ sobre un cuerpo primo $p$ (ej. sistemas de ecuaciones modulares o conteo de soluciones libres $p^{\text{libres}}$).
- **Cuándo no usar / alternativa:** Si el sistema es sobre $\mathbb{F}_2$ ($p = 2$) con $M \le 2000$, usar `std::bitset` reduce la complejidad a $\mathcal{O}(N M^2 / 64)$.
- **Contrato:** Recibe matriz aumentada `a` de tamaño $N \times (M + 1)$ y primo `mod`. Retorna `0` (incompatible), `1` (única), o `2` (infinitas, con una solución particular en `ans`). Dependencias: `norm_mod`, `sub_mod`, `mul_mod`, `inv_fermat` (§7.4.1).
- **Complejidad:** $\mathcal{O}(N \cdot M \cdot \min(N, M))$ tiempo y $\mathcal{O}(N M)$ memoria.

-- gauss_mod.cpp

- **Trampas y adaptaciones:** (1) El número de variables libres es $M - \text{rango}$, por lo que cuando retorna `2` existen exactamente $\text{mod}^{M - \text{rango}}$ soluciones distintas.

#### 7.6.5 Transformada Rápida de Fourier para Polinomios (`fft`, `multiply_poly`) [opcional]

- **Cuándo usar:** Multiplicar dos polinomios con coeficientes enteros (convolución $c_k = \sum_{i+j=k} a_i b_j$) en $\mathcal{O}(N \log N)$ cuando los coeficientes resultantes no exceden $\approx 10^{14}$.
- **Cuándo no usar / alternativa:** Si la convolución se pide módulo `998244353` o los coeficientes son más grandes y no toleran error de punto flotante, usar `multiply_ntt` (§7.6.6).
- **Contrato:** Redondea cada coeficiente final con `llround(fa[i].real())`. Dependencias: `PI` (§1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- fft.cpp

- **Trampas y adaptaciones:** (1) Para hacer *string matching con comodines* o contar sumas $a_i + b_j = S$, invertir el segundo arreglo (`reverse(all(b))`) para convertir diferencias $i - j$ en sumas $i + (M - 1 - j)$.

#### 7.6.6 Transformada Numérica Modular Exacta (`ntt`, `multiply_ntt`) [opcional]

- **Cuándo usar:** Multiplicar polinomios exactamente módulo $998244353 = 119 \cdot 2^{23} + 1$ (raíz primitiva $g = 3$) sin punto flotante ni error de redondeo.
- **Cuándo no usar / alternativa:** Si el módulo del problema no es de la forma $c \cdot 2^k + 1$ (ej. $10^9 + 7$) y los coeficientes finales caben en $10^{14}$, usar `multiply_poly` (§7.6.5) y tomar módulo al final.
- **Contrato:** Requiere longitud `n` potencia de 2 con $n \le 2^{23}$ (`multiply_ntt` ajusta la potencia de 2 automáticamente).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- ntt.cpp

- **Trampas y adaptaciones:** (1) Otros primos NTT frecuentes: $p = 7340033 = 7 \cdot 2^{20} + 1$ ($g = 3$) y $p = 469762049 = 7 \cdot 2^{26} + 1$ ($g = 3$).

#### 7.6.7 Transformada Rápida de Walsh-Hadamard para Convolución XOR (`fwt_xor`) [opcional]

- **Cuándo usar:** Calcular el arreglo $C[k] = \sum_{i \oplus j = k} A[i] B[j]$ en $\mathcal{O}(N \log N)$ (donde $N = 2^B$ es potencia de 2).
- **Cuándo no usar / alternativa:** Para convolución de suma de índices ($i + j = k$), usar `multiply_poly` o `multiply_ntt` (§7.6.5–§7.6.6).
- **Contrato:** El tamaño de `a` **debe ser potencia de 2**. Para convolucionar: `fwt_xor(a, false)`, `fwt_xor(b, false)`, multiplicar punto a punto `a[i] *= b[i]`, y llamar `fwt_xor(a, true)`.
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(1)$ memoria extra.

-- fwt_xor.cpp

- **Trampas y adaptaciones:** (1) Si se trabaja módulo `MOD`, en la inversa (`inv == true`) multiplicar por el inverso modular de $2$ en vez de dividir por `2`.

#### 7.6.8 Logaritmo Discreto Baby-Step Giant-Step (`discrete_log`) [opcional]

- **Cuándo usar:** Hallar el menor exponente entero $x \ge 0$ tal que $a^x \equiv b \pmod m$ (incluso cuando $\gcd(a, m) > 1$).
- **Cuándo no usar / alternativa:** Si la incógnita está en la base ($x^k \equiv b \pmod p$), reducir primero a logaritmo discreto tomando una raíz primitiva $g$ del primo $p$.
- **Contrato:** Extrae factores comunes $\gcd(a, m)$ antes de aplicar Baby-Step Giant-Step. Retorna el menor $x \ge 0$, o `-1` si no existe solución.
- **Complejidad:** $\mathcal{O}(\sqrt{m} \log \sqrt{m})$ (o $\mathcal{O}(\sqrt{m})$ con `safe_umap`) tiempo y $\mathcal{O}(\sqrt{m})$ memoria.

-- discrete_log.cpp

- **Trampas y adaptaciones:** (1) Definimos $0^0 \equiv 1 \pmod m$, por lo que si `b % m == 1 % m` la respuesta siempre es `0`.

#### 7.6.9 Estructura de Fracciones Racionales Exactas (`Frac`)

- **Cuándo usar:** Operar y comparar números racionales $p/q$ de forma exacta sin errores de redondeo de punto flotante (pendientes de rectas, probabilidades racionales pequeñas, etc.).
- **Cuándo no usar / alternativa:** Si haces decenas de sumas encadenadas con denominadores distintos, `p` y `q` crecerán exponencialmente y desbordarán `ll`; en ese caso usar Python `Fraction` (§12.4) o aritmética modular.
- **Contrato:** Mantiene siempre `q > 0` y $\gcd(|p|, q) = 1$. El comparador `<` multiplica en `i128` para soportar numeradores y denominadores de hasta $10^{18}$ sin overflow.
- **Complejidad:** $\mathcal{O}(\log(\min(|p|, q)))$ por operación aritmética; $\mathcal{O}(1)$ por comparación.

-- frac.cpp

- **Trampas y adaptaciones:** (1) Para representar pendientes dirigidas o no dirigidas que pueden ser verticales ($q = 0$), normalizar con `g = gcd(abs(dx), abs(dy))` sin llamar a `Frac(p, 0)` (que divide por `g` pero asume $q \neq 0$ en sumas).

------------------------------------------------------------------------

### 7.7 XOR en Rango $\mathcal{O}(1)$, Función `mex`, Grundy y Juego de Nim / Misère Nim

#### 7.7.1 XOR Acumulado en Rango $[L, R]$ en $\mathcal{O}(1)$ (`xor_n`, `xor_range`)

- **Cuándo usar:** Calcular $L \oplus (L + 1) \oplus \dots \oplus R$ en tiempo constante para $L, R \le 10^{18}$.
- **Cuándo no usar / alternativa:** Si los números no son consecutivos sino un arreglo arbitrario, usar un arreglo de prefijos XOR.
- **Contrato:** Basado en el ciclo de longitud 4 de $0 \oplus 1 \oplus \dots \oplus n$. Para $l > r$ o $n < 0$ retorna `0`.
- **Complejidad:** $\mathcal{O}(1)$ tiempo y memoria.

-- xor_range.cpp

- **Trampas y adaptaciones:** (1) Nunca escribir `n % 4 == 2 ? n + 1` pensando en OR bit a bit sin paréntesis por precedencia de operadores.

#### 7.7.2 Mínimo Excluido No Negativo (`mex`) para Grundy / Sprague-Grundy [opcional]

- **Cuándo usar:** Calcular en $\mathcal{O}(N)$ el menor entero $\ge 0$ que no aparece en el vector `v` (base de los valores Grundy $G(u) = \text{mex}(\{G(v) : u \to v\})$ en juegos imparciales).
- **Cuándo no usar / alternativa:** Si necesitas mantener el MEX de un conjunto que sufre inserciones y borrados online, mantener los ausentes en un `std::set<int>` o usar un Segment Tree de mínimos sobre la última posición vista.
- **Contrato:** Por principio del palomar, el MEX de $N$ números está siempre en $[0, N]$, por lo que ignora valores $< 0$ o $> N$.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y memoria.

-- mex.cpp

- **Trampas y adaptaciones:** (1) En la suma disjunta de $K$ juegos imparciales independientes, el valor Grundy total es $G(s_1) \oplus G(s_2) \oplus \dots \oplus G(s_K)$.

#### 7.7.3 Juego de Nim Normal y Misère Nim (`nim_normal_win`, `nim_misere_win`)

- **Cuándo usar:** Determinar si el primer jugador tiene estrategia ganadora en Nim normal (gana quien retira la última piedra) o en **Misère Nim** (pierde quien se ve forzado a retirar la última piedra).
- **Cuándo no usar / alternativa:** En juegos imparciales generales bajo regla Misère donde un movimiento desde un montón de Grundy $> 1$ no puede elegir arbitrariamente cualquier Grundy menor, la reducción de Misère Nim no aplica directamente.
- **Contrato:** Ignora correctamente montones vacíos (`p == 0`). En Misère Nim, si todos los montones tienen tamaño $\le 1$, el primer jugador gana si y solo si la cantidad de montones de tamaño `1` es **par** (`ones % 2 == 0`); si existe algún montón $> 1$, gana si y solo si el XOR total es $\neq 0$.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y $\mathcal{O}(1)$ memoria.

-- nim.cpp

- **Trampas y adaptaciones:** (1) No usar `*max_element <= 1 && sz(piles) % 2 == 0` porque los montones de tamaño `0` alterarían `sz(piles)` sin ser jugadas reales; contar explícitamente `if (p == 1) ones++`.

------------------------------------------------------------------------

### 7.8 Programación Lineal: Algoritmo Simplex de Dos Fases [opcional]

- **Cuándo usar:** Maximizar una función objetivo lineal $c^T x$ sujeta a $A x \le b, x \ge 0$ con variables continuas (o con matriz totalmente unimodular) cuando $N, M \le 100$.
- **Cuándo no usar / alternativa:** Si el problema es de flujo o corte en un grafo grande, usar `Dinic` (§6.6.1) o `MCMF` (§6.6.2) en lugar de programación lineal general.
- **Contrato:** Fase 1 halla una solución básica factible inicial incluso cuando algún $b_i < 0$ (retornando `-INF_LP` si el poliedro es vacío/infactible); Fase 2 optimiza $c^T x$ con regla de desempate lexicográfica anti-ciclos de Bland (retornando `+INF_LP` si es no acotado). Soporta valores óptimos negativos cuando el origen no es factible.
- **Complejidad:** Polinomial en promedio práctico; exponencial solo en casos patológicos artificiales.

-- simplex.cpp

- **Lectura de entrada y uso:** Crear `Simplex lp(n, m)` ($n$ restricciones $Ax \le b$, $m$ variables $x \ge 0$), llenar `lp.a`, `lp.b`, `lp.c` (0-based) y llamar `lp.solve()`:
```cpp
Simplex lp(n, m);
forn(j, m) cin >> lp.c[j];
forn(i, n) {
  forn(j, m) cin >> lp.a[i][j];
  cin >> lp.b[i];
}
auto [best, x] = lp.solve(); // -INF_LP infact, +INF_LP ilim
```
- **Trampas y adaptaciones:** (1) Una restricción $\ge$ ($a^T x \ge b$) se convierte multiplicando por $-1$: $(-a)^T x \le -b$; una igualdad $a^T x = b$ se representa con ambas desigualdades $\le$ y $\ge$.

### 7.9 Berlekamp-Massey y Recurrencias Lineales Modulares [opcional]

- **Cuándo usar:** Descubrir automáticamente la recurrencia lineal más corta que genera una sucesión modular dada (a partir de sus primeros $2D$ términos) y evaluar su término $K$-ésimo ($K \le 10^{18}$) en $\mathcal{O}(D^2 \log K)$.
- **Cuándo no usar / alternativa:** Si la sucesión no proviene de una recurrencia lineal de orden pequeño sino de un polinomio en $k$ de grado $D$, también funciona (orden $D + 1$), pero la interpolación de Lagrange (§7.10.1) la evalúa en $\mathcal{O}(D)$.
- **Contrato:** Índices **0-based** (`get_kth(s, 0)` retorna `s[0]`). Normaliza discrepancias negativas con `norm()` y soporta módulos primos. Dependencias: `MOD` (§7.4.1).
- **Complejidad:** Inferencia de recurrencia $\mathcal{O}(N^2)$; evaluación de `get_kth` en $\mathcal{O}(D^2 \log K)$.

-- berlekamp_massey.cpp

- **Trampas y adaptaciones:** (1) Proveer siempre al menos $2D + 2$ términos iniciales calculados por fuerza bruta o DP pequeña para que Berlekamp-Massey no infiera una recurrencia espuria más corta.

### 7.10 Interpolación de Lagrange en $\mathcal{O}(D)$ y $\mathcal{O}(D^2)$ [opcional]

#### 7.10.1 Interpolación de Lagrange en $\mathcal{O}(D)$ para Abscisas Consecutivas $0..D$ (`lagrange_eval_consecutive`) [opcional]

- **Cuándo usar:** Evaluar en $\mathcal{O}(D)$ un polinomio $P(x) \pmod{\text{MOD}}$ de grado $D$ (por ejemplo $\sum_{i=1}^n i^k$, que tiene grado $D = k + 1$) conociendo únicamente sus valores en $x = 0, 1, \dots, D$.
- **Cuándo no usar / alternativa:** Si las abscisas $x_i$ donde conoces $P(x_i)$ **no** son $0, 1, \dots, D$, usar `lagrange_eval_arbitrary` (§7.10.2).
- **Contrato:** `y` tiene tamaño $D + 1$ con `y[i] = P(i) mod MOD`. Requiere haber llamado `init_fact(D, MOD)` previamente. Dependencias: `init_fact`, `norm_mod`, `add_mod`, `sub_mod`, `mul_mod` (§7.4).
- **Complejidad:** $\mathcal{O}(D)$ tiempo y memoria por evaluación.

-- lagrange.cpp

- **Trampas y adaptaciones:** (1) La suma de potencias $\sum_{i=1}^n i^k$ es un polinomio de grado $D = k + 1$ (no $k$), por lo que hay que pasar $k + 2$ puntos $P(0), \dots, P(k + 1)$.

#### 7.10.2 Interpolación de Lagrange en $\mathcal{O}(N^2)$ para Puntos Arbitrarios (`lagrange_eval_arbitrary`) [opcional]

- **Cuándo usar:** Evaluar $P(x) \pmod{\text{MOD}}$ cuando los $N$ puntos conocidos $(x_i, y_i)$ tienen abscisas arbitrarias distintas módulo `MOD`.
- **Cuándo no usar / alternativa:** Si las abscisas son consecutivas $0..D$, `lagrange_eval_consecutive` (§7.10.1) es $\mathcal{O}(D)$ en lugar de $\mathcal{O}(N^2)$.
- **Contrato:** Requiere que todos los `xs[i]` sean distintos módulo `MOD`. Dependencias: `norm_mod`, `add_mod`, `sub_mod`, `mul_mod`, `inv_fermat` (§7.4.1).
- **Complejidad:** $\mathcal{O}(N^2)$ tiempo y $\mathcal{O}(1)$ memoria extra.

-- lagrange_arbitrary.cpp

- **Trampas y adaptaciones:** (1) Si necesitas recuperar explícitamente el vector de coeficientes $c_0 + c_1 x + \dots + c_{N-1} x^{N-1}$, precalcular el polinomio producto $\prod (x - x_i)$ en $\mathcal{O}(N^2)$ y dividir sintéticamente por $(x - x_i)$ para cada término.

------------------------------------------------------------------------

### 7.11 Conteo Avanzado y Esperanza: Stars and Bars, Inclusión-Exclusión, Contribuciones y Linealidad

#### 7.11.1 Stars and Bars: Composiciones No Negativas y Positivas (`stars_and_bars_nonneg`, `stars_and_bars_pos`)

- **Cuándo usar:** Contar cuántas tuplas de $k$ enteros suman exactamente $n$: con $x_i \ge 0$ es $\binom{n + k - 1}{k - 1}$, y con $x_i \ge 1$ es $\binom{n - 1}{k - 1}$.
- **Cuándo no usar / alternativa:** Si además cada variable tiene una cota superior $x_i \le c$, combinar Stars and Bars con **Inclusión-Exclusión** sobre el subconjunto de variables que violan $x_i \ge c + 1$.
- **Contrato:** Requiere haber llamado `init_fact(n + k, MOD)` previamente. Dependencias: `init_fact`, `nCr` (§7.4.2).
- **Complejidad:** $\mathcal{O}(1)$ por consulta tras `init_fact`.

-- stars_and_bars.cpp

- **Trampas y adaptaciones:** (1) Si cada variable exige $x_i \ge a_i$, sustituir $x'_i = x_i - a_i \ge 0$ y llamar `stars_and_bars_nonneg(n - sum(a_i), k)`.

#### 7.11.2 Principio de Inclusión-Exclusión para Múltiplos (`inclusion_exclusion_multiples`)

- **Cuándo usar:** Contar cuántos enteros en $[1, N]$ ($N \le 10^{18}$) son divisibles por **al menos uno** de los números de una lista pequeña $D$ ($|D| \le 20$).
- **Cuándo no usar / alternativa:** Si $|D| > 25$, iterar las $2^{|D|}$ máscaras da TLE.
- **Contrato:** Usa `i128` al actualizar el MCM (`lcm = lcm / g * divs[i]`) y corta la rama si `lcm > n` para evitar overflow.
- **Complejidad:** $\mathcal{O}(2^K \cdot K \log N)$ tiempo y $\mathcal{O}(1)$ memoria.

-- inclusion_exclusion_derangements.cpp

- **Trampas y adaptaciones:** (1) Para contar cuántos enteros en $[1, N]$ **no** son divisibles por ninguno de los divisores, calcular `n - inclusion_exclusion_multiples(n, divs)`.

#### 7.11.3 Técnica de Contribuciones con Pila Monótona (`sum_all_subarray_mins`) [opcional]

- **Cuándo usar:** Calcular la suma de $\min(a[l..r])$ (o $\max(a[l..r])$) sobre todos los $\binom{N+1}{2}$ subarreglos en $\mathcal{O}(N)$ contando en cuántos subarreglos cada $a[i]$ actúa como mínimo.
- **Cuándo no usar / alternativa:** Si la consulta pide la suma de mínimos solo dentro de un subrango $[L_q, R_q]$ por query, precalcular saltos en la pila monótona o usar Mo / Segment Tree.
- **Contrato:** Para no contar dos veces subarreglos con mínimos empatados, busca el anterior **estrictamente menor** (`<`) a la izquierda y el siguiente **menor o igual** (`<=`) a la derecha.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y memoria.

-- subarray_min_max_contribution.cpp

- **Trampas y adaptaciones:** (1) Usar `<` de ambos lados cuenta de más cuando hay elementos iguales; usar `<=` de ambos lados cuenta de menos. Siempre emparejar `<` a un lado con `<=` al otro.

#### 7.11.4 Linealidad de la Esperanza: Coupon Collector (`expected_coupon_collector`) [opcional]

- **Cuándo usar:** Calcular el número esperado de extracciones uniformes con reemplazo necesarias para obtener los $N$ tipos distintos de cupones ($E[T] = \sum_{k=0}^{N-1} \frac{N}{N - k} = N H_N$).
- **Cuándo no usar / alternativa:** Si los cupones tienen probabilidades **no uniformes** $p_i$, usar Inclusión-Exclusión Min-Max: $E[\max T_i] = \sum_{\emptyset \neq S \subseteq [N]} (-1)^{|S|-1} \frac{1}{\sum_{i \in S} p_i}$.
- **Contrato:** Por linealidad de la esperanza, el tiempo para pasar de $k$ cupones distintos a $k + 1$ sigue una distribución geométrica de éxito $p = (N - k)/N$, cuyo valor esperado es $1/p = N/(N - k)$.
- **Complejidad:** $\mathcal{O}(N)$ tiempo y $\mathcal{O}(1)$ memoria.

-- expected_value_linearity.cpp

- **Trampas y adaptaciones:** (1) Si el problema pide $N H_N \pmod{998244353}$, reemplazar `1.0L / rem` por `inv_num[rem]` (§7.4.2).
