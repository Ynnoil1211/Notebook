// 4. Linealidad de la Esperanza: Coupon Collector E[pasos]
// = N * H_N
ld expected_coupon_collector(int n) {
    ld h = 0;
    for (int i = 1; i <= n; ++i) h += 1.0L / i;
    return n * h;
}
