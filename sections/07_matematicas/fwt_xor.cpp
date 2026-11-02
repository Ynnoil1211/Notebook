// 5. Fast Walsh-Hadamard Transform (FWT para Convolucion
// XOR en O(N log N))
// n debe ser potencia de 2.
void fwt_xor(vll& a, bool inv) {
  int n = sz(a);
  for (int len = 1; 2 * len <= n; len <<= 1) {
    for (int i = 0; i < n; i += 2 * len) {
      for (int j = 0; j < len; ++j) {
        ll u = a[i + j], v = a[i + len + j];
        a[i + j] = u + v;
        a[i + len + j] = u - v;
        if (inv) { a[i + j] /= 2; a[i + len + j] /= 2; }
      }
    }
  }
}
