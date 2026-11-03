// 2. Principio de Inclusion-Exclusion en [1, N]:
// Cuenta enteros en [1, N] divisibles por al menos un d in
// divs.
ll inclusion_exclusion_multiples(ll N, const vll& divs) {
    int k = sz(divs);
    ll ans = 0;
    for (int mask = 1; mask < (1 << k); ++mask) {
        ll lcm_val = 1;
        int bits = __builtin_popcount(mask);
        for (int i = 0; i < k; ++i) {
            if ((mask >> i) & 1) {
                ll g = std::gcd(lcm_val, divs[i]);
                __int128 nxt =
                    (__int128)(lcm_val / g) * divs[i];
                if (nxt > N) {
                    lcm_val = N + 1;
                    break;
                }
                lcm_val = (ll)nxt;
            }
        }
        if (lcm_val <= N) {
            ll cnt = N / lcm_val;
            ans += (bits & 1) ? cnt : -cnt;
        }
    }
    return ans;
}
