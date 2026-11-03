------------------------------------------------------------------------

## Bloque 9: Geometría Computacional 2D

### 9.1 Geometría 2D: `Pt`, `PtLL` (Entero), Segmentos, Polígonos (Área, Centroide, Punto en Polígono $\mathcal{O}(N)$ y Convexo $\mathcal{O}(\log N)$), `Convex Hull`, Rotating Callipers y Closest Pair

#### 9.1.1 Punto 2D, Producto Punto, Producto Cruz y Orden Radial (`Pt`, `dot`, `cross`, `polar_cmp`)

- **Cuándo usar:** Base vectorial 2D para giros (`cross(a, b, c) > 0` es giro antihorario CCW), distancias y ordenamiento angular de vectores alrededor del origen (`polar_cmp`) en $[0, 2\pi)$ sin usar `atan2`.
- **Cuándo no usar / alternativa:** No usar `operator<` lexicográfico si buscas agrupar puntos con tolerancia `EPS` sin antes redondear, ya que la igualdad con `EPS` no es transitiva.
- **Contrato:** `operator<` y `operator==` son exactos para cumplir *strict weak ordering* en `std::sort`/`std::set`; para comparar con tolerancia usar `pt_eq_eps(a, b)`. `polar_cmp` separa el semiplano superior ($y > 0$ o $y = 0, x > 0$) del inferior y desempata por `cross(a, b) > 0`. Dependencias: `EPS` (§1.1).
- **Complejidad:** $\mathcal{O}(1)$ para productos vectoriales; $\mathcal{O}(N \log N)$ para ordenar $N$ puntos con `polar_cmp`.

-- geometry_2d.cpp

- **Trampas y adaptaciones:** (1) El vector nulo `(0, 0)` no tiene ángulo definido; filtrarlo antes de ordenar con `polar_cmp`.

#### 9.1.2 Segmentos y Rectas: Pertenencia, Distancia e Intersección (`on_segment`, `dist_point_segment`, `segments_intersect`, `line_intersection`)

- **Cuándo usar:** Verificar si un punto cae sobre un segmento cerrado, medir la distancia de un punto a un segmento, comprobar si dos segmentos cerrados se tocan o cruzan, y hallar el punto de intersección de dos rectas no paralelas.
- **Cuándo no usar / alternativa:** Antes de llamar `line_intersection(a, b, c, d)`, verificar que las rectas no sean paralelas (`abs(cross(b - a, d - c)) > EPS`).
- **Contrato:** `segments_intersect` cubre tanto cruces propios como casos colineales donde un extremo toca al otro segmento (`on_segment`). Dependencias: `Pt`, `dot`, `cross`, `sgn` (§9.1.1).
- **Complejidad:** $\mathcal{O}(1)$ tiempo y memoria.

-- segments_polygons.cpp

- **Trampas y adaptaciones:** (1) Para la distancia entre **dos segmentos** $AB$ y $CD$: si `segments_intersect(a, b, c, d)` es `true` la distancia es `0`; si no, es el mínimo de las 4 distancias `dist_point_segment` de cada extremo al otro segmento.

#### 9.1.3 Polígonos: Área, Centroide, Inclusión $\mathcal{O}(N)$ / Convexo $\mathcal{O}(\log N)$ y Kernel Entero (`polygon_area`, `polygon_centroid`, `point_in_polygon`, `point_in_convex_polygon`, `PtLL`)

- **Cuándo usar:** Calcular el área de un polígono simple (Shoelace), su centro de masa (`polygon_centroid`), probar si un punto está dentro/en el borde/fuera de un polígono general en $\mathcal{O}(N)$ o convexo en $\mathcal{O}(\log N)$, o calcular el doble del área en enteros exactos de 128 bits (`polygon_area2_int`).
- **Cuándo no usar / alternativa:** No usar `point_in_convex_polygon` si el polígono no es convexo o está en sentido horario (CW) o tiene vértices colineales duplicados en los bordes.
- **Contrato:** `point_in_polygon` y `point_in_convex_polygon` retornan `2` (en el **borde**), `1` (**estrictamente dentro**) o `0` (**fuera**). `polygon_area2_int` retorna $2 \cdot \text{Área} \in \mathbb{Z}$ sin usar punto flotante. Dependencias: `Pt`, `on_segment` (§9.1.1–§9.1.2).
- **Complejidad:** Área, centroide y `point_in_polygon` en $\mathcal{O}(N)$; `point_in_convex_polygon` en $\mathcal{O}(\log N)$.

-- geometry_int.cpp

- **Lectura de entrada y uso:** Leer los $N$ vértices en orden; usar `PtLL` para doble área entera exacta y `Pt` para inclusión de puntos:
```cpp
int n, q; cin >> n >> q;
vector<Pt> poly(n); vector<PtLL> poly_i(n);
forn(i, n) {
    ll x, y; cin >> x >> y;
    poly[i] = {(ld)x, (ld)y}; poly_i[i] = {x, y};
}
ll area2 = polygon_area2_int(poly_i); // 2 * Area exacta
while (q--) {
    Pt p; cin >> p.x >> p.y;
    int st = point_in_polygon(p, poly); // 0:out, 1:in, 2:on
}
```
- **Trampas y adaptaciones:** (1) Por el **Teorema de Pick**, en un polígono con vértices enteros, $2 \cdot \text{Área} = 2 I + B - 2$, donde $B = \sum \gcd(|\Delta x_i|, |\Delta y_i|)$ son los puntos enteros en el borde e $I$ los puntos estrictamente interiores.

#### 9.1.4 Envolvente Convexa de Andrew (`convex_hull`)

- **Cuándo usar:** Obtener el menor polígono convexo (en sentido antihorario CCW) que contiene a un conjunto de $N$ puntos en el plano.
- **Cuándo no usar / alternativa:** Si los puntos se insertan online y solo son gráficas de rectas $y = kx + m$, usar `LineContainer` (§8.4.1).
- **Contrato:** Elimina puntos duplicados y descarta puntos colineales intermedios en los lados (`cross(...) <= 0`). Retorna los vértices del casco en orden CCW sin repetir el primer vértice al final. Dependencias: `Pt`, `cross` (§9.1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- convex_hull.cpp

- **Lectura de entrada y uso:** Leer $N$ puntos en `vector<Pt> pts(n)` y obtener el polígono convexo CCW en `convex_hull(pts)`:
```cpp
int n; cin >> n;
vector<Pt> pts(n);
forn(i, n) cin >> pts[i].x >> pts[i].y;
vector<Pt> hull = convex_hull(pts); // vertices en orden CCW
```
- **Trampas y adaptaciones:** (1) Si el problema exige **conservar** los puntos colineales en el borde del Convex Hull, cambiar `<` (`<= 0` pasa a `< 0`) en la condición del `while` y cuidar de no duplicar los extremos.

#### 9.1.5 Diámetro de Polígono Convexo por Rotating Callipers (`rotating_callipers_max_dist2`) [opcional]

- **Cuándo usar:** Encontrar la mayor distancia euclidiana al cuadrado entre dos puntos de una nube de $N$ puntos (pasándole su `convex_hull` CCW) en $\mathcal{O}(N)$.
- **Cuándo no usar / alternativa:** Para la **menor** distancia entre dos puntos, el par más cercano no tiene por qué estar en el Convex Hull; usar `closest_pair_distance` (§9.1.6).
- **Contrato:** Recibe un polígono convexo CCW `h` (resultado de `convex_hull`). Avanza el puntero antipodal `j` mientras el área del triángulo con la arista `i -> i+1` crezca. Dependencias: `Pt`, `cross` (§9.1.1).
- **Complejidad:** $\mathcal{O}(N)$ sobre el casco convexo ($\mathcal{O}(N \log N)$ incluyendo `convex_hull`).

-- rotating_callipers.cpp

- **Trampas y adaptaciones:** (1) Siempre pasar a `rotating_callipers_max_dist2` el polígono **sin puntos colineales redundantes** generado por `convex_hull`.

#### 9.1.6 Par de Puntos Más Cercanos por Sweep-Line (`closest_pair_distance`) [opcional]

- **Cuándo usar:** Hallar la mínima distancia euclidiana entre dos puntos distintos (o `0` si hay puntos duplicados) de un conjunto de $N$ puntos en $\mathcal{O}(N \log N)$.
- **Cuándo no usar / alternativa:** Si las distancias son Manhattan ($|x_1 - x_2| + |y_1 - y_2|$) y quieres el **máximo**, basta $\max(\max(x+y) - \min(x+y), \max(x-y) - \min(x-y))$ en $\mathcal{O}(N)$.
- **Contrato:** Si `sz(pts) < 2` retorna `INF`. Detecta puntos idénticos antes de iniciar el barrido y retorna `0` sin producir división por cero ni comparación inválida en `std::set`. Dependencias: `Pt` (§9.1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- closest_pair.cpp

- **Trampas y adaptaciones:** (1) El `std::set` activo durante el barrido ordena por `(y, x)` (no por `(x, y)`) para poder acotar el rango vertical $[y - d, y + d]$ con `lower_bound`.

------------------------------------------------------------------------

### 9.2 Geometría Analítica Básica y Círculos: Proyección, Reflexión, Distancia Punto-Recta y `Circle`

#### 9.2.1 Proyección, Reflexión y Distancia Punto-Recta (`proj_point_line`, `reflect_point_line`, `dist_point_line`)

- **Cuándo usar:** Proyectar ortogonalmente un punto $P$ sobre una recta infinita $AB$, reflejar $P$ respecto al espejo $AB$, o medir la distancia perpendicular de $P$ a la recta infinita $AB$.
- **Cuándo no usar / alternativa:** Si el espejo es un **segmento acotado** $AB$ y buscas distancia, usar `dist_point_segment` (§9.1.2).
- **Contrato:** Maneja el caso degenerado $A = B$ retornando $A$ (o $|P - A|$ en distancia). Dependencias: `Pt`, `dot`, `cross` (§9.1.1).
- **Complejidad:** $\mathcal{O}(1)$ tiempo y memoria.

-- lines_projection_reflection.cpp

- **Trampas y adaptaciones:** (1) El camino más corto desde $S$ hasta $T$ tocando una recta $AB$ tiene longitud $|\text{reflect}(S, A, B) - T|$.

#### 9.2.2 Geometría de Círculos: Intersecciones y Tangentes (`Circle`, `circle_line_intersection`, `circle_circle_intersection`, `circle_tangents_from_point`) [opcional]

- **Cuándo usar:** Hallar los puntos de corte entre una recta y un círculo, entre dos círculos, o los puntos de tangencia trazados a un círculo desde un punto exterior $P$.
- **Cuándo no usar / alternativa:** Siempre distinguir el código `-1` de `circle_circle_intersection` (círculos idénticos con infinitos puntos de contacto) del código `0` (disjuntos).
- **Contrato:** Usa `sqrt(max((ld)0.0, h2))` para evitar `NaN` cuando la recta o el círculo son casi tangentes ($h^2 \approx -10^{-16}$). Dependencias: `proj_point_line` (§9.2.1), `Pt`, `pt_eq_eps` (§9.1.1).
- **Complejidad:** $\mathcal{O}(1)$ tiempo y memoria.

-- circles_tangents_intersections.cpp

- **Trampas y adaptaciones:** (1) En problemas de cobertura angular sobre un círculo (ej. cuántos círculos cubren el borde de $C_i$), convertir los dos puntos de `circle_circle_intersection` en ángulos con `atan2` y hacer sweep-line (§3.7.1).

------------------------------------------------------------------------

### 9.3 Intersección de Semiplanos (Halfplane Intersection) [opcional]

- **Cuándo usar:** Construir el polígono convexo que resulta de intersectar $N$ semiplanos dirigidos (región a la izquierda del vector $A \to B$), como el núcleo de visibilidad de un polígono (*Art Gallery Kernel*) o factibilidad de programación lineal en 2D.
- **Cuándo no usar / alternativa:** Si solo necesitas saber el valor óptimo lineal en 2D o más dimensiones sin los vértices del polígono, `Simplex` (§7.8) también lo resuelve.
- **Contrato:** Cada `Halfplane(a, b)` representa el semiplano a la **izquierda** del rayo $a \to b$ (`cross(b - a, q - a) >= 0`). Acota regiones abiertas con una caja `[-box, box]^2`. Si la intersección es vacía o degenerada (segmento/punto de área 0), retorna `{}`. Dependencias: `Pt`, `cross` (§9.1.1).
- **Complejidad:** $\mathcal{O}(N \log N)$ tiempo y $\mathcal{O}(N)$ memoria.

-- halfplane_intersection.cpp

- **Lectura de entrada y uso:** Cada `Halfplane(a, b)` conserva el semiplano a la **izquierda** del vector dirigido $a \to b$:
```cpp
vector<Halfplane> hps;
forn(i, n) {
    Pt a, b; cin >> a.x >> a.y >> b.x >> b.y;
    hps.pb(Halfplane(a, b));
}
vector<Pt> poly = halfplane_intersection(hps);
```
- **Trampas y adaptaciones:** (1) Para el núcleo de un polígono dado en orden CCW, agregar un `Halfplane(p[i], p[(i + 1) % n])` por cada lado del polígono.

### 9.4 Suma de Minkowski de Polígonos Convexos [opcional]

- **Cuándo usar:** Construir en $\mathcal{O}(N + M)$ el polígono convexo $P + Q = \{p + q : p \in P, q \in Q\}$, útil para colisión de polígonos convexos ($P$ intersecta a $Q + v \iff v \in P + (-Q)$) o máximo de dos funciones convexas.
- **Cuándo no usar / alternativa:** Ambos polígonos de entrada deben estar en orden **antihorario (CCW)**.
- **Contrato:** Soporta casos degenerados donde $P$ o $Q$ tienen $1$ o $2$ puntos. Reordena ambos polígonos empezando por el vértice de menor $(y, x)$ y fusiona sus vectores de aristas por ángulo. Dependencias: `Pt`, `cross` (§9.1.1).
- **Complejidad:** $\mathcal{O}(|P| + |Q|)$ tiempo y memoria.

-- minkowski_sum.cpp

- **Trampas y adaptaciones:** (1) La distancia mínima entre dos polígonos convexos disjuntos $P$ y $Q$ es la distancia del origen $(0, 0)$ al polígono $P + (-Q)$.
