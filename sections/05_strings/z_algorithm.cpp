// 1. Z-Algorithm: z[i] = LCP(s[0..n-1], s[i..n-1])
vi z_function(const string& s) {
    int n = sz(s), l = 0, r = 0;
    vi z(n, 0);
    for (int i = 1; i < n; ++i) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
