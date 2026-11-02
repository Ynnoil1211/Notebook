from collections import defaultdict

# Grafo sin tamano previo:
adj = defaultdict(list)
adj[u].append(v)

# Agrupar anagramas:
grupos = defaultdict(list)
for w in words:
    grupos["".join(sorted(w))].append(w)
