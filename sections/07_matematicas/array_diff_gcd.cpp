// 1. GCD de todo el arreglo A despues de sumar X a cada
// elemento
ll array_diff_gcd(const vll& a, ll x) {
  int n = sz(a);
  if (n == 1) return abs(a[0] + x);
  ll diff_g = 0;
  for (int i = 1; i < n; ++i) diff_g = std::gcd(diff_g, abs(
    a[i] - a[i - 1]));
  return std::gcd(abs(a[0] + x), diff_g);
}
