// 4. Componentes Conexas en el Grafo Complemento en O((N +
// M) log N)
vector<vi> complement_components(
    int n, vector<vi>& forbidden_adj) {
    for (int i = 1; i <= n; ++i)
        sort(all(forbidden_adj[i]));
    set<int> unvis;
    for (int i = 1; i <= n; ++i) unvis.insert(i);
    vector<vi> comps;

    for (int i = 1; i <= n; ++i) {
        if (!unvis.count(i)) continue;
        vi comp;
        queue<int> q;
        q.push(i);
        unvis.erase(i);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            comp.pb(u);
            for (auto it = unvis.begin();
                 it != unvis.end();) {
                int v = *it;
                if (!binary_search(all(forbidden_adj[u]),
                                   v)) {
                    q.push(v);
                    it = unvis.erase(
                        it);  // Borra en O(1) amortizado!
                } else ++it;
            }
        }
        comps.pb(comp);
    }
    return comps;
}
