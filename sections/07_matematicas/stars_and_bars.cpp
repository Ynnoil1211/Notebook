// 1. Stars and Bars: x_1 + ... + x_k = n (requiere
// init_fact() de §7.4)
ll stars_and_bars_nonneg(int n, int k) {  // x_i >= 0
    if (n < 0 || k <= 0) return (n == 0 && k == 0) ? 1 : 0;
    return nCr(n + k - 1, k - 1);
}
ll stars_and_bars_pos(int n, int k) {  // x_i >= 1
    if (n < k || k <= 0) return 0;
    return nCr(n - 1, k - 1);
}
