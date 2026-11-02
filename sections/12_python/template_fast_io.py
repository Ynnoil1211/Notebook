import sys
from math import gcd, isqrt, comb, perm, factorial
from collections import defaultdict, Counter, deque
from fractions import Fraction
from decimal import Decimal, getcontext
from functools import cache
import bisect
import itertools
import re

# Aumentar limite de recursion para DFS profunda
sys.setrecursionlimit(10**6)

# OBLIGATORIO en Python 3.11+ para BigInt > 4300 digitos a
# string
sys.set_int_max_str_digits(10**6)

def solve():
    # Lectura ultra rapida de todos los tokens
    input_data = sys.stdin.read().split()
    if not input_data:
        return
    it = iter(input_data)
    # n = int(next(it))

if __name__ == '__main__':
    solve()
