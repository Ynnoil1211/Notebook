from collections import Counter

c1 = Counter([1, 2, 2, 3])
c2 = Counter([2, 3, 3, 4])

# Operaciones algebraicas de multiconjuntos:
suma  = c1 + c2     # Suma conteos: {2: 3, 3: 3, 1: 1, 4: 1}
# Resta (mantiene solo positivos): {1: 1, 2: 1}
resta = c1 - c2
# Minimo conteo (interseccion): {2: 1, 3: 1}
inter = c1 & c2
# Maximo conteo (union): {1: 1, 2: 2, 3: 2, 4: 1}
union = c1 | c2

# Top K elementos mas frecuentes en O(N log K):
top2 = c1.most_common(2) # [(2, 2), (1, 1)]
