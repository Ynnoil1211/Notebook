// 1. Dijkstra con Reconstruccion de Camino (1-indexed)
const int MAX_V_DIJ = 200005;
vector<pair<int, ll>> adj_dij[MAX_V_DIJ];  // {vecino, peso}
ll dist_dij[MAX_V_DIJ];
int par_dij[MAX_V_DIJ];

void dijkstra(int s, int n) {
    fill(dist_dij + 1, dist_dij + n + 1, INF);
    fill(par_dij + 1, par_dij + n + 1, -1);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>,
                   greater<>>
        pq;
    dist_dij[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist_dij[u]) continue;
        for (auto [v, w] : adj_dij[u]) {
            if (dist_dij[u] + w < dist_dij[v]) {
                dist_dij[v] = dist_dij[u] + w;
                par_dij[v] = u;
                pq.push({dist_dij[v], v});
            }
        }
    }
}
vi get_path_dij(int target) {
    if (dist_dij[target] == INF) return {};
    vi path;
    for (int cur = target; cur != -1; cur = par_dij[cur])
        path.pb(cur);
    reverse(all(path));
    return path;
}
