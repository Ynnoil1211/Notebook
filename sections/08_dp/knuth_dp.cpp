// 3. Knuth's Optimization para DP de Intervalos en O(N^2)
// Resuelve dp[l][r] = min_{l <= k < r} (dp[l][k] +
// dp[k+1][r]) + C(l, r)
ll solve_knuth(int n, const vector<vll>& C) {
    vector<vll> dp(n + 1, vll(n + 1, 0));
    vector<vi> opt(n + 1, vi(n + 1, 0));
    for (int i = 1; i <= n; ++i) opt[i][i] = i;

    for (int len = 2; len <= n; ++len) {
        for (int l = 1; l + len - 1 <= n; ++l) {
            int r = l + len - 1;
            dp[l][r] = INF;
            int kl = opt[l][r - 1],
                kr = min(r - 1, opt[l + 1][r]);
            for (int k = kl; k <= kr; ++k) {
                ll val = dp[l][k] + dp[k + 1][r] + C[l][r];
                if (val < dp[l][r]) {
                    dp[l][r] = val;
                    opt[l][r] = k;
                }
            }
        }
    }
    return dp[1][n];
}
