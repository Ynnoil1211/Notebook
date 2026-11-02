// 1. Knapsack 0/1 en O(N * W) tiempo y O(W) memoria
ll knapsack_01(int W, const vi& weight, const vll& val) {
  vll dp(W + 1, 0);
  for (int i = 0; i < sz(weight); ++i)
    // Hacia adelante si Unbounded
    for (int w = W; w >= weight[i]; --w)
      dp[w] = max(dp[w], dp[w - weight[i]] + val[i]);
  return dp[W];
}
