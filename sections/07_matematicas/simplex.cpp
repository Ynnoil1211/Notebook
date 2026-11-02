// Simplex de Dos Fases con desempate de Bland (Anti-ciclos)
// Maximiza c^T x sujeto a A x <= b, x >= 0.
struct Simplex {
  const ld EPS_LP = 1e-9, INF_LP = 1e18;
  int n, m; // n restricciones, m variables
  vi B, N;
  vector<ld> b, c;
  vector<vector<ld>> a, D;

  Simplex(int n, int m) : n(n), m(m), b(n), c(m), a(n,
    vector<ld>(m, 0)) {}

  void pivot(int r, int s) {
    ld inv = 1.0L / D[r][s];
    for (int i = 0; i < n + 2; ++i) if (i != r && abs(
      D[i][s]) > EPS_LP) {
      ld inv2 = D[i][s] * inv;
      for (int j = 0; j < m
        + 2; ++j) D[i][j] -= D[r][j] * inv2;
      D[i][s] = D[r][s] * inv2;
    }
    for (int j = 0; j < m + 2; ++j) if (
      j != s) D[r][j] *= inv;
    for (int i = 0; i < n + 2; ++i) if (
      i != r) D[i][s] *= -inv;
    D[r][s] = inv;
    swap(B[r], N[s]);
  }

  bool run_simplex(int phase) {
    int x = n + (phase == 1);
    while (true) {
      int s = -1;
      for (int j = 0; j <= m; ++j) {
        if (phase == 2 && N[j] == -1) continue;
        if (D[x][j] < -EPS_LP) {
          if (s == -1 || make_pair(D[x][j],
            N[j]) < make_pair(D[x][s], N[s]))
            s = j;
        }
      }
      if (s == -1) return true;
      int r = -1;
      for (int i = 0; i < n; ++i) {
        if (D[i][s] <= EPS_LP) continue;
        ld ratio = D[i][m + 1] / D[i][s];
        if (r == -1 ||
            make_pair(ratio, B[i]) < make_pair(
              D[r][m + 1] / D[r][s], B[r]))
          r = i;
      }
      if (r == -1) return false; // No acotado
      pivot(r, s);
    }
  }

  // Retorna {max_Z, x}. Infactible: {-INF_LP, {}}. No
  // acotado: {INF_LP, {}}.
  pair<ld, vector<ld>> solve() {
    B.resize(n); N.resize(m + 1);
    D.assign(n + 2, vector<ld>(m + 2, 0));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) D[i][j] = a[i][j];
      B[i] = m + i;
      D[i][m] = -1;
      D[i][m + 1] = b[i];
    }
    for (int j = 0; j < m; ++j) { N[j] = j; D[n][j] = -c[j];
      }
    N[m] = -1; D[n + 1][m] = 1;
    int r = 0;
    for (int i = 1; i < n; ++i) if (
      D[i][m + 1] < D[r][m + 1]) r = i;
    if (D[r][m + 1] < -EPS_LP) {
      pivot(r, m);
      if (!run_simplex(
        1) || D[n + 1][m + 1] < -EPS_LP) return {-INF_LP, {
          }};
      for (int i = 0; i < n; ++i) if (B[i] == -1) {
        int s = 0;
        for (int j = 1; j <= m; ++j)
          if (s == -1 || make_pair(D[i][j],
            N[j]) < make_pair(D[i][s], N[s]))
            s = j;
        pivot(i, s);
      }
    }
    if (!run_simplex(2)) return {INF_LP, {}};
    vector<ld> x(m, 0);
    for (int i = 0; i < n; ++i) if (
      B[i] < m) x[B[i]] = D[i][m + 1];
    return {D[n][m + 1], x};
  }
};
