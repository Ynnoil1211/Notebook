# 1. Fracciones Exactas (simplifica automaticamente)
from fractions import Fraction

f1 = Fraction(3, 6)         # 1/2
f2 = Fraction("0.25")       # 1/4
res = f1 + f2               # 3/4
print(f"{res.numerator}/{res.denominator}")

# 2. Decimales con precision configurable (ej. 100 digitos)
from decimal import Decimal, getcontext
getcontext().prec = 100     # 100 digitos significativos

pi_approx = Decimal(22) / Decimal(7)
raiz_2 = Decimal(2).sqrt()
