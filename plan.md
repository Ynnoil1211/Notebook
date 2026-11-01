# Plan de corrección y validación del notebook ICPC

Actualizado: **2026-11-01**.

## 1. Objetivo, alcance y reglas de trabajo

Convertir `Notebook_ICPC_Final.md` en una referencia confiable para competencia: implementación correcta dentro de un dominio explícito, documentación fiel a su API y ejemplos que lean y utilicen entrada real.

Este plan incorpora la auditoría anterior, las correcciones ya realizadas y los errores detectados en la revisión actual. Actualizar el plan **no significa que los pendientes estén corregidos**.

> Antes del código de cada algoritmo deben aparecer su explicación de uso, contrato, formato, entrada/salida y lector real. Después deben mostrarse la llamada y la producción de la salida. La mera presencia de estos encabezados no acredita corrección.

### Prioridades

- **P0 — Bloqueante:** resultados incorrectos, accesos inválidos, división por cero, ciclos infinitos, comparadores inválidos o ejemplos cuya lectura falla.
- **P1 — Necesario para cerrar la auditoría:** contratos, límites numéricos, capacidades anunciadas, cobertura efectiva de pruebas e integración.
- **P2 — Cierre editorial y operativo:** navegación, documentación, render, legibilidad y simulación de competencia.

### Restricciones

- Primero incorporar contraejemplos y corregir; después ampliar contenido o retocar maquetación.
- No reescribir algoritmos correctos ni cambiar interfaces sin necesidad.
- No agregar funcionalidades solamente para justificar un título: estrechar la promesa cuando sea la solución más sencilla.
- Mantener una implementación canónica por algoritmo, salvo variantes justificadas.
- No modificar ni revertir trabajo ajeno al cambio.
- No renumerar el notebook durante las correcciones.
- Usar nombres de sección, símbolos e IDs estables. Las líneas citadas corresponden a la versión auditada y se desplazarán al editar.
- Distinguir siempre entre corrección demostrada, inspección, implementación presente y validación pendiente.

## 2. Estado actual y evidencia disponible

### 2.1 Inventario y herramientas

| Elemento | Estado observado |
| --- | --- |
| Fuente editorial | `Notebook_ICPC_Final.md`, aproximadamente 12 853 líneas |
| Fichas registradas | 164: 149 C++ y 15 Python |
| Secciones teóricas o de referencia | 13 |
| Bloques extraídos | 820 |
| Suite existente | `python -X utf8 tests/run_tests.py --strict` |
| Resultado de la suite | **25 suites aprobadas, 0 fallos**, en 57,31 s |
| Ejemplos efectivamente seleccionados por la suite | 162: 147 C++ y 15 Python |
| Ejemplos omitidos por la selección | §1.1 y §6.2.5; comprobados aparte y sus salidas publicadas funcionan |
| Comprobación adicional de lectores C++ | 147 lectores instrumentados; falla `cin` en §4.5.2 |
| Compatibilidad C++17 comprobada adicionalmente | Ensamblado conjunto de esos 147 ejemplos y los otros dos ejemplos por separado |
| PDF regenerado temporalmente | **53 páginas**, aproximadamente 5,48 MB |
| No destructividad del build comprobado | Hashes de Markdown, PDF original y `body.typ` original sin cambios |
| Entorno de Python usado | CPython 3.11.9 en Windows |
| UBSan | No disponible en el toolchain auditado |

**Interpretación:** `164/164 completo` describe actualmente la presencia y el orden de campos. No certifica que todas las API tengan ejemplo, que las lecturas sean válidas ni que las respuestas sean semánticamente correctas.

El ensamblado conjunto puede ocultar dependencias no declaradas. No debe presentarse como compilación aislada de cada ficha.

### 2.2 Correcciones anteriores verificadas: conservar sus regresiones

- [x] Separar comparación aproximada con EPS del orden de claves del template base.
- [x] PBDS: reemplazar `less_equal` por pares `(valor, id)` ordenados con `less`.
- [x] Puentes: resolver el contraejemplo de dos aristas paralelas.
- [x] Push-Relabel: superar la regresión de `gap`/`relabel` y las pruebas diferenciales existentes.
- [x] Miller–Rabin: usar multiplicación unsigned de 128 bits y superar casos por encima de `2^63`.
- [x] BM: normalizar discrepancias e inversos; `{0,1,1,2}`, índice `4`, devuelve `3`.
- [x] CRT: ensanchar el producto original del paso; la regresión devuelve `5000000006 10000000014`.
- [x] Closest pair: corregir el acceso inválido con tres puntos idénticos.
- [x] Minkowski: superar la regresión punto más triángulo y los accesos originales fuera de rango.
- [x] Block-Cut Tree: incorporar retrocesos, paralelas y vértices aislados en los casos probados.
- [x] D&C DP: no propagar `opt=-1` desde estados sin predecesor alcanzable.
- [x] Criba segmentada: manejar `0` y `1` y consolidar la implementación duplicada.
- [x] Misère Nim: contar montones de una ficha, incluyendo entradas con montones cero.
- [x] Parser C++: representar menos unario sin depender del signedness de `char`.
- [x] Python: truncar divisiones BigInt con aritmética entera, sin convertir a `float`.
- [x] Python: reemplazar solamente `/` simples, sin convertir `//` en `////`.
- [x] Incorporar persistencia de frecuencias, matching con König, Gauss modular, LCS, CHT monótono, NTT y Lagrange arbitrario.
- [x] Incorporar MITM, subset DP, rerooting, greedy, grafos funcionales, geometría analítica y conteo/esperanza.
- [x] Declarar construcción de HLD `O(N log N)` y criba de Möbius `O(N log N)`.
- [x] Advertir sobre colisiones de rolling hash y distinguir pendientes y consultas monótonas en CHT.
- [x] Mantener una sola implementación de criba segmentada, Euler dirigido, Húngaro y Blossom.
- [x] Incorporar guía inicial, checklist, separación DP/geometría e índice automático en la plantilla.
- [x] Compilar desde otro directorio con `--body` y salida temporales, sin alterar los originales.

Estos puntos no cierran algoritmos completos: por ejemplo, CRT, BM, Bellman-Ford y geometría conservan pendientes descritos abajo.

### 2.3 Validaciones que no deben darse por hechas

- [ ] Incorporar a la suite permanente los contraejemplos nuevos de esta auditoría.
- [ ] Compilar cada driver de forma aislada con sus dependencias explícitas.
- [ ] Revisar visualmente el PDF a tamaño real y en escala de grises.
- [ ] Validar experimentalmente `--watch`, incluidos sus fallos y recuperación.
- [ ] Validar el wrapper PowerShell y todas las opciones documentadas.
- [ ] Ejecutar una simulación real de competencia con tiempos y dificultades registrados.

## 3. Archivos afectados y fuente de verdad

| Archivo | Trabajo previsto |
| --- | --- |
| `Notebook_ICPC_Final.md` | Corregir algoritmos, contratos, capacidades, lectores, llamadas, salidas y adaptaciones. |
| `tools/extract_snippets.py` | Corregir clasificación, IDs, selección y resolución explícita de dependencias. |
| `tools/inventory.py` | Separar completitud estructural de cobertura y validación semántica. |
| `tests/run_tests.py` | Añadir regresiones, lectores instrumentados, compilación aislada y pruebas del build. |
| `INVENTORY.md`, `tests/inventory.json` | Regenerar con estados verificables, sin equiparar encabezados presentes con algoritmo validado. |
| `tests/simulation_report.md` | Corregir métricas y distinguir pruebas automáticas de simulación humana. |
| `README.md` | Versiones reales, alcance de pruebas, dominio de snippets y opciones del build. |
| `build_pdf.py`, `build_pdf.ps1` | Resolver los pendientes de pruebas y propagación de errores, sin cambios innecesarios. |
| `notebook_3col.typ` | Ajustes editoriales posteriores a las correcciones. |
| `body.typ`, `Notebook_ICPC_Final.pdf` | Regenerar desde el Markdown una vez validado; no corregirlos manualmente. |

Las pruebas deben extraer el código que se imprime. No mantener una copia corregida exclusivamente en los tests.

## 4. Estándar obligatorio de cada ficha

### 4.1 Orden editorial

1. Cuándo usar y problema que resuelve.
2. Cuándo no usar y alternativa.
3. Contrato, precondiciones y dependencias por nombre y sección.
4. Complejidad de la implementación impresa, incluida memoria e inicialización.
5. Formato de entrada: dimensiones, índices, operaciones y presencia o ausencia de `T`.
6. Entrada y salida literales, con significado de cada resultado.
7. Cómo leer la entrada mediante `cin`, `getline` o Python.
8. Implementación.
9. Cómo llamar a la API y producir la salida.
10. Trampas y adaptaciones: distinguir lo implementado de las extensiones sugeridas.

El driver se ensamblará en orden válido del lenguaje: dependencias y definiciones antes del `main` que usa lector y llamada. La presentación del lector antes de la implementación no significa que deba copiarse antes de declarar los tipos.

### 4.2 Contrato mínimo

- [ ] Índices, rangos inclusivos o semiabiertos y conversión desde el enunciado.
- [ ] Dominio de tamaños, valores, alfabetos y módulos.
- [ ] Límites de estados, sumas, productos y otros intermedios, no solo del resultado final.
- [ ] Estados vacío, sin solución, inalcanzable, infactible y no acotado cuando correspondan.
- [ ] Mutación de datos, reinicialización y posibilidad de llamadas repetidas.
- [ ] Condiciones de borrado, consulta de estructuras vacías y operaciones inválidas.
- [ ] Grafos dirigidos/no dirigidos, pesos permitidos, lazos y paralelas.
- [ ] Árbol o bosque requerido, raíz y valores en nodos o aristas.
- [ ] Precisión, finitud y degeneraciones en geometría.
- [ ] Versión mínima e importaciones en Python.

### 4.3 Reglas de los ejemplos

- [ ] El lector consume correctamente la entrada literal, sin `failbit` ni datos sobrantes inesperados.
- [ ] Los datos relevantes leídos llegan a la API; no se sustituyen por constantes.
- [ ] Las operaciones ejecutadas coinciden con el texto: suma frente a mínimo, asignación frente a incremento, prefijo frente a palabra exacta.
- [ ] No fijar dimensiones, nodos o posiciones que el formato no garantice.
- [ ] Comprobar estados de fracaso antes de reconstruir o indexar una solución.
- [ ] Para strings, no eliminar espacios o líneas vacías significativas mediante `ws` o `strip` sin justificarlo.
- [ ] Probar varios casos distintos para evitar coincidencias accidentales con valores hardcodeados.
- [ ] Definir el formato de altas, bajas y consultas en herramientas dinámicas.
- [ ] Mostrar un driver completo en integraciones de varias fases.
- [ ] Permitir agrupar métodos triviales de STL, pero no ocultar algoritmos independientes bajo un único ejemplo.

## 5. Fase 1 — Reparar la infraestructura de validación

**Prioridad: P0/P1.** Incorporar las regresiones antes de cambiar el comportamiento.

- [ ] **INFRA-01:** clasificar `Entrada` y `Salida` mediante sus etiquetas inmediatas o metadatos, no buscando palabras en un contexto amplio. En §6.2.5, «grado de salida» hace que la entrada se clasifique como salida.
- [ ] **INFRA-02:** incluir §1.1 como template con ejemplo ejecutable y §6.2.5 como algoritmo con entrada; ninguna ficha puede omitirse silenciosamente.
- [ ] **INFRA-03:** contrastar los IDs del inventario contra los realmente probados. Fallar ante faltantes, duplicados o fichas nuevas sin test; no aceptar solamente un mínimo de 160 fichas.
- [ ] **INFRA-04:** incorporar comprobación de `cin.fail()` y consumo de entrada al terminar la lectura correspondiente. Considerar las consultas que legítimamente se leen durante la llamada.
- [ ] **INFRA-05:** añadir pruebas Python que cambien la entrada y verifiquen que se usa, incluidos los casos hardcodeados de §12.1 y §12.4.
- [ ] **INFRA-06:** compilar cada ficha por separado con template, dependencias declaradas, lector y llamada. La autoinferencia de símbolos puede ayudar a diagnosticar dependencias, pero no suplir documentación faltante para aprobar.
- [ ] **INFRA-07:** parametrizar y ejecutar C++17, coherente con README. La suite actual usa C++20; conservar C++20 como comprobación adicional si se desea.
- [ ] **INFRA-08:** ampliar el inventario a cada algoritmo/API relevante: estructura, contrato, ejemplo, lector, llamada, caso límite y prueba diferencial.
- [ ] **INFRA-09:** separar estados `estructura_completa`, `ejemplo_ejecutado`, `contrato_revisado` y `regresiones_aprobadas`. Justificar cualquier `no_aplicable`.
- [ ] **INFRA-10:** conservar semillas, entrada completa, resultado esperado, resultado observado y código de salida en los fallos aleatorios.
- [ ] **INFRA-11:** mantener assertions de libstdc++ y detectar sanitizers realmente disponibles. No afirmar que UBSan corrió en el entorno actual.
- [ ] **INFRA-12:** sustituir la supuesta prueba del pipeline de Fase 7 por pruebas reales: actualmente revisa ancho y existencia de inventarios, no ejecuta el build.

**Cierre:** todas las fichas están contabilizadas y ninguna pasa por una salida accidental, por código ajeno incluido en el ensamblado conjunto o por una omisión del extractor.

## 6. Fase 2 — Corregir entradas, lectores y llamadas

**Prioridad: P0.** Referencias a `Notebook_ICPC_Final.md`.

| ID | Sección y líneas auditadas | Problema y acción requerida | Regresión / aceptación |
| --- | --- | --- | --- |
| IO-01 | §4.5.2, L2967–3048 | El formato anuncia actualización y consulta, pero el lector consume una matriz incompleta y la llamada consulta constantes. Elegir un formato único, leer la matriz u origen cero explícitos y ejecutar las operaciones leídas. Documentar el orden real de `query(x0,x1,y0,y1)`. | La entrada publicada no activa `cin.fail()`. Con matriz cero, asignar `(1,2)=15` y consultar todo devuelve `15`. |
| IO-02 | §4.4.1, L2691–2757 | Se anuncia suma y se implementa mínimo. Alinear texto y salida con mínimo; no cambiar globalmente `op` a suma porque HLD depende del mínimo. | Publicar `2 2` como mínimos; si se ofrece una variante de suma, demostrar `9 16` con su implementación correspondiente. |
| IO-03 | §4.6.1, L3257–3316 | `idx delta` se pasa a una API de asignación. Renombrar a `new_val` o implementar incremento y ajustar la salida. | Asignación produce `13`; incremento produce `16`. La ficha elige una sola semántica. |
| IO-04 | §5.5.1, L4317–4383 | Falta `count_exact`; la consulta de palabra vuelve a consultar prefijos. Implementar recorrido que devuelva `end_w` y usarlo. | El ejemplo publicado devuelve `2 0`; insertar `banana` y consultar exactamente `ba` devuelve `0`. |
| IO-05 | §6.1.1, L4870–4927 | El driver inicia siempre en `(1,1)` sin validar que esté abierta. Leer inicio válido o recorrer todas las componentes abiertas y leer consultas. | Grilla con filas `..` y `**`: la componente abierta tiene `2` adyacencias a paredes, no `3`. |
| IO-06 | §6.3.3, L5574–5619 | Floyd consulta siempre `d[1][3]`. Leer los pares consultados o imprimir la matriz completa. | `N=1, M=0` funciona sin accesos inválidos. |
| IO-07 | §7.6.3–§7.6.4, L8547–8551 y L8631–8633 | Gauss imprime dos incógnitas para cualquier `M`. Comprobar el estado y recorrer la dimensión real de la solución. | Entrada `1 1 / 1 2`: solución de una componente, sin acceder a `ans[1]`. Probar sistema incompatible y sistema indeterminado. |
| IO-08 | §7.9, L9295–9297 | Simplex indexa una solución vacía si es infactible/no acotado y supone dos variables. Distinguir estados y recorrer `x` solo si existe. | Probar una variable, dos variables, infactible y no acotado sin accesos inválidos. |
| IO-09 | §12.1, L12098–12136 | El lector fija `data=[10,20,30,40]`; `solve()` no implementa el ejemplo. Leer `N` y sus enteros y conectar la suma con esos datos. | `1 / 9` devuelve `9`, no `100`; conservar el caso de lectura en varias líneas. |
| IO-10 | §12.4, L12286–12312 | Se leen `p_in,q_in`, pero se usa `Fraction(3,6)`. Construir la fracción con los parámetros leídos y corregir el formato que dice no requerir entrada. | `1 2` y `3 6` producen fracciones equivalentes por datos reales; un tercer caso distinto cambia el resultado esperado. |
| IO-11 | §12.10, L12579–12618 | Entrada y lector vacíos; matriz hardcodeada. Leer dimensiones y celdas y mostrar la rotación sobre ellas. | Probar matrices `2x3`, `1x1` y rectangular distinta de la publicada. |
| IO-12 | §12.11, L12629–12660 | Entrada y lector vacíos; conjuntos hardcodeados. Leer tamaños y elementos, construir los conjuntos y operar sobre ellos. | Probar duplicados, conjunto vacío e intersección vacía. |
| IO-13 | §6.1.3, §6.3.1 y §6.3.2 | Definir salida para inalcanzables y ausencia de ciclo. No imprimir `INF` como una distancia ordinaria ni `sz(cyc)-1` cuando el vector está vacío. | Destino inalcanzable y Bellman-Ford sin ciclo tienen salida explícita; no aparece longitud `-1`. |
| IO-14 | §6.5.1, §6.5.4 y §6.9 | Kahn puede imprimir un orden parcial, 2-SAT imprime asignación aun siendo insatisfacible y Euler no documenta su salida vacía. Condicionar la reconstrucción al estado. | Ciclo dirigido más nodo aislado, fórmula insatisfacible y grafo no euleriano se reportan sin fingir una solución. |
| IO-15 | §1.3, L302–304 y L378–383 | Se indexa `words[1]` sin garantizar dos palabras; `getline(cin >> ws,...)` elimina espacios y líneas vacías. Restringir y explicar el dominio o hacer el ejemplo seguro. | Una sola palabra no provoca acceso fuera de rango; probar líneas con espacios iniciales si se admiten. |

- [ ] Implementar y probar **IO-01 a IO-15**.
- [ ] En la adaptación de §4.5.2, eliminar `t[x+n][y+m] += val`: el miembro es `st` y modificar solo una hoja no actualiza agregados. Para incremento, llamar `update` con el valor nuevo correcto.

**Cierre:** formato, entrada, lectura, llamada y salida describen y ejecutan la misma operación para todos los ejemplos.

## 7. Fase 3 — Corregir fallos algorítmicos bloqueantes

### 7.1 Simplex: anti-ciclado y estados

- [ ] **ALG-01 — §7.9, L9165 y L9230–9249:** implementar realmente Bland o una regla lexicográfica completa. Elegir el coeficiente más negativo y desempatar por ID no es Bland.
- [ ] Registrar como regresión el caso clásico de Beale. La implementación auditada repite la base tras seis pivotes y no termina; el óptimo esperado es `1`, con solución factible `(1,0,1,0)`.

```text
3 4
0.5 -5.5 -2.5 9
0.5 -1.5 -0.5 1
1 0 0 0
0 0 1
10 -57 -9 -24
```

- [ ] Mantener las regresiones de Fase 1, óptimo negativo, infactibilidad y no acotación.
- [ ] Resolver conjuntamente **IO-08**, sin indexar soluciones inexistentes.

### 7.2 Bellman-Ford: saturación y ciclos negativos

- [ ] **ALG-02 — §6.3.2, L5493 y L5537–5546:** no saturar las distancias usadas para decidir si persisten relajaciones sin una justificación de rango válida. Separar inalcanzabilidad y representación numérica; usar intermedios ensanchados y límites derivados del dominio.
- [ ] Esta entrada debe detectar el ciclo negativo alcanzable en `2`; actualmente devuelve `false`:

```text
3 2 1
1 2 0
2 2 -1000000000000000000
```

- [ ] Probar además aristas `(1,2,-10^18)` y `(2,2,-9·10^18)`: la suma en `ll` no debe desbordar antes de aplicar un límite.
- [ ] Conservar la regresión que evita contar una asignación saturada sin cambio como relajación real. No arreglar el falso negativo reintroduciendo falsos positivos.
- [ ] Probar ciclo alcanzable, ciclo no alcanzable, grafo acíclico con pesos negativos y reutilización del vector de ciclo.

### 7.3 Half-plane intersection: orden, degeneraciones y recorte

- [ ] **ALG-03 — §9.3, L11365–11368:** reemplazar el comparador aproximado por un orden débil estricto. Resolver agrupación de direcciones y tolerancias fuera del comparador.
- [ ] Probar transitividad con semiplanos definidos por `A: (0,0)->(1,0)`, `B: (0,1)->(1,1+0.75e-9)` y `C: (0,2)->(1,2+1.5e-9)`. Actualmente se cumple `A<C`, `C<B` y `B<A`.
- [ ] **ALG-04:** hacer coincidir la salida degenerada con el contrato: la intersección de `y>=0` e `y<=0` devuelve dos puntos, no `{}`.
- [ ] **ALG-05:** declarar que `box` recorta todas las regiones, incluidas las acotadas; o rediseñar la API para distinguir intersección real de intersección con caja.
- [ ] Probar el cuadrado `[2e9,2e9+1] x [0,1]`: su área real es `1` y el recorte predeterminado actual devuelve `0`.
- [ ] Definir semiplanos con extremos iguales, opuestos, paralelos, región vacía, punto, segmento y región no acotada. No confundir factibilidad con tener área positiva dentro de una caja arbitraria.

### 7.4 LCA y flujos

- [ ] **ALG-06 — §6.4.3, L5827–5830:** no ignorar los bits altos de `kth_ancestor`. Si el contrato satura en la raíz, tratar `k>=dep[u]` antes del lifting; exigir `k>=0` y definir su tipo máximo.
- [ ] Regresión: árbol `1—2`, raíz `1`, `kth_ancestor(2,4)` debe devolver `1`, no `2`.
- [ ] **ALG-07 — §6.6.1 y §6.6.2:** exigir y comprobar `s!=t`, o definir ese caso. Con `N=1,M=0,s=t=1`, Dinic no termina y MCMF devuelve flujo ficticio `10^18`.
- [ ] **ALG-08 — §6.6.1, §6.6.2 y §6.10:** corregir los índices reversos para `u==v` o descartar lazos según un contrato explícito. La arista directa de un lazo no puede apuntarse a sí misma como reversa.
- [ ] Probar lazos, paralelas y aristas antiparalelas; conservar diferenciales de flujo contra cortes exhaustivos y costos contra enumeración pequeña.
- [ ] Documentar ciclos negativos excluidos por MCMF/SPFA y no afirmar que los resuelve. Definir el número de aumentos en su complejidad.

### 7.5 Otros fallos lógicos y de seguridad

- [ ] **ALG-09 — §7.3.4, L7982–7989:** tratar `(a,b)=(0,0)` antes de `c%g`. Definir el resultado para `c=0` y `c!=0`; la conducta debe coincidir con el contrato, sin división por cero.
- [ ] **ALG-10 — §7.11.1, L9462:** normalizar la abscisa o comprobar `0<=x<n` antes de indexar. Para `y={0,1}`, `x=-1`, el resultado módulo `1e9+7` es `1000000006`.
- [ ] **ALG-11 — §6.3.3:** soportar de forma segura la detección de ciclos negativos de Floyd o restringir claramente el dominio. Un grafo dirigido completo de 30 nodos, sin lazos, con pesos `-1`, desborda `ll` durante las relajaciones. Marcar los pares con distancia `-infinito` si se ofrece esa semántica.
- [ ] **ALG-12 — §3.6.3:** corregir el comparador de Smith para tareas `(t,w)=(0,0)` o exigir duraciones positivas. Con tareas `(id,t,w)=(1,1,1),(2,0,0),(3,1,2)` aparecen comparaciones cíclicas; una ordenación puede dar coste `5` cuando existe coste `4`.
- [ ] **ALG-13 — §4.4.1, L2736–2738:** recomponer siempre `op(hijo_izquierdo,hijo_derecho)` en `update`, o restringir la plantilla a operaciones conmutativas. No basta con preservar el orden en `query` para ofrecer cualquier monoide.

**Cierre:** todos los contraejemplos tienen regresión permanente y la suite termina sin resultados incorrectos ni comportamiento indefinido dentro del contrato.

## 8. Fase 4 — Seguridad numérica y dominios reales

**Prioridad: P0 cuando existe UB o resultado incorrecto dentro del dominio anunciado; P1 para cerrar contratos.** No añadir restricciones artificiales solo para esconder un fallo: elegir límites apropiados para la herramienta y reflejarlos en ejemplos y pruebas.

### 8.1 Centinelas y alcanzabilidad

- [ ] **NUM-01:** auditar cada uso de `INF=1e18`. Una distancia o coste finito igual o mayor que el centinela no puede confundirse con inalcanzabilidad.
- [ ] Probar Dijkstra con una arista de coste `1e18`; actualmente no reconstruye el camino.
- [ ] Probar `IterSegTree`, Lazy y HLD con valores mayores que `1e18` si se admiten. El neutro de mínimo debe ser identidad en todo el dominio.
- [ ] Probar Húngaro y subset assignment sobre una matriz `1x1` de coste `2e18`.
- [ ] Probar TSP de dos nodos con ambas aristas `6e17`: coste total `1.2e18`, con ruta válida.
- [ ] Probar D&C con un elemento `1100000000`, un grupo y coste suma al cuadrado: `1.21e18`.
- [ ] Probar Knuth con dos pesos `6e17`: coste `1.2e18`.
- [ ] Probar closest pair con `(0,0)` y `(2e18,0)`: distancia `2e18`, no `1e18`.
- [ ] Separar alcanzabilidad y valor cuando corresponda; proteger sumas y justificar límites de todos los estados, no solamente del óptimo final.

### 8.2 Enteros, búsquedas y prefijos

- [ ] **NUM-02 — §1.1:** imprimir correctamente el mínimo de `i128` sin negarlo en el tipo signed; obtener su magnitud unsigned.
- [ ] **NUM-03 — §1.5:** manejar `decimal_to_base(LLONG_MIN,10)` mediante magnitud unsigned.
- [ ] **NUM-04 — §1.5, L573–588:** ensanchar `rem*10+digito` antes de operar en división y módulo de strings gigantes.
- [ ] Regresión: `s="10000000000000000000"`, `d=9223372036854775807` produce cociente `1` y resto `776627963145224193`.
- [ ] **NUM-05 — §1.1:** documentar y comprobar paso positivo, finitud y rango de `llround(x/step)` en `quantize_eps`. Con paso `1e-9`, `x=1e10` ya excede `ll` al cuantizar.
- [ ] **NUM-06 — §3.1.1:** evitar overflow de centinelas, diferencias, `mid+1` y `mid-1`; o acotar explícitamente el intervalo. `bs_max(LLONG_MAX,LLONG_MAX,siempre_true)` debe terminar correctamente si se admite ese extremo.
- [ ] **NUM-07 — §3.1.2:** ajustar dominio e intervalo del ejemplo de raíz. Con `X=1e60`, 90 bisecciones de `[0,X]` devuelven `0` en la llamada actual, no `1e30`. No prometer precisión absoluta independiente de la escala ni reducción efectiva tras estancarse la representación.
- [ ] **NUM-08 — §3.3.1:** ensanchar la suma de ventana o acotar sus intermedios. `a={LLONG_MAX,1}`, límite `LLONG_MAX`: longitud esperada `1`, no `2`.
- [ ] **NUM-09 — §3.6.1:** representar «todavía no se eligió un intervalo» sin `last_r=-INF`. Un único intervalo `[-1e18,-1e18+1]` debe poder elegirse en modo estricto.
- [ ] **NUM-10 — §1.4:** no contar submáscaras en `int` ni enumerarlas para un dominio irrestricto de 64 bits. Para solo obtener la cantidad, calcularla según `popcount` con un tipo adecuado y documentar el dominio.

### 8.3 CRT, diofánticas y aritmética modular

- [ ] **NUM-11 — §7.3.5, L8042:** normalizar residuos sin sumar incondicionalmente el módulo en `ll`.
- [ ] Regresión CRT: una congruencia con residuo `8000000000000000000` y módulo `9000000000000000000` debe conservar ambos valores.
- [ ] **NUM-12 — §7.3.5, L8051–8055:** comprobar el rango del MCM antes de convertir de `i128` a `ll`; definir error o contrato para resultados no representables. Exigir módulos positivos.
- [ ] **NUM-13 — §7.3.4:** escalar soluciones diofánticas en `i128` y comprobar su representabilidad. La entrada `(3,5,9000000000000000000)` no puede devolver `true` con coeficientes que incumplan la ecuación.
- [ ] **NUM-14 — §7.6.4 y §7.10:** ensanchar productos/acumulaciones de Gauss modular y BM, o declarar una cota de módulo segura. No heredan la protección de `mul_mod` si hacen productos directos en `ll`.
- [ ] Con primo `4294967291`, Gauss debe resolver `(p-1)x=p-1` con `x=1`; BM sobre términos constantes `p-1` debe conservar ese valor al extrapolar.
- [ ] **NUM-15 — §7.6.8:** normalizar base y objetivo, exigir `m>0` y proteger productos de BSGS, incluido `k*a/g`.
- [ ] Regresiones BSGS: `(10000000017,4,10000000019)` devuelve `2`; `(-2,2,5)` devuelve `3` si se admiten bases negativas.
- [ ] **NUM-16 — §7.4.1:** exigir en Fermat módulo primo y `a % m != 0`; documentar explícitamente cuándo no existe inverso.
- [ ] **NUM-17 — §7.3.1:** ensanchar diferencias y sumas antes de `abs`/GCD. Para `a={-9e18,9e18}`, `x=0`, el resultado correcto es `9e18`.

### 8.4 Bits, factorización y otras operaciones

- [ ] **NUM-18 — §7.6.2:** hacer coincidir el dominio de XOR Basis con sus bits recorridos. Si se ofrecen 62 bits no negativos, incluir `61..0`. Insertar `2^61` debe dar rango `1` y máximo `2^61`.
- [ ] **NUM-19 — §5.5.2:** unificar el contrato de BitTrie: código con bit superior `60`, texto con `B=30`. Declarar `0<=x<2^61` o ampliar hasta el bit requerido; no truncar silenciosamente.
- [ ] **NUM-20 — §7.2.2:** elegir entre limitar claramente Pollard a `N<=1e18` o soportar realmente todo `u64`. Para todo `u64`, usar suma `u128` en `pollard_f` y distribución unsigned, sin convertir límites a `ll`.
- [ ] Probar `pollard_f(3000000000,9999999999999999999,10000000000000000001)`: esperado `8999999999999999998`.
- [ ] Hacer coincidir `factorize_64(n)` con su dominio para `n<=1`; actualmente `0` puede recursar indefinidamente aunque la documentación diga que no agrega factores.
- [ ] **NUM-21 — §8.4.1–§8.4.2:** ensanchar antes de restar interceptos y pendientes en ambos CHT, proteger evaluaciones y revisar breakpoints extremos.
- [ ] Regresión CHT: rectas `(m,b)=(3,9e18),(2,-9e18),(1,9e18)` y consulta `0`: mínimo `-9e18`, máximo `9e18`.
- [ ] Probar además CHT dinámico con rectas `(0,0)` y `(0,1)` en `LLONG_MIN`: máximo `1`, no `0`.
- [ ] **NUM-22 — §7.6.7:** proteger intermedios de FWT, no solo la salida. `A={3e9,3e9}`, `B={1e9,1e9}` debe dar `{6e18,6e18}` aunque un producto transformado alcance `12e18`.
- [ ] **NUM-23 — §7.6.9:** usar reducción de factores y/o intermedios ensanchados en `Frac`. `1/4000000000 + 1/4000000000` debe dar `1/2000000000`.
- [ ] **NUM-24 — §9.1.3:** declarar límites de coordenadas y acumulación en el kernel entero. Si se pretende admitir todo `ll`, ensanchar también antes de restar coordenadas en `cross128(a,b,c)`.

## 9. Fase 5 — Alinear contratos, API, reinicialización y complejidad

### 9.1 Estructuras y capacidades inexistentes

- [ ] **DOC-01 — §4.8.2:** completar una consulta concreta de Sack y guardar las respuestas reales. No anunciar `ans_sack` si no existe ni usar el hijo pesado como sustituto de una respuesta de subárbol.
- [ ] Reiniciar `cnt_s` correctamente y unificar `keep=true/false`. Repetir el ejemplo debe volver a producir `2 3`, no `4 3`.
- [ ] Corregir nombres inexistentes de sus advertencias (`big_c`, `timer_s`, `cnt_col`) y declarar `n<MAX_SACK` y el dominio de colores, o comprimirlos.
- [ ] **DOC-02 — §4.9:** limitar Link-Cut Tree a `link`, `cut` y conectividad, o implementar valores, agregados, actualización y `query_path` con pruebas. No dejar promesas de sumas sin código.
- [ ] **DOC-03 — §6.8:** documentar la representación comprimida real de Block-Cut Tree: nodos de articulación, nodos de bloque y mapeo `bct_id[u]`.
- [ ] Para el grafo `1—2`, explicar por qué `bct_nodes=1` y ambos vértices mapean al mismo bloque; no anunciar tres nodos.
- [ ] Mostrar consultas entre `bct_id[u]` y `bct_id[v]`, comprobar componentes en grafos desconectados y llamar bosque a la salida cuando corresponda.
- [ ] **DOC-04 — §5.5.3:** corregir `insert(s,id)` anunciado por Aho: la implementación ofrece `insert(s)`, sin identificación individual de patrones.
- [ ] **DOC-05 — §5.6.2:** retirar `propagate_occurrences()` inexistente, describir transiciones mediante arreglo de 26 letras y propagación ya realizada en el constructor. Alinear la construcción total con su `sort`: `O(N log N)`.
- [ ] **DOC-06 — §5.6.3:** corregir Eertree: raíces `0` y `1`, nodos reales desde `2`, miembro `st` y cantidad `sz(st)-2`. No recomendar propagar de nuevo frecuencias ya propagadas.
- [ ] Regresión Eertree: `a` tiene un palíndromo distinto; en `aa`, la frecuencia de `a` sigue siendo `2`, no `3` por doble propagación.
- [ ] **DOC-07 — §6.4.1:** restringir el diámetro publicado a árboles sin pesos o implementar lectura y acumulación de pesos. El código actual cuenta aristas.
- [ ] **DOC-08:** retirar Prim de §6.4 si solo se conserva Kruskal, Treap BST de §4.7 si solo se incluye implícito y Unbounded Knapsack del encabezado si únicamente se describe como adaptación.

### 9.2 Dominio, estado y llamadas repetidas

- [ ] **DOC-09 — §5.6.2:** exigir `'a'..'z'` tanto en SAM como en su texto de consulta, o ampliar las transiciones. `SuffixAutomaton("A")` no debe causar acceso inválido en un dominio anunciado como genérico.
- [ ] **DOC-10 — §5.6.1:** convertir bytes desde `unsigned char` y ajustar alfabeto/centinela de Suffix Array, o restringir a ASCII. Probar byte `128`, que actualmente puede indexar `ws[-128]`.
- [ ] **DOC-11:** definir patrones vacíos en KMP/autómata KMP y texto vacío en longest common substring de SAM. Las API deben devolver un resultado definido o declarar una precondición explícita.
- [ ] **DOC-12:** declarar `N>=1` o soportar vacío en PersistentST y Mo; no desreferenciar `a[0]` ni `max_element` de un vector vacío.
- [ ] **DOC-13 — §2.1:** exigir presencia en `CoordComp::get_id` o documentar posición de inserción `[0,K]`. Comprimir `{10}` y consultar `11` devuelve `K`, no un índice válido de elemento.
- [ ] **DOC-14 — §4.1.2:** declarar `DynCon::solve()` de un solo uso o hacerlo repetible. Actualmente acumula respuestas y modifica cierres. Declarar altas repetidas, borrados inexistentes y paralelas.
- [ ] **DOC-15:** aclarar si Kuhn/Blossom devuelven cardinalidad total o incremento al reutilizar el objeto. `max_matching()` después de `min_vertex_cover()` y una segunda llamada a `Blossom::solve()` no deben contradecir el contrato.
- [ ] **DOC-16 — §6.4.2:** limpiar `mst` al comenzar Kruskal o exigir un vector vacío; dos llamadas no deben duplicar aristas de salida accidentalmente.
- [ ] **DOC-17 — §3.4.1:** documentar la semántica del constructor de prefijos para strings. Para contar pares adyacentes iguales dentro de la subcadena `[l,r]`, la API actual requiere ajustar el extremo; una subcadena de longitud uno tiene respuesta `0`.
- [ ] **DOC-18:** declarar profundidades de DFS y evitar depender de la pila predeterminada en árboles camino grandes. HLD/Sack fallaron con 100 000 nodos y LCA/grafo funcional con 200 000 en Windows.
- [ ] Preferir recorridos iterativos donde sea viable. En grafo funcional, resolver las colas en orden inverso al de eliminación de Kahn.
- [ ] **DOC-19 — §8.1.5:** limitar Digit DP al dominio real del lector `ll` y memo de 20 posiciones, con cota de conteo compatible; no prometer `10^5` dígitos. Exigir `K>0` y probar `K=0` como entrada inválida definida.
- [ ] **DOC-20 — §8.4.3–§8.4.4:** exigir pesos no negativos en los ejemplos concretos de D&C con suma al cuadrado y Knuth con suma de intervalo, o demostrar las hipótesis para otro dominio.
- [ ] Regresiones de dominio: D&C `N=4,K=2,a={-1,-2,-2,2}` da actualmente `9` frente a óptimo `5`; Knuth `{-2,-2,-1,-2}` da `-14` frente a `-16`. Rechazar esos casos si quedan fuera del contrato, no presentarlos como resueltos por la optimización.
- [ ] **DOC-21 — §7.4.2:** separar parámetros y lector de factoriales tabulados y Lucas. Exigir `0<=max_n<MAX_F`, primo y `max_n<mod`; asociar las tablas al módulo usado.
- [ ] No anunciar `n,r<=1e18` con un lector `int` ni intentar `init_fact(n)` para ese dominio. `init_fact(7,7); nCr(7,0,7)` debe evitar la inversión de factorial nulo.

### 9.3 Complejidades, firmas y adaptaciones

- [ ] **DOC-22 — §3.4.3:** corregir `count_subarrays_div_m`: el vector de tamaño `M` cuesta `O(N+M)` tiempo y `O(M)` memoria. Usar hash si se pretende admitir `M` grande con pocos elementos.
- [ ] **DOC-23 — §6.8:** ajustar la cota de Block-Cut Tree por la ordenación usada para deduplicar extremos de bloques, o reemplazarla por marcas lineales.
- [ ] **DOC-24 — §6.6.2:** definir `A` como número de aumentos y publicar una cota compatible con SPFA, por ejemplo `O(A·V·E)`; con capacidades enteras, explicar `A<=F`.
- [ ] **DOC-25 — §3.6.3:** corregir el retorno anunciado de Smith: se obtiene `vector<JobEx>`, no `{coste,indices}`, salvo que se implemente ese retorno.
- [ ] **DOC-26 — §3.7.1:** corregir el retorno anunciado de sweep-line: entrega un entero por convención, no `{maximo,coordenada}`. La salida `3 2` compara extremos cerrados y semiabiertos; `2` no es una coordenada.
- [ ] **DOC-27 — §6.7:** aclarar que Directed MST usa un DSU local y devuelve vértices padre, no IDs `parent_edge`.
- [ ] **DOC-28 — §5.4.2:** describir la transformación real de Manacher, de tamaño `2N+1` con `#`, no una variante `2N+3` con centinelas inexistentes.
- [ ] **DOC-29 — §7.2.2:** no recomendar `sort(all(fac))` sobre `map<ull,int>`; ya está ordenado y no admite ese `sort`.
- [ ] **DOC-30 — §10.3:** retirar garantías generales de ejecución extremadamente rápida hasta 55–60 nodos; describir límites prácticos y considerar poda por coloreo solo si es necesaria y probada.
- [ ] Mantener correctas las distinciones `O(N·2^N)` de asignación, `O(N²·2^N)` de TSP y `O(3^N)` de enumeración total de submáscaras, con presupuesto real de memoria.
- [ ] Mantener las hipótesis corregidas de WQS/Aliens, Pick, Sprague–Grundy y rolling hash; no reintroducir las afirmaciones erróneas anteriores al compactar.

**Cierre:** cada capacidad anunciada corresponde a una API existente, una variante diferenciada o una adaptación marcada como tal.

## 10. Fase 6 — Python y cobertura individual de algoritmos

### 10.1 Compatibilidad y contratos Python

- [ ] **PY-01 — README y §12.1:** elegir Python mínimo 3.11 o proteger `sys.set_int_max_str_digits` con detección de disponibilidad para mantener compatibilidad 3.10. No anunciar 3.10 mientras se llame incondicionalmente a una API de 3.11.
- [ ] **PY-02 — §12.2:** implementar lectura de bases de origen `37..62` o restringir explícitamente `base_a<=36`, manteniendo salida hasta 62. `base_a_to_b("z",62,10)` no funciona con `int(s,62)`.
- [ ] Probar cero, negativos, caracteres inválidos y extremos de bases; definir el alfabeto exacto.
- [ ] **PY-03 — §12.6:** exigir pesos positivos o corregir la condición `rem_weight==0` si se admiten pesos cero. `N=1,W=0,peso=0,valor=5` debería producir `5` en ese dominio, no `0`.
- [ ] **PY-04 — §12.14:** corregir el contrato de `math.comb`: para `k<0` lanza `ValueError`, no devuelve `0`; para `k>n`, con parámetros no negativos válidos, devuelve `0`.
- [ ] **PY-05 — §12.8:** definir cadena no vacía antes de `most_common(1)[0]`, o soportar entrada vacía. Aclarar si espacios iniciales/finales forman parte de la cadena.
- [ ] **PY-06:** mantener las correcciones de truncamiento entero y reemplazo de barras, y añadir pruebas con BigInt y expresiones que ya contengan `//`.
- [ ] Aclarar que `eval` solo es apropiado para expresiones confiables del dominio previsto, no como parser de texto arbitrario no confiable.

### 10.2 Ejemplos independientes pendientes

- [ ] **COV-01 — §3.1.1:** ejemplo real de `bs_min`, incluidos ausencia de solución y límites; no probar únicamente `bs_max`.
- [ ] **COV-02 — §3.4.3:** leer y llamar `count_subarrays_xor_k` con el tipo correcto. Caso mínimo `{1,1}`, `K=0`: una subcadena de XOR cero.
- [ ] **COV-03 — §3.5.1:** ejemplo de `apply_double_diff`: arreglo base, lista de operaciones y rangos de operaciones, con su reserva e indexación 1-based.
- [ ] **COV-04 — §1.5:** ejemplos de `decimal_to_base` y `to_binary_string`. Aclarar que la segunda representación actual es LSB-first, si se conserva.
- [ ] **COV-05 — §3.4.1:** ejemplo del constructor de prefijos para strings que demuestre las consultas internas de la subcadena.
- [ ] **COV-06 — §7.6.1:** lectura de matriz y exponente y llamada a `mat_pow`; el ejemplo de `fib_fast` no valida la API matricial.
- [ ] **COV-07 — §7.4.1:** demostrar `binpow` e inverso de Fermat con sus condiciones, además de resta e inverso por Euclides.
- [ ] **COV-08 — §7.4.2:** ejemplo de `nPr` y lector independiente para el dominio real de Lucas.
- [ ] **COV-09 — §6.4.3:** lectura de `k` y llamada a `kth_ancestor`.
- [ ] **COV-10 — §5.5.2:** formato de inserción, borrado y consulta para BitTrie dinámico.
- [ ] **COV-11 — §6.5.4:** lectura general de cláusulas 2-SAT y mapeo de literales. Explicar que `add_imp` recibe nodos codificados, a diferencia de `add_or`.
- [ ] **COV-12 — §4.7:** si se anuncia cortar/pegar con Treap implícito, mostrar la composición de `split` y `merge`, con sus posiciones.
- [ ] **COV-13:** revisar el resto de algoritmos agrupados y marcar cada uno como implementado con ejemplo, adaptación explícita o capacidad retirada. No exigir fichas extensas para cada método trivial de STL.

**Cierre:** el inventario deja de confundir número de subsecciones con cobertura completa de algoritmos y operaciones.

## 11. Fase 7 — Navegación, build, PDF y documentación

### 11.1 Navegación y consistencia editorial

- [ ] Actualizar guía inicial, títulos y referencias según los dominios corregidos de cada algoritmo.
- [ ] Mantener la distinción entre mediana inferior/superior, condiciones de sliding window y elección LCA/Euler/HLD.
- [ ] Corregir el checklist de overflow: `if(dist[u]<INF)` no protege por sí solo `dist[u]+w`; usar comprobaciones o ensanchamiento reales.
- [ ] No presentar `INF=1e18` como universalmente suficiente para cualquier `ll`.
- [ ] Mantener índice automático con páginas y enlaces a fichas reales.
- [ ] Revisar firmas, nombres de miembros y recomendaciones copiables: no dejar variables inexistentes en las adaptaciones.

### 11.2 Build reproducible

- [x] Preprocesamiento del Markdown en memoria, sin sobrescribir el original, en el build comprobado.
- [x] Parámetro `body` transmitido a la plantilla en el build temporal comprobado.
- [x] Compilación desde otro directorio con rutas absolutas y salida temporal, conservando los originales.
- [ ] Probar `--no-pandoc` con Pandoc ausente y body existente, y su fallo cuando el body no existe.
- [ ] Probar `--body` y `--output` personalizados con rutas relativas, espacios y archivos nuevos; documentar una base de resolución inequívoca.
- [ ] Probar `--watch` con cambios de Markdown, plantilla y body según modo; comprobar que reconvierte Markdown cuando corresponde.
- [ ] Definir y comprobar cómo propaga errores `--watch`. La llamada actual ignora el retorno de `compile_typst_once`; no presentar esa propagación como resuelta sin prueba.
- [ ] Probar fallos de Pandoc, de Typst y PDF de destino bloqueado; preservar siempre la última salida válida y limpiar temporales.
- [ ] Probar realmente `build_pdf.ps1`, incluidos `$PSScriptRoot`, paso de argumentos y código de salida; su presencia en el código no sustituye su ejecución.
- [ ] Automatizar hashes antes/después y ejecución desde un directorio temporal para prevenir regresiones de no destructividad.

### 11.3 PDF y reportes

- [ ] Actualizar `tests/simulation_report.md`: la regeneración actual tiene **53 páginas**, no 55. Volver a medir después de los cambios; no fijar ese número como expectativa permanente.
- [ ] Actualizar README y reportes con el número real de fichas, ejemplos seleccionados y ejemplos ejecutados, obtenidos de resultados y no de cadenas hardcodeadas.
- [ ] No llamar «simulación de competencia» a una colección de pruebas automáticas; separar ambas evidencias.
- [ ] Regenerar `body.typ` y PDF únicamente desde el Markdown corregido.
- [ ] Conservar el objetivo de líneas de código de hasta 60 caracteres sin introducir cortes que cambien semántica ni formatos Python innecesariamente ilegibles.
- [ ] No reducir la fuente de código por debajo de los 5,9 pt actuales para ocultar crecimiento.
- [ ] Revisar visualmente código, fórmulas, tablas, colores, cortes de columnas y páginas, encabezados huérfanos y última página.
- [ ] Comprobar legibilidad al 100 % y en escala de grises, especialmente lectores y llamadas.
- [ ] Acordar un presupuesto de páginas antes de ampliar repertorio. Las 53 páginas observadas no son un límite oficial de competencia.

## 12. Matriz de regresiones y pruebas diferenciales

### 12.1 Regresiones deterministas obligatorias

| Grupo | Casos mínimos y resultado exigido |
| --- | --- |
| Regresiones históricas | BM índice 4 → 3; Push-Relabel original → flujo 1; puentes paralelos → 0; CRT original; duplicados en closest pair; punto más triángulo en Minkowski. |
| Lectores | Todos los ejemplos sin fallo de lectura ni datos sobrantes inesperados; casos IO-01 a IO-15 y entradas alternativas contra constantes accidentales. |
| Simplex | Beale termina con óptimo 1; una variable; óptimo negativo; infactible; no acotado; impresión sin indexar solución vacía. |
| Bellman-Ford | Lazo negativo que satura, pesos grandes, ciclo no alcanzable y grafo sin ciclo; sin falsos positivos ni negativos por clamp. |
| Semiplanos | Transitividad del comparador, paralelos, opuestos, vacío, punto, segmento, no acotado y polígono fuera de la caja predeterminada. |
| LCA | Árbol de un nodo, `k=0`, `k=dep[u]`, `k` con bits por encima de `LOG` y consultas grandes admitidas. |
| Flujos | `s=t`, lazos, paralelas, antiparalelas, capacidad cero y capacidades extremas dentro del contrato. |
| CRT/diofánticas | Normalización con residuo y módulo cercanos a `LLONG_MAX`; MCM no representable; módulos inválidos; `(0,0,c)`; escalado grande. |
| Modular | Primo `4294967291` si se admite; residuos negativos, cero no invertible y módulos compuestos cuando corresponda. |
| Bits | XOR Basis y BitTrie en el bit superior prometido; cero, duplicados y borrado válido. |
| Numéricos | `LLONG_MIN`/mínimo `i128`, división string gigante, BSGS grande, Lagrange con abscisa negativa y centinelas frente a valores finitos grandes. |
| CHT/FWT/Frac | Contraejemplos NUM-21 a NUM-23, incluyendo intermedios que no caben aunque el resultado final sí. |
| Reinicialización | Dos ejecuciones de Sack, DynCon, matching y Kruskal según la semántica documentada. |
| Strings | Patrón vacío, texto vacío, alfabeto inválido y bytes altos según dominio; palabra exacta frente a prefijo; frecuencias de Eertree sin doble propagación. |
| Dominios DP | Pesos negativos excluidos o soportados explícitamente; `K=0`; factorial nulo módulo primo; límites de memoria de máscaras. |
| Python | Entrada variable en template/Fraction/matrices/sets; bases 37–62 según contrato; `comb` con `k<0`; pesos cero según contrato; versión mínima. |
| Pila | Árboles camino grandes y colas funcionales largas dentro de las restricciones publicadas, con solución iterativa o requisitos de entorno explícitos. |
| Build | Directorio distinto, hashes, sin Pandoc, body personalizado, fallos, archivo bloqueado y modo watch. |

### 12.2 Oráculos sencillos

| Familia | Referencia independiente |
| --- | --- |
| Fenwick, segmento, lazy, SQRT y Treap | Vector o matriz y operaciones directas. |
| Conectividad dinámica y LCT de bosques | Reconstruir componentes con BFS en casos pequeños. |
| LCA, HLD, Sack y rerooting | Recorridos explícitos de caminos y subárboles. |
| Flujo máximo | Enumerar cortes en redes pequeñas. |
| MCMF y asignación | Enumeración de flujos/asignaciones pequeñas respetando el dominio. |
| Matching y 2-SAT | Enumeración exhaustiva de soluciones pequeñas. |
| BM y recurrencias | Simulación directa de recurrencias conocidas y generadas. |
| CRT y modular | Enteros arbitrarios de Python y verificación de cada congruencia. |
| CHT | Evaluar todas las rectas en cada consulta. |
| D&C y Knuth | DP cuadrática o cúbica sin optimización en instancias pequeñas. |
| Closest pair | Todos los pares. |
| Minkowski | Hull de todas las sumas de vértices. |
| Semiplanos | Recorte poligonal independiente en casos acotados y pruebas separadas de factibilidad/degeneración. |
| Strings y prefijos | Comparación directa y enumeración de subcadenas/palíndromos. |
| Simplex | Instancias con óptimo conocido, enumeración de vértices pequeños y regresiones de degeneración/anti-ciclado. |

No usar exclusivamente otro algoritmo del notebook como oráculo si ambos pueden compartir el mismo error.

## 13. Secuencia concreta de entrega

1. **Infraestructura:** corregir omisiones, instrumentar lectura y añadir los contraejemplos antes de cambiar implementaciones.
2. **Entrada/salida:** resolver IO-01 a IO-15 y los ejemplos Python que ignoran entrada.
3. **Bloqueantes:** Simplex, Bellman-Ford, semiplanos, LCA, flujos, diofánticas, Floyd y comparadores de Smith/segmento adaptable.
4. **Seguridad numérica:** centinelas, CRT, modular, BSGS, bits, Pollard, CHT, strings gigantes, FWT y fracciones.
5. **Contratos e integración:** Sack, LCT, Block-Cut Tree, strings, dominios DP, reinicialización y profundidad de pila.
6. **Cobertura:** ejemplos por algoritmo/API e inventario que refleje pruebas efectivas, no solo encabezados.
7. **Cierre operativo:** README, reportes, build, PDF y simulación real.

Cada entrega debe enumerar IDs resueltos, pruebas ejecutadas y pendientes. No marcar una fase completa solo porque compila el PDF o pasa la suite anterior.

No incorporar nuevas estructuras complejas —Beats, CDQ, parallel binary search o variantes adicionales de Mo— antes de cerrar los errores identificados, salvo necesidad concreta del equipo.

## 14. Definición de terminado

- [ ] Todas las regresiones históricas y nuevas pasan.
- [ ] No quedan pendientes P0 sin una decisión explícita de retirar la capacidad afectada.
- [ ] Cada algoritmo/API conservado tiene contrato real y ejemplo propio o agrupación justificada.
- [ ] Todos los lectores consumen su entrada correctamente y los datos relevantes se utilizan.
- [ ] Todas las llamadas manejan dimensiones, estados sin solución y reinicialización.
- [ ] Los 149 ejemplos C++ actuales y los 15 Python están contabilizados y ejecutados; si cambia el inventario, se valida el conjunto real de IDs.
- [ ] Cada driver C++ compila aisladamente en C++17 con dependencias declaradas.
- [ ] Python funciona en la versión mínima publicada.
- [ ] Los límites numéricos y de memoria cubren los intermedios de la implementación impresa.
- [ ] No quedan capacidades, firmas, miembros o retornos inexistentes anunciados.
- [ ] Los comparadores cumplen orden débil estricto y las tolerancias no se usan para simular equivalencias transitivas inexistentes.
- [ ] El inventario separa estructura, ejecución, contrato y regresiones; no presenta completitud editorial como certificación algorítmica.
- [ ] La guía y el índice reflejan técnicas reales y sus precondiciones.
- [ ] Todas las opciones documentadas del build funcionan y preservan fuentes y última salida válida ante fallos.
- [ ] El PDF regenerado supera revisión visual y prueba de legibilidad a tamaño real.
- [ ] Los reportes contienen métricas medidas y no atribuyen pruebas no ejecutadas.
- [ ] Se realiza una práctica usando únicamente el notebook y se registran tiempos de búsqueda, dificultades de adaptación y errores de uso.

## 15. Límites de la evidencia

- La auditoría revisó las 164 fichas y ejecutó la suite existente más pruebas dirigidas; no constituye una demostración formal de todos los algoritmos ni de todos sus dominios.
- Los contraejemplos nuevos no deben confundirse con tests permanentes ya incorporados: añadirlos es parte de este plan.
- Resultados observados tras overflow con signo no son comportamiento portable; indican comportamiento indefinido que debe eliminarse o excluirse mediante un contrato adecuado.
- Los fallos de pila dependen del entorno, aunque son reproducibles en Windows y relevantes para los límites anunciados.
- UBSan no estuvo disponible; las comprobaciones con assertions y pruebas dirigidas no lo sustituyen.
- La regeneración temporal del PDF comprueba compilación y no destructividad, no legibilidad, navegación visual ni conformidad con reglas de una competencia.
- No se validaron visualmente las páginas ni se ejecutó una simulación humana de competencia durante esta auditoría.
- El modo watch y todas las variantes del wrapper/build siguen pendientes de validación específica.
- Crear o actualizar este plan no corrige por sí mismo el notebook ni autoriza a marcar sus fases como completadas.
