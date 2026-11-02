// 1b. LCS (Longest Common Subsequence con reconstruccion)
// en O(N * M)
string lcs_string(const string& a, const string& b) {
  int n = sz(a), m = sz(b);
  vector<vi> dp(n + 1, vi(m + 1, 0));
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
      dp[i][j] = (a[i - 1] == b[j - 1])
                   ? (dp[i - 1][j - 1] + 1)
                   : max(dp[i - 1][j], dp[i][j - 1]);
  string res;
  for (int i = n, j = m; i > 0 && j > 0; ) {
    if (a[i - 1] == b[j - 1]) { res += a[i - 1]; --i; --j; }
    else if (dp[i - 1][j] >= dp[i][j - 1]) --i;
    else --j;
  }
  reverse(all(res));
  return res;
}
