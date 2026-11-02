// 1. Exponenciacion de Matrices en O(D^3 log E)
struct Matrix {
  int n;
  vector<vll> m;
  Matrix(int n, ll diag = 0) : n(n), m(n, vll(n, 0)) {
    for (int i = 0; i < n; ++i) m[i][i] = diag;
  }
  Matrix operator*(const Matrix& o) const {
    Matrix res(n, 0);
    for (int i = 0; i < n; ++i)
      for (int k = 0; k < n; ++k) {
        if (!m[i][k]) continue;
        for (int j = 0; j < n; ++j)
          res.m[i][j] = (
            res.m[i][j] + m[i][k] * o.m[k][j]) % MOD;
      }
    return res;
  }
};
Matrix mat_pow(Matrix b, ll e) {
  Matrix res(b.n, 1); // Identidad
  for (; e > 0; e >>= 1) {
    if (e & 1) res = res * b;
    b = b * b;
  }
  return res;
}

// Fibonacci en O(log N) usando duplicacion rapida: retorna
// {F_n, F_{n+1}}
pll fib_fast(ll n, ll m = MOD) {
  if (n == 0) return {0, 1};
  auto [a, b] = fib_fast(n >> 1, m);
  ll c = a * (2 * b - a + m) % m;
  ll d = (a * a % m + b * b % m) % m;
  return (n & 1) ? pll{d, (c + d) % m} : pll{c, d};
}
