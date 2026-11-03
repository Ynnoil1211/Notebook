// Combinatoria O(N) precalculo, O(1) consulta:
// Recurrencias: fact[i] = fact[i-1] * i % m
//   inv_num[i] = (m - (m/i) * inv_num[m%i] % m) % m
//   inv_fact[i] = inv_fact[i-1] * inv_num[i] % m
const int MAX_F = 1000005;
ll fact[MAX_F], inv_fact[MAX_F], inv_num[MAX_F];
void init_fact(int n = MAX_F - 1, ll m = MOD) {
    fact[0] = inv_fact[0] = 1;
    inv_num[0] = 0;
    inv_num[1] = 1;
    for (int i = 1; i <= n; ++i)
        fact[i] = (fact[i - 1] * i) % m;
    inv_fact[n] = inv_fermat(fact[n], m);
    for (int i = n - 1; i >= 1; --i)
        inv_fact[i] = (inv_fact[i + 1] * (i + 1)) % m;
    for (int i = 2; i <= n; ++i)
        inv_num[i] = (m - (m / i) * inv_num[m % i] % m) % m;
}
inline ll nCr(int n, int r, ll m = MOD) {
    if (r < 0 || r > n || n < 0) return 0;
    return fact[n] * inv_fact[r] % m * inv_fact[n - r] % m;
}
inline ll nPr(int n, int r, ll m = MOD) {
    if (r < 0 || r > n || n < 0) return 0;
    return fact[n] * inv_fact[n - r] % m;
}
// Producto recurrente O(r) sin tablas:
// C(n, r) = prod_{i=1..r} (n - r + i) / i
ll nCr_iter(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    r = min(r, n - r);
    ll res = 1;
    for (ll i = 1; i <= r; ++i) res = res * (n - r + i) / i;
    return res;
}

// Teorema de Lucas: C(n, r) = prod_{i>=0} C(n_i, r_i) mod p
// (Requiere haber llamado init_fact(p - 1, p))
ll lucas_nCr(ll n, ll r, ll p) {
    if (r < 0 || r > n) return 0;
    if (r == 0) return 1;
    return lucas_nCr(n / p, r / p, p) *
           nCr(n % p, r % p, p) % p;
}

// Formula de Legendre: v_p(n!) = sum_{k>=1} floor(n / p^k)
ll legendre_prime_in_fact(ll n, ll p) {
    ll cnt = 0;
    while (n > 0) {
        cnt += n / p;
        n /= p;
    }
    return cnt;
}
