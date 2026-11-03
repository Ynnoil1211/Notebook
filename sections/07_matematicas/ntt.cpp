// 4b. Number Theoretic Transform (NTT exacto mod 998244353,
// raiz g = 3)
// Requisitos: modulo primo de la forma p = c * 2^k + 1
// (998244353 = 119 * 2^23 + 1),
// longitud n potencia de 2 con n <= 2^23. A diferencia de
// FFT (double), NTT no sufre
// error de redondeo pero calcula los coeficientes modulo
// NTT_MOD.
const ll NTT_MOD = 998244353;
const ll NTT_G = 3;

ll ntt_pow(ll b, ll e) {
    ll r = 1;
    for (b %= NTT_MOD; e > 0; e >>= 1, b = b * b % NTT_MOD)
        if (e & 1) r = r * b % NTT_MOD;
    return r;
}
void ntt(vll& a, bool inv) {
    int n = sz(a);
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll wlen = ntt_pow(NTT_G, (NTT_MOD - 1) / len);
        if (inv) wlen = ntt_pow(wlen, NTT_MOD - 2);
        for (int i = 0; i < n; i += len) {
            ll w = 1;
            for (int j = 0; j < len / 2; ++j) {
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * w % NTT_MOD;
                a[i + j] = (u + v < NTT_MOD)
                               ? (u + v)
                               : (u + v - NTT_MOD);
                a[i + j + len / 2] =
                    (u - v >= 0) ? (u - v)
                                 : (u - v + NTT_MOD);
                w = w * wlen % NTT_MOD;
            }
        }
    }
    if (inv) {
        ll inv_n = ntt_pow(n, NTT_MOD - 2);
        for (ll& x : a) x = x * inv_n % NTT_MOD;
    }
}
vll multiply_ntt(vll a, vll b) {
    if (a.empty() || b.empty()) return {};
    int need = sz(a) + sz(b) - 1, n = 1;
    while (n < need) n <<= 1;
    for (ll& x : a) x = (x % NTT_MOD + NTT_MOD) % NTT_MOD;
    for (ll& x : b) x = (x % NTT_MOD + NTT_MOD) % NTT_MOD;
    a.resize(n, 0);
    b.resize(n, 0);
    ntt(a, false);
    ntt(b, false);
    for (int i = 0; i < n; ++i)
        a[i] = a[i] * b[i] % NTT_MOD;
    ntt(a, true);
    a.resize(need);
    return a;
}
