// 2. Base de XOR (Gauss Binario en O(B) por insercion)
struct XorBasis {
    static const int B = 60;
    ll basis[B + 1] = {0};
    int sz_b = 0;

    bool insert(ll x) {
        for (int i = B; i >= 0; --i) {
            if (!((x >> i) & 1LL)) continue;
            if (!basis[i]) {
                basis[i] = x;
                sz_b++;
                return true;
            }
            x ^= basis[i];
        }
        return false;  // x ya era linealmente dependiente
    }
    // Maximo valor posible de (initial_val ^ subconjunto)
    ll max_xor(ll ans = 0) const {
        for (int i = B; i >= 0; --i)
            ans = max(ans, ans ^ basis[i]);
        return ans;
    }
    bool can_form(ll x) const {
        for (int i = B; i >= 0; --i)
            if ((x >> i) & 1LL) x ^= basis[i];
        return x == 0;
    }
};
