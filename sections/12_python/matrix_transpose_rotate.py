mat = [
    [1, 2, 3],
    [4, 5, 6]
]

# 1. Transponer matriz (columnas a filas):
trans = [list(col) for col in zip(*mat)]

# 2. Rotar 90 grados sentido horario:
rot90 = [list(col[::-1]) for col in zip(*mat)]

# 3. Rotar 90 grados sentido antihorario:
rot90_ccw = [list(col) for col in zip(*mat)][::-1]
