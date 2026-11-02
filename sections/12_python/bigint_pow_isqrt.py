# Potencia Modular BigInt en O(log e): pow(b, e, mod)
# Mucho mas veloz que (b ** e) % mod
ans = pow(3, 10**18, 10**9 + 7)

# Division entera con negativos (sin usar float, exacto para
# BigInt):
# En Python -7 // 3 da -3 (floor hacia -infinito).
# Para truncar hacia cero como en C++ sin perder precision
# en BigInt:
def trunc_div(a: int, b: int) -> int:
    return -(
        abs(
            a) // abs(
                b)) if (
                    a < 0) ^ (b < 0) else abs(a) // abs(b)

# Raiz Cuadrada Entera Exacta: isqrt(N)
# Retorna floor(sqrt(N)) sin perder precision de punto
# flotante
r = isqrt(10**50)  # 10^25 exacto (double fallaria aqui)
