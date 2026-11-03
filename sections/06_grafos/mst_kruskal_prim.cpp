// 2. Kruskal MST (requiere DSU de la seccion 4.1)
struct KrEdge {
    int u, v;
    ll w;
    bool operator<(const KrEdge& o) const {
        return w < o.w;
    }
};
ll kruskal_mst(int n, vector<KrEdge>& edges,
               vector<KrEdge>& mst) {
    sort(all(edges));
    DSU dsu(n);
    ll cost = 0;
    for (const auto& e : edges)
        if (dsu.unite(e.u, e.v)) {
            cost += e.w;
            mst.pb(e);
        }
    return (dsu.num_comp == 1) ? cost : -1;
}
