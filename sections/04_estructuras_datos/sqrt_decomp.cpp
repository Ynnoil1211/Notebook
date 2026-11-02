// 1. SQRT Decomposition en Bloques (0-indexed)
struct SqrtDecomp {
  int n, B;
  vll a, b;
  SqrtDecomp(const vll& arr)
    : n(sz(arr)), B((int)sqrt(n) + 1), a(arr), b(B, 0) {
    for (int i = 0; i < n; ++i) b[i / B] += a[i];
  }
  void update(int pos, ll val) { // O(1)
    b[pos / B] += val - a[pos];
    a[pos] = val;
  }
  ll query(int l, int r) const { // O(sqrt(N))
    ll sum = 0;
    int bl = l / B, br = r / B;
    if (bl == br) {
      for (int i = l; i <= r; ++i) sum += a[i];
    } else {
      for (int i = l, end = (
        bl + 1) * B - 1; i <= end; ++i) sum += a[i];
      for (int i = bl + 1; i < br; ++i) sum += b[i];
      for (int i = br * B; i <= r; ++i) sum += a[i];
    }
    return sum;
  }
};
