// 2. Pollard's Rho (Factorizacion completa en O(N^(1/4)))
ull pollard_f(ull x, ull c, ull m) {
  return (mul_mod64(x, x, m) + c) % m;
}
ull pollard_rho(ull n) {
  if ((n & 1) == 0) return 2;
  ull x = 2, y = 2, d = 1, c = rd(1, n - 1);
  while (d == 1) {
    x = pollard_f(x, c, n);
    y = pollard_f(pollard_f(y, c, n), c, n);
    d = std::gcd(x > y ? x - y : y - x, n);
  }
  return (d == n) ? pollard_rho(n) : d;
}
void factorize_64(ull n, map<ull, int>& factors) {
  if (n == 1) return;
  if (miller_rabin(n)) { factors[n]++; return; }
  ull div = pollard_rho(n);
  factorize_64(div, factors);
  factorize_64(n / div, factors);
}
