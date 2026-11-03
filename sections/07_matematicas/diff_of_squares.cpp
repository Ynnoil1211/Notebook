// 2. Expresar N = a^2 - b^2 (retorna pares {a, b} >= 0)
vector<pll> solve_diff_of_squares(ll n) {
    vector<pll> sols;
    for (ll d1 = 1; d1 * d1 <= n; ++d1) {
        if (n % d1 == 0) {
            ll d2 = n / d1;
            if ((d1 + d2) % 2 == 0)
                sols.pb({(d1 + d2) / 2, (d2 - d1) / 2});
        }
    }
    return sols;
}
