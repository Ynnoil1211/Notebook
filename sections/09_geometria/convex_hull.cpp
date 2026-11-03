// 5. Convex Hull (Monotone Chain de Andrew en O(N log N) —
// orden CCW)
vector<Pt> convex_hull(vector<Pt> pts) {
    int n = sz(pts), k = 0;
    if (n <= 1) return pts;
    sort(all(pts));
    pts.erase(unique(all(pts), pt_eq_eps), pts.end());
    n = sz(pts);
    if (n <= 2) return pts;
    vector<Pt> h(2 * n);
    for (int i = 0; i < n; ++i) {
        while (k >= 2 &&
               cross(h[k - 2], h[k - 1], pts[i]) <= EPS)
            k--;
        h[k++] = pts[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t &&
               cross(h[k - 2], h[k - 1], pts[i]) <= EPS)
            k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}
