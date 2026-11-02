// 1. Grid BFS / Flood Fill con Identificador de Componentes
const int MAX_R = 1005, MAX_C = 1005;
int n_g, m_g, comp_g[MAX_R][MAX_C], ans_comp[MAX_R * MAX_C];
char grid_g[MAX_R][MAX_C];
int dx4[] = {-1, 1, 0, 0}, dy4[] = {0, 0, -1, 1};

void flood_fill_bfs(int sr, int sc, int id) {
  queue<pii> q;
  q.push({sr, sc});
  comp_g[sr][sc] = id;
  int metric = 0;
  while (!q.empty()) {
    auto [r, c] = q.front(); q.pop();
    for (int d = 0; d < 4; ++d) {
      int nr = r + dx4[d], nc = c + dy4[d];
      if (nr < 0 || nr >= n_g || nc < 0 || nc
        >= m_g) continue;
      if (grid_g[nr][nc] == '*') {
        metric++; // Ejemplo: contar paredes '*' adyacentes
      } else if (
        grid_g[nr][nc] == '.' && comp_g[nr][nc] == -1) {
        comp_g[nr][nc] = id;
        q.push({nr, nc});
      }
    }
  }
  ans_comp[id] = metric;
}
