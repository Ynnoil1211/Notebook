// 1b. CHT Estático/Monótono con Deque en O(N) (para MINIMO:
// insertar pendientes m decrecientes
//     y consultar x crecientes).
struct MonotoneCHT {
  struct L { ll m, b; ll eval(ll x) const {
    return m * x + b; } };
  deque<L> dq;
  bool bad(const L& l1, const L& l2, const L& l3) const {
    return (__int128)(l3.b - l1.b) * (l1.m - l2.m) <=
      (__int128)(l2.b - l1.b) * (l1.m - l3.m);
  }
  // Precondicion: m estrictamente decreciente
  void add(ll m, ll b) {
    L cur = {m, b};
    while (sz(dq) >= 2 && bad(dq[sz(dq) - 2], dq.back(),
      cur)) dq.pop_back();
    dq.pb(cur);
  }
  // Precondicion: x creciente y !dq.empty()
  ll query_monotone(ll x) {
    while (sz(dq) >= 2 && dq[0].eval(x) >= dq[1].eval(
      x)) dq.pop_front();
    return dq[0].eval(x);
  }
};
