// 2. Algoritmo de Mo (Offline con ordenamiento serpenteante
// par/impar)
// ~ N / sqrt(Q) para N, Q <= 2e5
const int BLOCK_SIZE = 450;
struct MoQuery {
  int l, r, idx;
  bool operator<(const MoQuery& o) const {
    int b1 = l / BLOCK_SIZE, b2 = o.l / BLOCK_SIZE;
    if (b1 != b2) return b1 < b2;
    // Optimiza el puntero R
    return (b1 & 1) ? (r < o.r) : (r > o.r);
  }
};

// Ejemplo: Contar cantidad de elementos distintos en cada
// rango [l, r]
vll solve_mo(const vi& a, vector<MoQuery>& queries) {
  int max_val = *max_element(all(a));
  vi freq(max_val + 1, 0);
  int distinct_cnt = 0;

  auto add = [&](int idx) {
    if (++freq[a[idx]] == 1) distinct_cnt++;
  };
  auto del = [&](int idx) {
    if (--freq[a[idx]] == 0) distinct_cnt--;
  };

  sort(all(queries));
  vll ans(sz(queries));
  int cur_l = 0, cur_r = -1;

  // ¡IMPORTANTE! Primero expandir (cur_l--, cur_r++), luego
  // contraer
  for (const auto& q : queries) {
    while (cur_l > q.l) add(--cur_l);
    while (cur_r < q.r) add(++cur_r);
    while (cur_l < q.l) del(cur_l++);
    while (cur_r > q.r) del(cur_r--);
    ans[q.idx] = distinct_cnt;
  }
  return ans;
}
