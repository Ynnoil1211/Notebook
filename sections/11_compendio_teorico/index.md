------------------------------------------------------------------------

## Bloque 11: Compendio Teórico, Teoremas, Fórmulas e Identidades

### 11.1 Condiciones de Optimización de Programación Dinámica (DP Speedups) [opcional]

- **Tabla autoritativa de recurrencias y complejidades**: ver **§8.4**.
- **Desigualdad del Cuadrilátero**: $C(a, c) + C(b, d) \le C(a, d) + C(b, c)$ para todo $a \le b \le c \le d$. Es condición suficiente para que $opt[k][i] \le opt[k][i+1]$ en Divide & Conquer DP. Si además $C(b, c) \le C(a, d)$ (monotonicidad en inclusión), garantiza $opt[l][r-1] \le opt[l][r] \le opt[l+1][r]$ en Knuth DP.
- **CHT ($\mathcal{O}(N)$ vs $\mathcal{O}(N \log N)$)**: Lograr $\mathcal{O}(N)$ con dos punteros exige **simultáneamente** que las pendientes $m_j$ se inserten en orden monótono y que las abscisas de consulta $x_i$ sean monótonas. Si solo $m_j$ es monótona pero $x_i$ es arbitraria, se requiere búsqueda binaria sobre la envolvente en $\mathcal{O}(N \log N)$; si las pendientes tampoco son monótonas, usar `LineContainer` ($\mathcal{O}(N \log N)$).
- **WQS / Aliens Trick**: Para **minimizar** $f(k)$ (costo mínimo usando exactamente $k$ transiciones/grupos), se requiere que $f(k)$ sea **convexa** ($f(k+1) - f(k) \ge f(k) - f(k-1)$); para **maximizar**, que $f(k)$ sea **cóncava** ($f(k+1) - f(k) \le f(k) - f(k-1)$). Al penalizar cada grupo con $\lambda$, si hay tramos colineales en $f(k)$, desempatar siempre tomando la menor (o mayor) cantidad de grupos para reconstruir $K$.

---

### 11.2 Combinatoria (Expresada en Recurrencias, Sumatorias $\sum$ y Productorias $\prod$)

- **Coeficiente Binomial $\binom{n}{r}$ y Permutaciones $P(n, r)$**:
  - **Productoria directa $\mathcal{O}(r)$**: $\binom{n}{r} = \prod_{i=1}^{r} \frac{n - r + i}{i}, \qquad P(n, r) = \prod_{i=0}^{r-1} (n - i)$.
  - **Recurrencia multiplicativa (1 for-loop)**: $C_{n,0} = 1, \quad C_{n,i} = C_{n,i-1} \cdot \frac{n - i + 1}{i} \quad (i = 1..r)$.
  - **Recurrencia aditiva de Pascal (DP $\mathcal{O}(n^2)$ sin dividir, cualquier módulo)**: $C_{n,0} = C_{n,n} = 1, \quad C_{n,r} = C_{n-1,r-1} + C_{n-1,r}$.

- **Coeficiente Multinomial ($\sum_{i=1}^m k_i = n$)**:
  - **Productoria acumulativa de binomiales**: $\binom{n}{k_1, k_2, \dots, k_m} = \prod_{i=1}^{m} \binom{\sum_{j=1}^i k_j}{k_i} = \prod_{i=1}^{m} \binom{n - \sum_{j < i} k_j}{k_i}$.

- **Stars and Bars ($\sum_{i=1}^k x_i = n$)**:
  - **Sin cota superior**: Con $x_i \ge 0$: $\binom{n+k-1}{k-1} = \prod_{i=1}^{k-1} \frac{n+i}{i}$. Con $x_i \ge 1$: $\binom{n-1}{k-1} = \prod_{i=1}^{k-1} \frac{n-k+i}{i}$.
  - **Con cota superior $0 \le x_i \le c$ (Sumatoria)**: $\sum_{j=0}^{\lfloor n/(c+1) \rfloor} (-1)^j \binom{k}{j} \binom{n - j(c+1) + k - 1}{k - 1}$.

- **Identidades de Sumatorias con Binomiales**:
  - **Binomio / Subconjuntos**: $\sum_{k=0}^n \binom{n}{k} a^{n-k} b^k = (a+b)^n; \quad \sum_{k=0}^n \binom{n}{k} = 2^n; \quad \sum_{k=0}^n k\binom{n}{k} = n 2^{n-1}$.
  - **Palo de Hockey (Hockey-Stick)**: $\sum_{i=r}^n \binom{i}{r} = \binom{n+1}{r+1}$.
  - **Vandermonde (Convolución)**: $\sum_{k=0}^r \binom{m}{k}\binom{n}{r-k} = \binom{m+n}{r}$.

- **Sumas Polinomiales y Geométricas ($S_n = S_{n-1} + f(n)$)**:
  - $\sum_{k=1}^n k = \frac{n(n+1)}{2}; \quad \sum_{k=1}^n k^2 = \frac{n(n+1)(2n+1)}{6}; \quad \sum_{k=1}^n k^3 = \left(\frac{n(n+1)}{2}\right)^2$.
  - $\sum_{k=1}^n k^4 = \frac{n(n+1)(2n+1)(3n^2+3n-1)}{30}$.
  - $\sum_{k=0}^n x^k = \frac{x^{n+1}-1}{x-1} \; (x \neq 1); \quad \sum_{k=1}^n k x^k = \frac{x - (n+1)x^{n+1} + n x^{n+2}}{(1-x)^2}$.

- **Números de Catalan ($C_n$)**:
  - **Recurrencia multiplicativa $\mathcal{O}(n)$**: $C_0 = 1, \quad C_n = C_{n-1} \cdot \frac{2(2n-1)}{n+1} = \frac{1}{n+1}\prod_{i=1}^n \frac{n+i}{i}$.
  - **Recurrencia en sumatoria (DP $\mathcal{O}(n^2)$ sin dividir)**: $C_0 = 1, \quad C_n = \sum_{i=0}^{n-1} C_i \cdot C_{n-1-i}$.
  - **Forma cerrada**: $C_n = \frac{1}{n+1}\binom{2n}{n} = \binom{2n}{n} - \binom{2n}{n+1}$ ($1, 1, 2, 5, 14, 42, 132, 429, 1430\dots$).
  - Aplicaciones: Paréntesis balanceados ($n$ pares); triangulaciones de polígono ($n+2$ lados); árboles binarios ($n$ nodos); caminos de Dyck de $(0,0)$ a $(2n,0)$.

- **Desarreglos (Derangements $D_n$ — permutaciones sin puntos fijos)**:
  - **Recurrencia de 1 paso $\mathcal{O}(n)$**: $D_0 = 1, \quad D_i = i \cdot D_{i-1} + (-1)^i$.
  - **Recurrencia aditiva de 2 pasos**: $D_0 = 1, D_1 = 0, \quad D_i = (i-1)(D_{i-1} + D_{i-2})$.
  - **Sumatoria explícita**: $D_n = n! \sum_{k=0}^n \frac{(-1)^k}{k!} = \sum_{k=0}^n (-1)^k \binom{n}{k} (n-k)!$. Con $k$ puntos fijos: $\binom{n}{k} D_{n-k}$.

- **Números de Stirling de 1ra Especie $\left[\begin{matrix}n\\k\end{matrix}\right]$ (permutaciones de $n$ con $k$ ciclos)**:
  - **Recurrencia DP $\mathcal{O}(nk)$**: $\left[\begin{matrix}0\\0\end{matrix}\right] = 1, \quad \left[\begin{matrix}n\\k\end{matrix}\right] = (n-1)\left[\begin{matrix}n-1\\k\end{matrix}\right] + \left[\begin{matrix}n-1\\k-1\end{matrix}\right]$.
  - **Productoria generadora**: $\sum_{k=0}^n \left[\begin{matrix}n\\k\end{matrix}\right] x^k = \prod_{i=0}^{n-1} (x + i)$.

- **Números de Stirling de 2da Especie $\left\{\begin{matrix}n\\k\end{matrix}\right\}$ (particionar $n$ en $k$ cajas no vacías)**:
  - **Recurrencia DP $\mathcal{O}(nk)$**: $\left\{\begin{matrix}0\\0\end{matrix}\right\} = 1, \quad \left\{\begin{matrix}n\\k\end{matrix}\right\} = k \left\{\begin{matrix}n-1\\k\end{matrix}\right\} + \left\{\begin{matrix}n-1\\k-1\end{matrix}\right\}$.
  - **Sumatoria directa $\mathcal{O}(k \log n)$**: $\left\{\begin{matrix}n\\k\end{matrix}\right\} = \frac{1}{k!} \sum_{j=0}^k (-1)^{k-j}\binom{k}{j} j^n$. (Sobreyectivas $n \to k$: multiplicar por $k!$).

- **Números de Bell ($B_n$ — particiones totales de $n$ elementos)**:
  - **Recurrencia en sumatoria**: $B_0 = 1, \quad B_n = \sum_{k=0}^{n-1}\binom{n-1}{k}B_k = \sum_{k=0}^n \left\{\begin{matrix}n\\k\end{matrix}\right\}$.

- **Particiones de Enteros $p(n)$ (Recurrencia Pentagonal de Euler $\mathcal{O}(n\sqrt{n})$)**:
  - $p(0) = 1, \quad p(n) = \sum_{k \ge 1} (-1)^{k-1} \left[ p\!\left(n - \frac{k(3k-1)}{2}\right) + p\!\left(n - \frac{k(3k+1)}{2}\right) \right]$.

- **Lema de Burnside y Collares (Sumatoria sobre simetrías / divisores)**:
  - $|X/G| = \frac{1}{|G|} \sum_{g \in G} |X^g|; \qquad \text{Collares}(n, k) = \frac{1}{n} \sum_{d \mid n} \phi(d) k^{n/d}$.

---

### 11.3 Teoría de Números y Aritmética Modular

- **Ecuaciones Diofánticas Lineales**:
  - $a x + b y = c$ tiene solución entera si y solo si $g = \gcd(a, b) \mid c$.
  - Si $(x_0, y_0)$ es una solución particular, todas las soluciones son:
    $x = x_0 + k \cdot \frac{b}{g}, \quad y = y_0 - k \cdot \frac{a}{g} \quad (k \in \mathbb{Z})$.

- **Problema de las Monedas de Frobenius (Chicken McNuggets Theorem)**:
  - Para $\gcd(a, b) = 1$, el mayor número entero que NO puede expresarse como $a x + b y$ ($x, y \ge 0$) es:
    $g(a, b) = a \cdot b - a - b$.
  - El número exacto de enteros positivos no representables es $\frac{(a-1)(b-1)}{2}$.

- **Teorema de Wilson**:
  - Un entero $p > 1$ es primo si y solo si $(p-1)! \equiv -1 \pmod p$.

- **Teorema de Euler y Reducción de Torres de Potencias**:
  - Si $\gcd(a, m) = 1$, $a^{\phi(m)} \equiv 1 \pmod m$.
  - En general para cualquier $a$:
    $a^b \equiv a^{b \pmod{\phi(m)} + \phi(m)} \pmod m \quad \text{si } b \ge \phi(m)$.

- **Fórmula de Inversión de Möbius**:
  - $g(n) = \sum_{d \mid n} f(d) \iff f(n) = \sum_{d \mid n} \mu(d) g\left(\frac{n}{d}\right)$.
  - Propiedad clave: $\sum_{d \mid n} \mu(d) = [n = 1]$.
  - Identidad de coprimalidad: $[\gcd(i, j) = 1] = \sum_{d \mid \gcd(i, j)} \mu(d)$.

- **Símbolo de Legendre y Criterio de Euler**:
  - $\left(\frac{a}{p}\right) \equiv a^{(p-1)/2} \pmod p \in \{1, -1, 0\}$.
  - $a$ es residuo cuadrático mod $p$ primo impar si $\left(\frac{a}{p}\right) = 1$.
  - Ley de Reciprocidad Cuadrática: $\left(\frac{p}{q}\right)\left(\frac{q}{p}\right) = (-1)^{\frac{p-1}{2}\frac{q-1}{2}}$.

- **Teorema de los Dos Cuadrados de Fermat**:
  - Un primo impar $p$ se puede escribir como $a^2 + b^2$ si y solo si $p \equiv 1 \pmod 4$.
  - Un entero $n$ es suma de dos cuadrados si y solo si todo factor primo $q \equiv 3 \pmod 4$ aparece con exponente par en su factorización.

- **Ternas Pitagóricas Primitivas**:
  - Todo triplete $a^2 + b^2 = c^2$ con $\gcd(a, b, c) = 1$ se genera mediante:
    $a = m^2 - n^2, \quad b = 2mn, \quad c = m^2 + n^2$, con $m > n > 0$, $\gcd(m, n) = 1$, y $m, n$ de distinta paridad.

- **Raíces Primitivas**:
  - $g$ es raíz primitiva mod $n$ si el orden multiplicativo de $g$ es $\phi(n)$.
  - Existen raíces primitivas solo para $n \in \{1, 2, 4, p^k, 2p^k\}$ con $p$ primo impar.
  - Para verificar si $g$ es raíz primitiva mod $p$: verificar que $g^{(p-1)/q} \not\equiv 1 \pmod p$ para todos los factores primos $q$ de $p-1$.

- **Período de Pisano (Fibonacci mod $M$)**:
  - El período $\pi(M)$ de Fibonacci módulo $M$ siempre existe y es $\le 6M$.
  - $\pi(10^k) = 1.5 \times 10^k$ para todo $k \ge 3$. $\pi(10^9) = 1.5 \times 10^9$.

---

### 11.4 Teoría de Grafos, Árboles y Redes

- **Teorema Matriz-Árbol de Kirchhoff (Matrix-Tree Theorem)**:
  - Sea $L = D - A$ la matriz Laplaciana del grafo ($D_{ii} = \operatorname{deg}(v_i)$, $A_{ij} = \text{aristas entre } i \text{ y } j$).
  - El número de árboles generadores (spanning trees) del grafo es igual al valor absoluto de cualquier cofactor de $L$ (eliminar fila $i$ y columna $i$ y calcular $\det(L_{ii})$).
  - En grafos dirigidos: el número de arborescencias dirigidas enraizadas en $r$ es el cofactor del Laplaciano dirigido $L^{\text{dir}} = D^{\text{in}} - A$ eliminando la fila y columna $r$.

- **Fórmula de Cayley y Bosques**:
  - Número de árboles etiquetados en $n$ vértices: $n^{n-2}$.
  - Número de bosques etiquetados en $n$ vértices con $k$ componentes conexas donde los vértices $1, \dots, k$ pertenecen a componentes distintas: $k \cdot n^{n-k-1}$.

- **Código de Prüfer**:
  - Biyección entre árboles etiquetados con $n$ vértices y secuencias de longitud $n-2$ con elementos en $\{1, \dots, n\}$.
  - Un vértice con grado $d$ aparece exactamente $d - 1$ veces en el código de Prüfer.

- **Teorema de König y Relaciones de Gallai**:
  - En grafos bipartitos:
    $\text{Matching Máximo} = \text{Min Vertex Cover}$.
    $\text{Max Independent Set} = V - \text{Matching Máximo}$.
  - Teorema de Gallai (para grafos generales sin vértices aislados):
    $\alpha(G) + \tau(G) = V \quad (\text{Indep Set} + \text{Vertex Cover} = V)$.
    $\alpha'(G) + \tau'(G) = V \quad (\text{Matching Máximo} + \text{Edge Cover Mínimo} = V)$.

- **Teorema de Dilworth (Anticadenas en Posets)**:
  - En cualquier conjunto parcialmente ordenado (DAG transitivo), el tamaño de la **anticadena más grande** es igual al número mínimo de cadenas disjuntas necesarias para cubrir todos los elementos.
  - Se resuelve modelando como Maximum Bipartite Matching.

- **Teorema de Tutte-Berge y Matrimonio de Hall**:
  - Condición de Hall para emparejamiento completo de $X$ a $Y$ en bipartito:
    $|N(S)| \ge |S|$ para todo subconjunto $S \subseteq X$.
  - Fórmula de Tutte-Berge: El tamaño del matching máximo en cualquier grafo es $\frac{1}{2} \min_{U \subseteq V} (|V| + |U| - \text{odd}(G - U))$.

- **Criterio Gráfico de Erdős-Gallai**:
  - Una secuencia de grados $d_1 \ge d_2 \ge \dots \ge d_n$ corresponde a un grafo simple no dirigido si y solo si $\sum d_i$ es par y para todo $1 \le k \le n$:
    $\sum_{i=1}^k d_i \le k(k-1) + \sum_{i=k+1}^n \min(d_i, k)$.

- **Emparejamiento Estable (Gale-Shapley)**:
  - Si los proponentes son los hombres, el algoritmo genera el emparejamiento estable **óptimo para los hombres** y **pésimo para las mujeres** en $\mathcal{O}(N^2)$.

- **Flujo Máximo con Demandas (Cotas Inferiores $l(u, v) \le f(u, v) \le c(u, v)$)**:
  - Crear super-fuente $S'$ y super-sumidero $T'$.
  - Para cada arista $(u, v)$ con capacidad $[l, c]$, agregar arista en la red con capacidad $c - l$.
  - Para cada vértice $u$, definir balance $b(u) = \sum_{v} l(v, u) - \sum_{w} l(u, w)$.
  - Si $b(u) > 0$, agregar arista $(S', u)$ con capacidad $b(u)$.
  - Si $b(u) < 0$, agregar arista $(u, T')$ con capacidad $-b(u)$.
  - Agregar arista infinita $(T, S)$ con capacidad $\infty$. Si el flujo máximo desde $S'$ satura todas las aristas que salen de $S'$, existe circulación factible.

- **Fórmula de Euler para Grafos Planares**:
  - $V - E + F = 1 + C$, donde $C$ es el número de componentes conexas ($V - E + F = 2$ si es conexo).
  - Corolarios para grafos simples conexos con $V \ge 3$:
    $E \le 3V - 6$ (si no tiene triángulos, $E \le 2V - 4$).
    Siempre existe al menos un vértice con grado $\le 5$.

---

### 11.5 Geometría y Distancias

- **Teorema de Pick**:
  - Para un polígono simple con vértices en coordenadas enteras (retícula):
    $\text{Área} = I + \frac{B}{2} - 1$
    donde $I$ es el número de puntos enteros estrictamente interiores y $B$ es el número de puntos enteros en el borde.
  - Un **segmento cerrado** entre $(x_1, y_1)$ y $(x_2, y_2)$ contiene exactamente $\gcd(|x_2 - x_1|, |y_2 - y_1|) + 1$ puntos enteros; al sumar sobre las aristas cerrando el polígono (contando cada vértice una sola vez, arista semiabierta $[P_i, P_{i+1})$), la contribución de cada lado al total $B$ del borde es $\gcd(|x_2 - x_1|, |y_2 - y_1|)$.

- **Fórmula de Herón y Radios de Triángulos**:
  - Semiperímetro: $s = \frac{a+b+c}{2}$.
  - Área: $A = \sqrt{s(s-a)(s-b)(s-c)}$.
  - Radio del incírculo (inradius): $r = \frac{A}{s}$.
  - Radio del circuncírculo (circumradius): $R = \frac{abc}{4A}$.

- **Distancia de Gran Círculo (Fórmula de Haversine)**:
  - Distancia entre dos puntos en una esfera de radio $R$ con latitud/longitud $(\phi_1, \lambda_1)$ y $(\phi_2, \lambda_2)$ en radianes:
    $d = 2R \arcsin\left(\sqrt{\sin^2\left(\frac{\Delta\phi}{2}\right) + \cos(\phi_1)\cos(\phi_2)\sin^2\left(\frac{\Delta\lambda}{2}\right)}\right)$.

---

### 11.6 Teoría de Juegos y XOR

- **Teorema de Sprague-Grundy**:
  - Todo juego **imparcial, finito/terminante (acíclico)** de información perfecta bajo **convención normal** (gana quien realiza la última jugada) es equivalente a un montón de Nim con tamaño igual a su valor de Grundy $G(u) = \operatorname{mex}\{G(v) : u \to v\}$.
  - Para la suma disjunta de juegos independientes: $G(S_1 + S_2 + \dots) = G(S_1) \oplus G(S_2) \oplus \dots$
  - El estado es **ganador** para el primer jugador si y solo si la suma XOR es $\neq 0$.

- **Juego de Nim**:
  - Posición ganadora $\iff x_1 \oplus x_2 \oplus \dots \oplus x_n \neq 0$.
  - **Regla de Misère Nim** (pierde quien toma la última ficha): Se juega idéntico al Nim normal con suma XOR $\neq 0$, EXCEPTO cuando todos los montones tienen tamaño $\le 1$: en ese caso, la posición es ganadora si y solo si el número de montones de tamaño 1 es **par**.

- **Juego de Moore ($\text{Nim}_k$)**:
  - En cada turno, el jugador puede retirar fichas de entre $1$ y $k$ montones distintos.
  - Condición de victoria: Sumar cada columna binaria de los tamaños de los montones en base 10; la posición es perdedora si y solo si la suma de cada columna binaria es un múltiplo de $k + 1$.

---

### 11.7 Reglas Lógicas de 2-SAT [opcional]

| Cláusula Deseada | Implicaciones en el Grafo de 2-SAT |
| :--- | :--- |
| **$p$ debe ser Verdadero** | $\neg p \implies p$ |
| **$p$ debe ser Falso** | $p \implies \neg p$ |
| **Al menos uno ($p \lor q$)** | $\neg p \implies q \quad \text{y} \quad \neg q \implies p$ |
| **No ambos $\neg(p \land q) \equiv (\neg p \lor \neg q)$** | $p \implies \neg q \quad \text{y} \quad q \implies \neg p$ |
| **Equivalentes ($p = q$)** | $p \implies q, \quad q \implies p, \quad \neg p \implies \neg q, \quad \neg q \implies \neg p$ |
| **Distintos / XOR ($p \neq q$)** | $p \implies \neg q, \quad \neg p \implies q, \quad q \implies \neg p, \quad \neg q \implies p$ |
| **A lo sumo uno entre $x_1, \dots, x_n$** | Crear prefijos $p_i = p_{i-1} \lor x_i$: $x_i \implies p_i$, $p_{i-1} \implies p_i$, $p_{i-1} \implies \neg x_i$. |

- **Solubilidad**: El sistema 2-SAT es satisfacible si y solo si para ninguna variable $x$, $x$ y $\neg x$ caen en la misma Componente Fuertemente Conexa (SCC). La asignación se obtiene eligiendo el valor cuya SCC aparece después en el orden topológico.

---

### 11.8 Tabla de Constantes, Primos y Módulos Frecuentes

| Tipo | Constante / Primo | Propiedades Clave |
| :--- | :--- | :--- |
| **Estándar** | $10^9 + 7 = 1000000007$ | Primo común en Codeforces/ICPC ($2 \times 10^9 + 14$ entra en signed 32-bit). |
| **Estándar 2** | $10^9 + 9 = 1000000009$ | Ideal para segundo módulo en doble rolling hash con $10^9+7$. |
| **Primos Hash** | $10^9 + 21, 10^9 + 33, 10^9 + 87$ | Primos grandes para evitar hacks en tablas hash. |
| **NTT Primo 1** | $998244353 = 119 \cdot 2^{23} + 1$ | Raíz primitiva $g = 3$. Soporta transformadas de tamaño hasta $2^{23} \approx 8.38 \times 10^6$. |
| **NTT Primo 2** | $1004535809 = 479 \cdot 2^{21} + 1$ | Raíz primitiva $g = 3$. Soporta tamaños hasta $2^{21} \approx 2.09 \times 10^6$. |
| **NTT Primo 3** | $469762049 = 7 \cdot 2^{26} + 1$ | Raíz primitiva $g = 3$. Soporta tamaños hasta $2^{26} \approx 6.71 \times 10^7$. |
| **64-bit Prime** | $10^{18} + 9$ (`1000000000000000009`) | Primo de 64 bits para pruebas con Miller-Rabin. |
| **64-bit Prime** | $10^{18} + 31$ (`1000000000000000031`) | Segundo primo de 64 bits. |
| **Bases Miller-Rabin** | `2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37` | Conjunto determinista para probar primalidad de cualquier $N < 2^{64}$. |
| **Primos Gemelos** | $(10^9 + 7, 10^9 + 9)$ | Pareja de primos gemelos útiles para hashes combinados. |
