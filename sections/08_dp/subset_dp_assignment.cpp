// 1. DP de Asignacion con dp[mask] en O(N * 2^N) tiempo y
// O(2^N) RAM
// Asigna la persona i = popcount(mask) a la tarea j no
// usada.
ll subset_assignment_dp(const vector<vll>& cost) {
    int n = sz(cost);
    int full = (1 << n);
    vll dp(full, INF);
    dp[0] = 0;
    for (int mask = 0; mask < full - 1; ++mask) {
        if (dp[mask] >= INF) continue;
        int i = __builtin_popcount(mask);
        for (int j = 0; j < n; ++j) {
            if (!(mask & (1 << j))) {
                int nxt = mask | (1 << j);
                dp[nxt] =
                    min(dp[nxt], dp[mask] + cost[i][j]);
            }
        }
    }
    return dp[full - 1];
}
