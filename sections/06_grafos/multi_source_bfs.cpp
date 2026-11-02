// 2. Multi-Source BFS (con aristas invertidas adj_rev si es
// dirigido)
vi adj_rev[200005];
int dist_ms[200005];

void multi_source_bfs(const vi& sources, int n) {
  fill(dist_ms + 1, dist_ms + n + 1, -1);
  queue<int> q;
  for (int s : sources) { dist_ms[s] = 0; q.push(s); }
  while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : adj_rev[u]) {
      if (dist_ms[v] == -1) {
        dist_ms[v] = dist_ms[u] + 1;
        q.push(v);
      }
    }
  }
}
