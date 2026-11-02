ull mul_mod64(ull a, ull b, ull m) {
  return (ull)((__uint128_t)a * b % m);
}
ull pow_mod64(ull b, ull e, ull m) {
  ull r = 1;
  for (b %= m; e > 0; e >>= 1) {
    if (e & 1) r = mul_mod64(r, b, m);
    b = mul_mod64(b, b, m);
  }
  return r;
}

// 1. Miller-Rabin Determinista para todo entero de 64 bits
// (< 2^64)
bool miller_rabin(ull n) {
  if (n < 2) return false;
  for (ull p : {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    if (n % p == 0) return n == p;
  }
  ull d = n - 1, s = 0;
  while ((d & 1) == 0) { d >>= 1; s++; }
  for (ull a : {2ULL, 325ULL, 9375ULL, 28178ULL,
                450775ULL, 9780504ULL, 1795265022ULL}) {
    if (a % n == 0) continue;
    ull x = pow_mod64(a, d, n);
    if (x == 1 || x == n - 1) continue;
    bool composite = true;
    for (ull r = 1; r < s; ++r) {
      x = mul_mod64(x, x, n);
      if (x == n - 1) { composite = false; break; }
    }
    if (composite) return false;
  }
  return true;
}
