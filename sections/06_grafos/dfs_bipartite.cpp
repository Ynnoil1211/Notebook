// 1. Grafo Bipartito (2-Coloreo con colores 1 y 2)
vi adj_dfs[200005];
int col_bip[200005];  // Inicializar en 0
bool dfs_bipartite(int u, int c) {
    col_bip[u] = c;
    for (int v : adj_dfs[u]) {
        if (col_bip[v] == 0) {
            if (!dfs_bipartite(v, 3 - c)) return false;
        } else if (col_bip[v] == col_bip[u]) return false;
    }
    return true;
}
