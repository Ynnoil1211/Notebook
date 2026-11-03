// 1. Interval Scheduling: maximo numero de intervalos
// disjuntos
int interval_scheduling(vector<pll> ivs,
                        bool strict_disjoint = false) {
    sort(all(ivs), [](const pll& a, const pll& b) {
        return a.se != b.se ? a.se < b.se : a.fi < b.fi;
    });
    int cnt = 0;
    ll last_r = -INF;
    for (auto [l, r] : ivs) {
        if (strict_disjoint ? (l > last_r)
                            : (l >= last_r)) {
            cnt++;
            last_r = r;
        }
    }
    return cnt;
}
