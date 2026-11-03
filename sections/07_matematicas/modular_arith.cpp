constexpr ll MOD = 1000000007LL;

inline ll norm_mod(ll x, ll m = MOD) {
    x %= m;
    return (x < 0) ? (x + m) : x;
}
inline ll add_mod(ll a, ll b, ll m = MOD) {
    ll res = norm_mod(a, m) + norm_mod(b, m);
    return (res >= m) ? (res - m) : res;
}
inline ll sub_mod(ll a, ll b, ll m = MOD) {
    ll res = norm_mod(a, m) - norm_mod(b, m);
    return (res < 0) ? (res + m) : res;
}
inline ll mul_mod(ll a, ll b, ll m = MOD) {
    return (ll)((__int128)norm_mod(a, m) * norm_mod(b, m) %
                m);
}
ll binpow(ll base, ll exp, ll m = MOD) {
    if (m == 1) return 0;
    base = norm_mod(base, m);
    ll res = 1;
    while (exp > 0) {
        if (exp & 1) res = mul_mod(res, base, m);
        base = mul_mod(base, base, m);
        exp >>= 1;
    }
    return res;
}
// Inverso cuando m es PRIMO (Fermat):
inline ll inv_fermat(ll a, ll m = MOD) {
    return binpow(a, m - 2, m);
}

// Inverso cuando m es COMPUESTO (requiere m > 1 y gcd(a, m)
// == 1):
ll inv_extgcd(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    ll g = extgcd(norm_mod(a, m), m, x, y);
    return (g == 1) ? norm_mod(x, m) : -1;
}
