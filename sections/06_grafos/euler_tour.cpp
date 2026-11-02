// 3. Euler Tour en Arboles (tin, tout y ancestralidad en
// O(1))
int tin_e[200005], tout_e[200005], timer_e = 0;
void dfs_euler(int u, int p) {
  tin_e[u] = ++timer_e;
  for (int v : adj_dfs[u])
    if (v != p) dfs_euler(v, u);
  tout_e[u] = timer_e;
}
bool is_ancestor(int u, int v) { // ¿u es ancestro de v?
  return tin_e[u] <= tin_e[v] && tout_e[v] <= tout_e[u];
}
