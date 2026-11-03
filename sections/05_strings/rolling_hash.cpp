struct PolyHash {
    static const ll M1 = 1000000007LL;
    static const ll M2 = 1000000009LL;
    static const ll BASE = 911382323LL;
    int n;
    vll h1, h2, p1, p2;
    string s;

    PolyHash(const string& str)
        : n(sz(str)), h1(n + 1, 0), h2(n + 1, 0),
          p1(n + 1, 1), p2(n + 1, 1), s(str) {
        for (int i = 0; i < n; ++i) {
            p1[i + 1] = (p1[i] * BASE) % M1;
            p2[i + 1] = (p2[i] * BASE) % M2;
            h1[i + 1] = (h1[i] * BASE + s[i]) % M1;
            h2[i + 1] = (h2[i] * BASE + s[i]) % M2;
        }
    }
    // Hash de s[l..r] (0-indexed, inclusivo) en O(1)
    pll get(int l, int r) const {
        ll r1 = (h1[r + 1] - h1[l] * p1[r - l + 1]) % M1;
        if (r1 < 0) r1 += M1;
        ll r2 = (h2[r + 1] - h2[l] * p2[r - l + 1]) % M2;
        if (r2 < 0) r2 += M2;
        return {r1, r2};
    }
    // Longitud del mayor prefijo comun entre s[i..n-1] y
    // s[j..n-1] en O(log N)
    int lcp(int i, int j) const {
        int low = 1, high = n - max(i, j), ans = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (get(i, i + mid - 1) ==
                get(j, j + mid - 1)) {
                ans = mid;
                low = mid + 1;
            } else high = mid - 1;
        }
        return ans;
    }
    // Retorna true si s[l1..r1] < s[l2..r2]
    // lexicograficamente en O(log N)
    bool cmp_substr(int l1, int r1, int l2, int r2) const {
        int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
        int common = min({lcp(l1, l2), len1, len2});
        if (common == len1 || common == len2)
            return len1 < len2;
        return s[l1 + common] < s[l2 + common];
    }
};
