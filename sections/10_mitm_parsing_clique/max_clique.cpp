// Max Clique para N <= 64 usando mascaras ull
struct MaxClique {
    int n;
    vector<ull> adj;
    vi best_clique;

    MaxClique(int n) : n(n), adj(n, 0) {}

    void add_edge(int u, int v) {
        adj[u] |= (1ULL << v);
        adj[v] |= (1ULL << u);
    }
    void expand(ull R, ull P, ull X, vi& cur) {
        if (P == 0 && X == 0) {
            if (sz(cur) > sz(best_clique))
                best_clique = cur;
            return;
        }
        if (sz(cur) + __builtin_popcountll(P) <=
            sz(best_clique))
            return;
        int u = __builtin_ctzll(P | X);
        ull cand = P & ~adj[u];
        while (cand) {
            int v = __builtin_ctzll(cand);
            cand &= ~(1ULL << v);
            cur.push_back(v);
            expand(R | (1ULL << v), P & adj[v], X & adj[v],
                   cur);
            cur.pop_back();
            P &= ~(1ULL << v);
            X |= (1ULL << v);
        }
    }
    vi solve() {
        best_clique.clear();
        ull P = (n == 64 ? ~0ULL : (1ULL << n) - 1);
        vi cur;
        expand(0, P, 0, cur);
        return best_clique;
    }
};
