// 4. Poligonos: Area (Shoelace), Centroide y Punto en
// Poligono O(N)
ld polygon_area(const vector<Pt>& p) {
    ld a = 0;
    for (int i = 0; i < sz(p); ++i)
        a += cross(p[i], p[(i + 1) % sz(p)]);
    // Sin dividir entre 2 es siempre entero en coords
    // enteras
    return abs(a) / 2.0;
}
// Retorna: 2 si esta en el BORDE, 1 si esta ESTRICTAMENTE
// DENTRO, 0 si FUERA
int point_in_polygon(const Pt& q, const vector<Pt>& p) {
    int n = sz(p), wn = 0;
    for (int i = 0; i < n; ++i) {
        Pt a = p[i], b = p[(i + 1) % n];
        if (on_segment(q, a, b)) return 2;
        if (a.y <= q.y) {
            if (b.y > q.y && cross(a, b, q) > EPS) wn++;
        } else {
            if (b.y <= q.y && cross(a, b, q) < -EPS) wn--;
        }
    }
    return wn != 0 ? 1 : 0;
}

// Centroide de un poligono simple de area no nula en O(N):
Pt polygon_centroid(const vector<Pt>& p) {
    Pt c(0, 0);
    ld a2 = 0;
    for (int i = 0; i < sz(p); ++i) {
        ld cr = cross(p[i], p[(i + 1) % sz(p)]);
        c = c + (p[i] + p[(i + 1) % sz(p)]) * cr;
        a2 += cr;
    }
    return c / (3.0L * a2);
}

// Punto en Poligono Convexo CCW en O(log N) (sin puntos
// colineales redundantes):
// Retorna: 2 en BORDE, 1 ESTRICTAMENTE DENTRO, 0 FUERA.
int point_in_convex_polygon(const Pt& q,
                            const vector<Pt>& p) {
    int n = sz(p);
    if (n < 3) return 0;
    if (cross(p[0], p[1], q) < -EPS ||
        cross(p[0], p[n - 1], q) > EPS)
        return 0;
    if (on_segment(q, p[0], p[1]) ||
        on_segment(q, p[0], p[n - 1]))
        return 2;
    int l = 1, r = n - 2, idx = 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (cross(p[0], p[mid], q) >= -EPS) {
            idx = mid;
            l = mid + 1;
        } else r = mid - 1;
    }
    ld c = cross(p[idx], p[idx + 1], q);
    if (c < -EPS) return 0;
    if (on_segment(q, p[idx], p[idx + 1])) return 2;
    return 1;
}

// Kernel de Puntos Enteros con cross y doble area en i128
// (sin error de punto flotante):
struct PtLL {
    ll x, y;
    PtLL(ll x = 0, ll y = 0) : x(x), y(y) {}
    PtLL operator+(const PtLL& o) const {
        return {x + o.x, y + o.y};
    }
    PtLL operator-(const PtLL& o) const {
        return {x - o.x, y - o.y};
    }
    bool operator<(const PtLL& o) const {
        return x != o.x ? x < o.x : y < o.y;
    }
    bool operator==(const PtLL& o) const {
        return x == o.x && y == o.y;
    }
};
inline __int128 cross128(const PtLL& a, const PtLL& b) {
    return (__int128)a.x * b.y - (__int128)a.y * b.x;
}
inline __int128 cross128(const PtLL& a, const PtLL& b,
                         const PtLL& c) {
    return cross128(b - a, c - a);
}
inline __int128 polygon_area2_int(const vector<PtLL>& p) {
    __int128 a2 = 0;
    for (int i = 0; i < sz(p); ++i)
        a2 += cross128(p[i], p[(i + 1) % sz(p)]);
    return a2 < 0 ? -a2 : a2;
}
