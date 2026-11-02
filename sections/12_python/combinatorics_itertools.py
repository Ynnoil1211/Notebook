from math import comb, perm, factorial
from itertools import permutations, combinations, product

# nCr y nPr gigantes sin overflow:
formas = comb(1000, 50)
p = perm(100, 3)
fact_100 = factorial(100)

# Generacion exhaustiva:
perms = list(permutations([1, 2, 3]))        # 3! formas
combs = list(combinations([1, 2, 3, 4], 2))  # 4C2 formas
# 2^3 producto cartesiano
cart  = list(product([0, 1], repeat=3))
