// 2. Ciclo en Grafo Dirigido (0 = blanco, 1 = gris/activo,
// 2 = negro)
int state_dir[200005];
bool dfs_dir_cycle(int u) {
    state_dir[u] = 1;
    for (int v : adj_dfs[u]) {
        // Back-edge encontrada
        if (state_dir[v] == 1) return true;
        if (state_dir[v] == 0 && dfs_dir_cycle(v))
            return true;
    }
    state_dir[u] = 2;
    return false;
}
