// 1. Orden Topologico (Kahn BFS — detecta ciclos si
// sz(order) < n)
vi topological_sort(int n, const vector<vi>& adj) {
  vi in_deg(n + 1, 0), order;
  for (int u = 1; u <= n; ++u)
    for (int v : adj[u]) in_deg[v]++;
  // Usar Min-Heap si piden orden lexicografico menor
  queue<int> q;
  for (int i = 1; i <= n; ++i)
    if (in_deg[i] == 0) q.push(i);
  while (!q.empty()) {
    int u = q.front(); q.pop();
    order.pb(u);
    for (int v : adj[u])
      if (--in_deg[v] == 0) q.push(v);
  }
  return order;
}
