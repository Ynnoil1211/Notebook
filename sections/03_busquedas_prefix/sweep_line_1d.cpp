// 1. Maxima cantidad de intervalos solapados en un punto
//    closed_ends = true para [l, r]; false para [l, r).
int max_interval_overlap(const vector<pll>& ivs,
                         bool closed_ends = true) {
    vector<pair<ll, int>> ev;
    ev.reserve(2 * sz(ivs));
    for (auto [l, r] : ivs) {
        ev.pb({l, +1});
        ev.pb({r, -1});
    }
    sort(all(ev), [&](const auto& a, const auto& b) {
        if (a.fi != b.fi) return a.fi < b.fi;
        // En [l, r] entra (+1) antes de salir (-1);
        // en [l, r) sale (-1) antes de entrar (+1).
        return closed_ends ? (a.se > b.se) : (a.se < b.se);
    });
    int cur = 0, best = 0;
    for (auto [x, d] : ev) {
        cur += d;
        best = max(best, cur);
    }
    return best;
}
