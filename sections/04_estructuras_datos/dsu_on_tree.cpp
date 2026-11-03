// 2. DSU on Tree (Sack) — Consultas de frecuencia en
// subarboles en O(N log N)
const int MAX_SACK = 200005;
vi adj_s[MAX_SACK];
int col_s[MAX_SACK], cnt_s[MAX_SACK], big_s[MAX_SACK];
int tin_s[MAX_SACK], tout_s[MAX_SACK], ver_s[MAX_SACK],
    t_sack = 0;

int pre_sack(int u, int p) {
    int sz = 1, mx = 0;
    tin_s[u] = ++t_sack;
    ver_s[t_sack] = u;
    big_s[u] = -1;
    for (int v : adj_s[u]) {
        if (v == p) continue;
        int c_sz = pre_sack(v, u);
        sz += c_sz;
        if (c_sz > mx) {
            mx = c_sz;
            big_s[u] = v;
        }
    }
    tout_s[u] = t_sack;
    return sz;
}
void add_node_sack(int u, int delta) {
    // Actualizar estructura de frecuencias
    cnt_s[col_s[u]] += delta;
}
void dfs_sack(int u, int p, bool keep) {
    for (int v : adj_s[u])
        if (v != p && v != big_s[u]) dfs_sack(v, u, false);
    if (big_s[u] != -1) dfs_sack(big_s[u], u, true);

    for (int v : adj_s[u])
        if (v != p && v != big_s[u])
            for (int t = tin_s[v]; t <= tout_s[v]; ++t)
                add_node_sack(ver_s[t], +1);
    add_node_sack(u, +1);

    // AQUI: cnt_s[c] tiene la cantidad exacta de nodos de
    // color 'c'
    // en el subarbol de 'u'. Responder todas las queries de
    // 'u'.

    if (!keep)
        for (int t = tin_s[u]; t <= tout_s[u]; ++t)
            add_node_sack(ver_s[t], -1);
}
