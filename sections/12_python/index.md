------------------------------------------------------------------------

## Bloque 12: Python 3 para ICPC — Trucos, BigInt, Bases y Librería Estándar

> **¿Cuándo usar Python en competencia?**:
> - Cuando el problema requiere **BigInt** (miles de dígitos sin límite de 64 o 128 bits).
> - Cuando toque convertir o parsear **bases arbitrarias** o formatear binario/hexadecimal.
> - Cuando requieras **fechas, días transcurridos y calendarios** exactos (`datetime`).
> - Cuando se necesiten **fracciones exactas** (`Fraction`) o **100+ decimales** (`Decimal`).
> - Cuando parsear strings en C++ tome decenas de líneas y en Python salga con `re` o `eval`.
> - Cuando una DP con estados complejos (strings o tuplas) se resuelva en 5 líneas con `@cache`.

### 12.1 Template Base, Fast I/O, Límites de Recursión y Digits en BigInt

- **Cuándo usar:** Plantilla inicial en Python 3 para lectura masiva de tokens (`sys.stdin.read().split()`), recursión profunda (`setrecursionlimit`) y conversión de enteros de más de 4300 dígitos a texto (`set_int_max_str_digits`).
- **Cuándo no usar / alternativa:** En problemas interactivos donde debes leer línea por línea y hacer `flush`, no usar `sys.stdin.read()` (que espera hasta `EOF`); usar `sys.stdin.readline()` y `print(..., flush=True)`.
- **Contrato:** En Python 3.11+, imprimir un entero de más de 4300 dígitos lanza `ValueError` si no se llama antes a `sys.set_int_max_str_digits(10**6)`. Dependencias: librería estándar de Python 3.
- **Complejidad:** $\mathcal{O}(\text{tokens})$ para lectura completa de entrada.

-- template_fast_io.py

- **Trampas y adaptaciones:** (1) En PyPy3 / CPython, una recursión profunda con `@cache` o DFS que supere $10^5$ niveles puede agotar la pila del sistema operativo aunque `setrecursionlimit` sea alto; preferir BFS/iterativo si $N > 10^5$.

### 12.2 Conversión de Bases Arbitrarias (Base $A \leftrightarrow B$) y Formateo

- **Cuándo usar:** Convertir números (incluso gigantes) entre cualquier par de bases $A, B \in [2, 62]$ o formatear en binario/octal/hexadecimal con relleno de ceros.
- **Contrato:** `int(s, base)` soporta nativamente bases $2 \le \text{base} \le 36$; `to_base(n, b)` soporta bases hasta $62$ (`0-9A-Za-z`) y maneja `n = 0` y negativos.
- **Complejidad:** $\mathcal{O}(L^2)$ para conversión de cadenas de longitud $L$ en BigInt.

-- base_conversion.py

- **Trampas y adaptaciones:** (1) `bin(x)` devuelve el prefijo `'0b'` (y `'-0b'` si es negativo); usar `f'{x:b}'` evita tener que cortar `bin(x)[2:]`.

### 12.3 Aritmética Gigante (BigInt), Potencia Modular y Raíz Entera

- **Cuándo usar:** Operar con enteros de miles de dígitos sin overflow, calcular $b^e \pmod m$ en $\mathcal{O}(\log e)$ con `pow(b, e, mod)`, truncar divisiones hacia cero (`trunc_div`) y extraer $\lfloor\sqrt{N}\rfloor$ exacta con `math.isqrt(N)`.
- **Cuándo no usar / alternativa:** Nunca escribir `(b ** e) % mod` (que construye el número gigante $b^e$ en memoria y da TLE/MLE); usar siempre el tercer argumento `pow(b, e, mod)`.
- **Contrato:** En Python, `-7 // 3 == -3` (redondea hacia $-\infty$); `trunc_div(a, b)` trunca hacia cero igual que C++ usando solo aritmética entera (sin `int(a / b)`, que falla para $> 10^{16}$ por precisión de `float`). En Python 3.8+, `pow(a, -1, m)` calcula directamente el inverso modular. Dependencias: `from math import isqrt`.
- **Complejidad:** `pow(b, e, m)` en $\mathcal{O}(\log e)$; `isqrt(N)` en $\mathcal{O}(M(\text{bits}))$.

-- bigint_pow_isqrt.py

- **Trampas y adaptaciones:** (1) Nunca usar `int(math.sqrt(N))` para $N > 10^{15}$; usar siempre `math.isqrt(N)`.

### 12.4 Fracciones Exactas (`Fraction`) y Decimales de Alta Precisión (`Decimal`)

- **Cuándo usar:** Geometría, probabilidades o sistemas lineales pequeños donde el punto flotante de 64/80 bits da WA por precisión, o cálculo de raíces/divisiones con 100+ cifras decimales exactas.
- **Cuándo no usar / alternativa:** Si el denominador crece exponencialmente en miles de pasos, `Fraction` se vuelve lento por el cálculo de GCD en cada suma.
- **Contrato:** Construir siempre `Fraction(num, den)` o `Fraction('0.25')` (desde `str`, nunca desde `float` binario inexacto). En `Decimal`, ajustar `getcontext().prec = 100` antes de operar. Dependencias: `fractions`, `decimal`.
- **Complejidad:** $\mathcal{O}(\log(\text{den}))$ por operación racional.

-- fraction_decimal.py

- **Trampas y adaptaciones:** (1) `Decimal(0.1)` hereda la imprecisión binaria de `float`; pasar siempre strings `Decimal('0.1')` o enteros `Decimal(1) / Decimal(10)`.

### 12.5 Manejo de Fechas y Calendarios (`datetime`)

- **Cuándo usar:** Contar días exactos entre dos fechas, avanzar/retroceder $K$ días o determinar el día de la semana respetando años bisiestos automáticamente.
- **Cuándo no usar / alternativa:** Si los años superan `9999` (`datetime.MAXYEAR = 9999`), reducir el año módulo `400` (el calendario gregoriano se repite exactamente cada $400$ años = $146097$ días).
- **Contrato:** `weekday()` retorna `0` para Lunes hasta `6` para Domingo. Dependencias: `import datetime`.
- **Complejidad:** $\mathcal{O}(1)$ tiempo y memoria.

-- datetime_calendar.py

- **Trampas y adaptaciones:** (1) El ciclo gregoriano de 400 años tiene exactamente $400 \times 365 + 97 = 146097$ días, que es divisible por $7$ (por lo que los días de la semana también coinciden cada 400 años).

### 12.6 DP con Memoización Automática (`@cache` / `cache_clear`)

- **Cuándo usar:** Prototipar rápidamente DP top-down sobre estados dispersos (Digit DP, juegos, árboles) sin dimensionar arreglos multidimensionales a mano.
- **Cuándo no usar / alternativa:** Si el estado es denso y hay millones de transiciones simples, un bucle iterativo en C++ (Bloque 8) es $20\times$–$50\times$ más rápido.
- **Contrato:** Todos los argumentos de la función decorada con `@cache` deben ser *hashables* (`int`, `str`, `tuple`; nunca `list`). Llamar `dp.cache_clear()` al final de cada caso de prueba en problemas multi-test. Dependencias: `from functools import cache`.
- **Complejidad:** Tiempo y memoria proporcionales al número de estados alcanzables.

-- dp_cache.py

- **Trampas y adaptaciones:** (1) Si olvidas `dp.cache_clear()` entre casos de prueba que leen arreglos globales `weight`/`val`, Python reutilizará resultados del caso anterior y dará WA.

### 12.7 Extracción de Números en Texto Sucio con Expresiones Regulares (`re`)

- **Cuándo usar:** Parsear entradas con formato sucio (coordenadas entre paréntesis, comas, signos, etiquetas de texto) extrayendo todos los enteros en una sola línea.
- **Cuándo no usar / alternativa:** Si la entrada solo contiene enteros separados por espacios, `map(int, sys.stdin.read().split())` es más rápido.
- **Contrato:** `re.findall(r'-?\d+', s)` extrae todos los enteros con o sin signo `-`. Dependencias: `import re`.
- **Complejidad:** $\mathcal{O}(|S|)$ tiempo y memoria.

-- regex_integers.py

- **Trampas y adaptaciones:** (1) Si `'2026-11-01'` debe leerse como tres números positivos separados por guiones y no como `2026, -11, -1`, usar `r'\d+'` sin el `-?`.

### 12.8 Frecuencias y Álgebra de Multiconjuntos (`Counter`)

- **Cuándo usar:** Contar frecuencias de elementos o caracteres en $\mathcal{O}(N)$, hallar los $K$ más frecuentes (`most_common(K)`) u operar multiconjuntos con suma `+`, resta `-`, intersección `&` (mínimo) y unión `|` (máximo).
- **Cuándo no usar / alternativa:** Si un adversario construye colisiones contra el hash de enteros en Codeforces/AtCoder con `Counter(int)`, envolver las llaves con `x ^ RANDOM_SEED` o convertirlas a `str`.
- **Contrato:** La resta `c1 - c2` elimina automáticamente las llaves cuya frecuencia quede $\le 0$; para conservar negativos usar `c1.subtract(c2)`. Dependencias: `from collections import Counter`.
- **Complejidad:** $\mathcal{O}(N)$ esperado.

-- counter_multiset.py

- **Trampas y adaptaciones:** (1) `sum(c.values())` (o `c.total()` en Python 3.10+) devuelve el total de elementos contando multiplicidades.

### 12.9 Agrupación y Grafos Instantáneos (`defaultdict`)

- **Cuándo usar:** Agrupar elementos por clave (ej. anagramas por firma ordenada `''.join(sorted(w))`) o representar grafos dispersos con etiquetas arbitrarias (strings, tuplas) sin inicializar llaves previamente.
- **Cuándo no usar / alternativa:** Si los nodos son enteros `0..N-1`, una lista de listas `[[] for _ in range(N)]` es más rápida y consume menos memoria.
- **Contrato:** Al acceder a `d[k]` por primera vez, invoca la fábrica (`list`, `int`, `set`) y crea la entrada automáticamente. Dependencias: `from collections import defaultdict`.
- **Complejidad:** $\mathcal{O}(1)$ promedio por acceso/inserción.

-- defaultdict_grouping.py

- **Trampas y adaptaciones:** (1) Iterar `for k in d:` mientras lees `d[otra_llave]` dentro del bucle lanza `RuntimeError: dictionary changed size during iteration`; usar `d.get(otra_llave)` si no quieres crear llaves vacías.

### 12.10 Rotación y Transposición de Matrices en 1 Línea (`zip(*mat)`)

- **Cuándo usar:** Transponer una grilla $R \times C$ (convertir columnas en filas) o rotarla $90^\circ$ en sentido horario o antihorario en una sola línea.
- **Contrato:** `zip(*mat)` empareja los elementos de igual columna de cada fila. `col[::-1]` invierte cada columna para rotar $90^\circ$ en sentido horario.
- **Complejidad:** $\mathcal{O}(R \cdot C)$ tiempo y memoria.

-- matrix_transpose_rotate.py

- **Trampas y adaptaciones:** (1) Si las filas son strings (`['abc', 'def']`), usar `[''.join(col) for col in zip(*mat)]` para obtener una lista de strings.

### 12.11 Álgebra de Conjuntos Nativos (`set`)

- **Cuándo usar:** Operaciones de intersección (`A & B`), unión (`A | B`), diferencia (`A - B`), diferencia simétrica (`A ^ B`) y prueba de subconjunto (`A <= B`).
- **Cuándo no usar / alternativa:** Si necesitas mantener los elementos **ordenados** con búsqueda binaria (`lower_bound`), el `set` de Python es una tabla hash desordenada; usar C++ `std::set` o `ordered_set` (§1.3).
- **Contrato:** `set()` crea un conjunto vacío (`{}` crea un diccionario vacío).
- **Complejidad:** $\mathcal{O}(|A| + |B|)$ para operaciones binarias entre conjuntos.

-- set_operations.py

- **Trampas y adaptaciones:** (1) Un `set` no puede contener elementos mutables (`list`, `set`); usar `tuple` o `frozenset` si necesitas un conjunto de conjuntos.

### 12.12 Bits en Números Gigantes (`bit_count`, `bit_length`)

- **Cuándo usar:** Contar bits encendidos (`n.bit_count()`, equivalente a `popcount`) o el índice del bit más alto (`n.bit_length()`, cantidad de bits necesarios para representar `n`) en enteros de precisión arbitraria.
- **Cuándo no usar / alternativa:** En Python $< 3.10$ donde no exista `bit_count()`, usar `bin(n).count('1')`.
- **Contrato:** Para `n = 0`, `0.bit_count() == 0` y `0.bit_length() == 0`. Para $n > 0$, $\lfloor \log_2 n \rfloor = \text{n.bit\_length}() - 1$. Dependencias: Python 3.10+.
- **Complejidad:** $\mathcal{O}(\text{palabras de } n)$.

-- bigint_bits.py

- **Trampas y adaptaciones:** (1) En Python los enteros con signo negativo tienen extensión infinita de bits en complemento a 2 (`~x == -x - 1`); aplicar siempre máscara `(x & ((1 << B) - 1))` si simulas registros de $B$ bits.

### 12.13 Búsqueda Binaria con Proyección (`bisect` con `key`) y Ordenamiento Multi-Criterio

- **Cuándo usar:** Hacer `lower_bound` (`bisect_left`) o `upper_bound` (`bisect_right`) proyectando por un atributo con `key=`, u ordenar objetos por múltiples criterios ascendentes/descendentes.
- **Cuándo no usar / alternativa:** En Python $< 3.10$ el parámetro `key=` no existe en `bisect`; en ese caso buscar sobre una lista de tuplas `(clave, ...)`.
- **Contrato:** El `sort` de Python (*Timsort*) es **estable** y $\mathcal{O}(N \log N)$ ($\mathcal{O}(N)$ en datos casi ordenados). Dependencias: `import bisect`.
- **Complejidad:** `bisect` en $\mathcal{O}(\log N)$; `sort` en $\mathcal{O}(N \log N)$.

-- bisect_sorting.py

- **Trampas y adaptaciones:** (1) Para ordenar strings en orden **descendente** dentro de una tupla mixta sin implementar `cmp_to_key`, aprovechar la estabilidad de `sort` ordenando primero por el criterio secundario y luego por el primario.

### 12.14 Combinatoria y Generadores Nativos (`comb`, `perm`, `itertools`)

- **Cuándo usar:** Calcular $\binom{n}{k}$, $P(n, k)$ o $n!$ exactos en enteros gigantes, o iterar todas las permutaciones, combinaciones y productos cartesianos en pocas líneas para fuerza bruta o *stress testing*.
- **Cuándo no usar / alternativa:** Si necesitas $\binom{n}{k} \pmod{10^9+7}$ para $n = 10^6$, `math.comb(10**6, 5*10**5)` construye un número de $300\,000$ bits y da TLE; usar factoriales modulares (§7.4.2).
- **Contrato:** `comb(n, k)` retorna `0` si $k < 0$ o $k > n$. `permutations`, `combinations` y `product` devuelven iteradores perezosos en orden lexicográfico. Dependencias: `math`, `itertools`.
- **Complejidad:** $\mathcal{O}(k)$ operaciones BigInt para `comb(n, k)`.

-- combinatorics_itertools.py

- **Trampas y adaptaciones:** (1) `itertools.combinations_with_replacement(iterable, r)` genera multiconjuntos de tamaño `r` con repetición permitida.

### 12.15 Evaluación de Expresiones Aritméticas (`eval`)

- **Cuándo usar:** Evaluar expresiones con paréntesis, `+`, `-`, `*`, `//`, `%`, `**` o bitmasks (`|`, `&`, `^`) directamente con el parser de Python.
- **Cuándo no usar / alternativa:** Si hay divisiones con operandos **negativos** y el problema exige truncamiento hacia cero estilo C++ (`-7 / 3 == -2`), `//` de Python dará `-3` (hacia $-\infty$); en ese caso usar `ExpressionParser` en C++ (§10.2).
- **Contrato:** La expresión regular `r'(?<!/)/(?!/)'` reemplaza únicamente las barras simples `/` por división entera `//` sin corromper los `//` que ya existían en la cadena. Dependencias: `import re`.
- **Complejidad:** $\mathcal{O}(|S|)$ para expresiones con enteros de tamaño fijo.

-- eval_expressions.py

- **Trampas y adaptaciones:** (1) Nunca usar `.replace('/', '//')` porque convertiría un `'//'` ya existente en `'////'` (`SyntaxError`).
