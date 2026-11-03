// 2. Union de intervalos cerrados [l, r] y longitud
// cubierta
pair<vector<pll>, ll> merge_intervals(vector<pll> ivs) {
    if (ivs.empty()) return {{}, 0};
    sort(all(ivs));
    vector<pll> res = {ivs[0]};
    for (int i = 1; i < sz(ivs); ++i) {
        if (ivs[i].fi <= res.back().se)
            res.back().se = max(res.back().se, ivs[i].se);
        else res.pb(ivs[i]);
    }
    ll total_len = 0;
    for (auto [l, r] : res) total_len += (r - l);
    return {res, total_len};
}
