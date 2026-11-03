// 3. Patrones de Prefijos: Subarreglos con Suma = K,
// Divisibles por M,
// Prefix XOR = K y Subarreglo de Suma Maxima (Kadane) en
// O(N).
ll count_subarrays_sum_k(const vll& a, ll k) {
    safe_umap<ll, int> freq;
    freq[0] = 1;
    ll pref = 0, ans = 0;
    for (ll x : a) {
        pref += x;
        auto it = freq.find(pref - k);
        if (it != freq.end()) ans += it->se;
        freq[pref]++;
    }
    return ans;
}

ll count_subarrays_div_m(const vll& a, ll m) {
    vi freq(m, 0);
    freq[0] = 1;
    ll pref = 0, ans = 0;
    for (ll x : a) {
        pref = ((pref + x) % m + m) % m;
        ans += freq[pref]++;
    }
    return ans;
}

ll count_subarrays_xor_k(const vi& a, int k) {
    safe_umap<int, int> freq;
    freq[0] = 1;
    int px = 0;
    ll ans = 0;
    for (int x : a) {
        px ^= x;
        auto it = freq.find(px ^ k);
        if (it != freq.end()) ans += it->se;
        freq[px]++;
    }
    return ans;
}

// Kadane: mayor suma de un subarreglo contiguo NO vacio
// (admite todo < 0)
ll kadane_max_subarray(const vll& a) {
    assert(!a.empty());
    ll cur = a[0], best = a[0];
    for (int i = 1; i < sz(a); ++i) {
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}
