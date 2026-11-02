from functools import cache

# DP de Mochila o Digit DP con memoizacion automatica:
@cache
def dp(i: int, rem_weight: int) -> int:
    if i == n or rem_weight == 0:
        return 0
    # Opcion 1: No tomar
    res = dp(i + 1, rem_weight)
    # Opcion 2: Tomar
    if weight[i] <= rem_weight:
        res = max(res, val[i] + dp(i + 1,
            rem_weight - weight[i]))
    return res

# Limpiar memoizacion entre casos de prueba:
dp.cache_clear()
