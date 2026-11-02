// 1. Prefix Sum 1D (1-indexed en las consultas [l, r])
struct PrefixSum {
  vll pref;
  PrefixSum(const vll& a) : pref(sz(a) + 1, 0) {
    for (int i = 0; i < sz(a); ++i)
      pref[i + 1] = pref[i] + a[i];
  }
  // Constructor para contar caracteres adyacentes iguales
  // (CF 313B)
  PrefixSum(const string& s) : pref(sz(s) + 1, 0) {
    for (int i = 0; i + 1 < sz(s); ++i)
      pref[i + 1] = pref[i] + (s[i] == s[i + 1]);
    if (!s.empty()) pref[sz(s)] = pref[sz(s) - 1];
  }
  ll query(int l, int r) const { // 1-based [l, r]
    return (l > r) ? 0 : pref[r] - pref[l - 1];
  }
};
