# 1. De cualquier Base B a Entero Decimal: int(string, base)
num = int("1A3F", 16)       # Hexadecimal a int -> 6719
num = int("101101", 2)      # Binario a int -> 45
num = int("Z9", 36)         # Base 36 a int -> 1269

# 2. De Decimal a Binario, Octal, Hexadecimal nativo:
s_bin = bin(45)[2:]         # "101101"
s_oct = oct(45)[2:]         # "55"
s_hex = hex(45)[2:].upper() # "2D"

# Con f-strings y padding de ceros (ej. 8 bits):
f_bin = f"{45:08b}"         # "00101101"
f_hex = f"{45:04X}"         # "002D"

# 3. De Decimal a cualquier Base B (2 <= B <= 62):
CHARS = ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "abcdefghijklmnopqrstuvwxyz")

def to_base(n: int, b: int) -> str:
    if n == 0:
        return "0"
    res = []
    neg = n < 0
    n = abs(n)
    while n > 0:
        res.append(CHARS[n % b])
        n //= b
    if neg:
        res.append('-')
    return "".join(reversed(res))

# Conversion directa Base A -> Base B:
def base_a_to_b(s: str, base_a: int, base_b: int) -> str:
    return to_base(int(s, base_a), base_b)
