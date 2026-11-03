// 1. Estructura Base de Punto / Vector 2D (usar ld para
// general o ll para exacto)
struct Pt {
    ld x, y;
    Pt(ld x = 0, ld y = 0) : x(x), y(y) {}
    Pt operator+(const Pt& o) const {
        return {x + o.x, y + o.y};
    }
    Pt operator-(const Pt& o) const {
        return {x - o.x, y - o.y};
    }
    Pt operator*(ld k) const { return {x * k, y * k}; }
    Pt operator/(ld k) const { return {x / k, y / k}; }
    // Orden lexicografico exacto para cumplir strict weak
    // ordering en sort/set:
    bool operator<(const Pt& o) const {
        return x != o.x ? x < o.x : y < o.y;
    }
    bool operator==(const Pt& o) const {
        return x == o.x && y == o.y;
    }
    ld norm2() const { return x * x + y * y; }
    ld norm() const { return hypot(x, y); }
    // Rotacion 90 grados CCW
    Pt perp() const { return {-y, x}; }
    // Rotacion CCW por 'ang_rad'
    Pt rot(ld ang_rad) const {
        return {x * cos(ang_rad) - y * sin(ang_rad),
                x * sin(ang_rad) + y * cos(ang_rad)};
    }
};

int sgn(ld x) { return (x > EPS) - (x < -EPS); }
bool pt_eq_eps(const Pt& a, const Pt& b) {
    return abs(a.x - b.x) <= EPS && abs(a.y - b.y) <= EPS;
}
ld dot(const Pt& a, const Pt& b) {
    return a.x * b.x + a.y * b.y;
}
ld cross(const Pt& a, const Pt& b) {
    return a.x * b.y - a.y * b.x;
}
// Giro de a -> b -> c: > 0 izquierda (CCW), < 0 derecha
// (CW), 0 colineal
ld cross(const Pt& a, const Pt& b, const Pt& c) {
    return cross(b - a, c - a);
}

// 2. Orden Radial (Polar Sort alrededor del origen (0, 0)
// en O(N log N))
int half_plane(const Pt& p) {
    return (p.y > 0 || (p.y == 0 && p.x > 0)) ? 0 : 1;
}
bool polar_cmp(const Pt& a, const Pt& b) {
    int ha = half_plane(a), hb = half_plane(b);
    return (ha != hb) ? (ha < hb) : (cross(a, b) > 0);
}
