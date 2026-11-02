// 2. MEX (Minimum Excluded >= 0) en O(N)
int mex(const vi& v) {
  int n = sz(v);
  vector<bool> pres(n + 1, false);
  for (int x : v) if (0 <= x && x <= n) pres[x] = true;
  for (int i = 0; i <= n; ++i) if (!pres[i]) return i;
  return n + 1;
}
