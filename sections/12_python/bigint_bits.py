n = 10**60

# Cantidad de bits encendidos (popcount) de un BigInt:
unos = n.bit_count()

# Cuantos bits ocupa el numero:
bits_necesarios = n.bit_length()
