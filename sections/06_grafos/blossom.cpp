// 4. Blossom (Max Matching en Grafo General No Bipartito en
// O(V^3))
struct Blossom {
  int n, timer = 0;
  vector<vi> adj;
  vi match, p, base, vis, in_q, in_b;
  queue<int> q;

  Blossom(int n)
    : n(n), adj(n + 1), match(n + 1, 0), p(n + 1, 0),
      base(n + 1, 0), vis(n + 1, 0), in_q(n + 1, 0), in_b(
        n + 1, 0) {}

  void add_edge(int u, int v) { adj[u].pb(v); adj[v].pb(u);
    }

  int lca(int u, int v) {
    ++timer;
    while (true) {
      if (u) {
        u = base[u];
        if (vis[u] == timer) return u;
        vis[u] = timer;
        u = p[match[u]];
      }
      swap(u, v);
    }
  }
  void mark_blossom(int u, int v, int b) {
    while (base[u] != b) {
      in_b[base[u]] = in_b[base[match[u]]] = 1;
      p[u] = v;
      v = match[u];
      u = p[v];
    }
  }
  int bfs(int s) {
    fill(all(p), 0); fill(all(in_q), 0);
    for (int i = 1; i <= n; ++i) base[i] = i;
    while (!q.empty()) q.pop();
    q.push(s); in_q[s] = 1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : adj[u]) {
        if (base[u] == base[v] || match[u] == v) continue;
        if (v == s || (match[v] && p[match[v]])) {
          int b = lca(u, v);
          fill(all(in_b), 0);
          mark_blossom(u, v, b);
          mark_blossom(v, u, b);
          for (int i = 1; i <= n; ++i)
            if (in_b[base[i]]) {
              base[i] = b;
              if (!in_q[i]) { q.push(i); in_q[i] = 1; }
            }
        } else if (!p[v]) {
          p[v] = u;
          if (!match[v]) return v;
          v = match[v];
          q.push(v); in_q[v] = 1;
        }
      }
    }
    return 0;
  }
  int solve() {
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
      if (!match[i]) {
        int cur = bfs(i);
        if (cur) {
          ans++;
          while (cur) {
            int nxt = match[p[cur]];
            match[cur] = p[cur];
            match[p[cur]] = cur;
            cur = nxt;
          }
        }
      }
    }
    // Cantidad de parejas; match[u] indica pareja de u
    return ans;
  }
};
