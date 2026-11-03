// 3. 0-1 BFS en Grafo General o Grilla con Reconstruccion
// de Camino
vector<pii> adj01[200005];  // {vecino, peso_0_o_1}
int dist01[200005], par01[200005];

void bfs_01(int s, int n) {
    fill(dist01 + 1, dist01 + n + 1, INF_INT);
    fill(par01 + 1, par01 + n + 1, -1);
    deque<int> dq;
    dist01[s] = 0;
    dq.push_front(s);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (auto [v, w] : adj01[u]) {
            if (dist01[u] + w < dist01[v]) {
                dist01[v] = dist01[u] + w;
                par01[v] = u;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
}
vi get_path_01(int target) {
    if (dist01[target] == INF_INT) return {};
    vi path;
    for (int cur = target; cur != -1; cur = par01[cur])
        path.pb(cur);
    reverse(all(path));
    return path;
}
