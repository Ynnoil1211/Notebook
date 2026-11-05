// 4. Fast Fourier Transform (FFT — Multiplicacion de
// Polinomios en O(N log N))
using cd = complex<double>;
void fft(vector<cd>& a, bool inv) {
    int n = sz(a);
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (inv ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (inv)
        for (cd& x : a) x /= n;
}
vll multiply_poly(const vll& a, const vll& b) {
    vector<cd> fa(all(a)), fb(all(b));
    int n = 1;
    while (n < sz(a) + sz(b)) n <<= 1;
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i) fa[i] *= fb[i];
    fft(fa, true);
    vll res(sz(a) + sz(b) - 1);
    for (int i = 0; i < sz(res); ++i)
        res[i] = llround(fa[i].real());
    return res;
}
