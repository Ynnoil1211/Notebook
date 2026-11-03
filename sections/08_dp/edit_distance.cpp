// 2. Edit Distance (Levenshtein) en O(N * M)
int edit_distance(const string& a, const string& b) {
    int n = sz(a), m = sz(b);
    vector<vi> dp(n + 1, vi(m + 1, 0));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;
    for (int j = 0; j <= m; ++j) dp[0][j] = j;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] =
                    1 + min({dp[i - 1][j], dp[i][j - 1],
                             dp[i - 1][j - 1]});
        }
    return dp[n][m];
}
