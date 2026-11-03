// 1. Totient de Euler individual en O(sqrt(N))
ll euler_phi(ll n) {
    ll ans = n;
    for (ll d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            while (n % d == 0) n /= d;
            ans -= ans / d;
        }
    }
    if (n > 1) ans -= ans / n;
    return ans;
}
