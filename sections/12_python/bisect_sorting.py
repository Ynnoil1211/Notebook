import bisect

# 1. Búsqueda binaria con funcion key (Python 3.10+):
eventos = [(10, "A"), (25, "B"), (40, "C"), (90, "D")]
# lower_bound por el primer elemento (tiempo >= 30):
# idx = 2
idx = bisect.bisect_left(eventos, 30, key=lambda x: x[0])

# 2. Ordenamiento multi-criterio:
# 1) Puntos descendente (-x['pts'])
# 2) Penalidad ascendente (+x['pen'])
# 3) Nombre alfabetico (+x['nom'])
jugadores.sort(key=lambda x: (-x['pts'], x['pen'],
    x['nom']))
