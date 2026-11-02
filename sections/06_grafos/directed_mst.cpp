// Edmonds' Chu-Liu-Edmonds en O(E log V) con Skew Heaps
struct DirEdge { int u, v; ll w; };
struct SkewNode {
  DirEdge key;
  SkewNode *l = 0, *r = 0;
  ll delta = 0;
  void prop() {
    key.w += delta;
    if (l) l->delta += delta;
    if (r) r->delta += delta;
    delta = 0;
  }
  DirEdge top() { prop(); return key; }
};
SkewNode* skew_merge(SkewNode *a, SkewNode *b) {
  if (!a || !b) return a ? a : b;
  a->prop(); b->prop();
  if (a->key.w > b->key.w) swap(a, b);
  swap(a->l, (a->r = skew_merge(b, a->r)));
  return a;
}
void skew_pop(SkewNode*& a) {
  a->prop(); a = skew_merge(a->l, a->r);
}

// Retorna {costo_total, parent_edge} (-1 si no es conexo)
pair<ll, vi> directed_mst(int n, int root,
  vector<DirEdge>& edges) {
  struct ArbUF {
    vi p, r;
    vector<pair<int, int>> hist;
    ArbUF(int n) : p(n), r(n, 0) { iota(all(p), 0); }
    int find(int i) { return p[i] == i ? i : find(p[i]); }
    bool unite(int i, int j) {
      i = find(i); j = find(j);
      if (i == j) return false;
      if (r[i] < r[j]) swap(i, j);
      hist.push_back({j, r[i] == r[j]});
      p[j] = i;
      if (r[i] == r[j]) r[i]++;
      return true;
    }
    int time() { return sz(hist); }
    void rollback(int t) {
      while (sz(hist) > t) {
        auto [j, inc] = hist.back(); hist.pop_back();
        if (inc) r[p[j]]--;
        p[j] = j;
      }
    }
  } uf(n);

  vector<SkewNode*> heap(n, 0);
  for (auto& e : edges) heap[e.v] = skew_merge(heap[e.v],
    new SkewNode{e});
  ll res = 0;
  vi seen(n, -1), path(n), par(n, -1);
  seen[root] = root;
  vector<DirEdge> Q(n), in(n, {-1, -1, 0});
  deque<tuple<int, int, vector<DirEdge>>> cycs;

  for (int s = 0; s < n; ++s) {
    int u = s, qi = 0, w;
    while (seen[u] < 0) {
      if (!heap[u]) return {-1, {}}; // Inalcanzable
      DirEdge e = heap[u]->top();
      heap[u]->delta -= e.w; skew_pop(heap[u]);
      Q[qi] = e; path[qi++] = u; seen[u] = s;
      res += e.w; u = uf.find(e.u);
      if (seen[u] == s) { // Ciclo detectado: contraer
        SkewNode* cyc = 0;
        int end = qi, t = uf.time();
        do cyc = skew_merge(cyc, heap[w = path[--qi]]);
        while (uf.unite(u, w));
        u = uf.find(u); heap[u] = cyc; seen[u] = -1;
        cycs.push_front({u, t, {&Q[qi], &Q[end]}});
      }
    }
    for (int i = 0; i < qi; ++i) in[uf.find(Q[i].v)] = Q[i];
  }
  for (auto& [u, t, comp] : cycs) { // Reconstruccion
    uf.rollback(t);
    DirEdge inEdge = in[u];
    for (auto& e : comp) in[uf.find(e.v)] = e;
    in[uf.find(inEdge.v)] = inEdge;
  }
  for (int i = 0; i < n; ++i) par[i] = in[i].u;
  return {res, par};
}
