// 1. Trie Binario (Max/Min XOR con insercion y borrado;
// precondicion: no vacio)
struct BitTrie {
  static const int B = 60; // 30 para int, 60 para long long
  struct Node { int ch[2] = {-1, -1}, cnt = 0; };
  vector<Node> t = {Node()};

  // delta = +1 insertar, -1 borrar (x debe existir)
  void update(ll x, int delta) {
    int u = 0;
    for (int b = B; b >= 0; --b) {
      int bit = (x >> b) & 1LL;
      if (t[u].ch[bit] == -1) {
        t[u].ch[bit] = sz(t);
        t.emplace_back();
      }
      u = t[u].ch[bit];
      t[u].cnt += delta;
    }
  }
  // Retorna max(x ^ y) para 'y' activo en el Trie
  ll max_xor(ll x) const {
    int u = 0;
    ll ans = 0;
    for (int b = B; b >= 0; --b) {
      int bit = (x >> b) & 1LL, want = bit ^ 1;
      if (t[u].ch[want] != -1 && t[t[u].ch[want]].cnt > 0) {
        ans |= (1LL << b);
        u = t[u].ch[want];
      } else {
        u = t[u].ch[bit];
      }
    }
    return ans;
  }
  // Retorna min(x ^ y) para 'y' activo en el Trie
  ll min_xor(ll x) const {
    int u = 0;
    ll ans = 0;
    for (int b = B; b >= 0; --b) {
      int bit = (x >> b) & 1LL;
      if (t[u].ch[bit] != -1 && t[t[u].ch[bit]].cnt > 0) {
        u = t[u].ch[bit];
      } else {
        ans |= (1LL << b);
        u = t[u].ch[bit ^ 1];
      }
    }
    return ans;
  }
};
