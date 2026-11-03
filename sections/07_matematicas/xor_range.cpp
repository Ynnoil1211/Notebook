// 1. XOR de 0 a n en O(1)
ll xor_n(ll n) {
    if (n < 0) return 0;
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}
// XOR de todos los enteros en el intervalo cerrado [l, r]
// en O(1)
ll xor_range(ll l, ll r) {
    return (l > r) ? 0 : (xor_n(r) ^ xor_n(l - 1));
}
