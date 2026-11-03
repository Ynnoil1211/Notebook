// Semiplanos a la izquierda del rayo p -> p + v
struct Halfplane {
    Pt p, v;
    ld angle;
    Halfplane() {}
    Halfplane(Pt a, Pt b)
        : p(a), v(b - a), angle(atan2(v.y, v.x)) {}
    bool out(const Pt& q) const {
        return cross(v, q - p) < -EPS;
    }
    bool operator<(const Halfplane& o) const {
        if (abs(angle - o.angle) > EPS)
            return angle < o.angle;
        // Mas restrictivo primero si son paralelos
        return cross(v, o.p - p) < 0;
    }
};

Pt halfplane_intersect(const Halfplane& a,
                       const Halfplane& b) {
    ld c = cross(a.v, b.v);
    return a.p + a.v * (cross(b.p - a.p, b.v) / c);
}

// Retorna poligono convexo CCW (vacio si la interseccion es
// vacia o degenerada).
// Si box > 0, acota regiones abiertas al cuadrado [-box,
// box]^2.
vector<Pt> halfplane_intersection(vector<Halfplane> h,
                                  ld box = 1e9) {
    if (box > 0) {
        h.push_back({{-box, -box}, {box, -box}});
        h.push_back({{box, -box}, {box, box}});
        h.push_back({{box, box}, {-box, box}});
        h.push_back({{-box, box}, {-box, -box}});
    }
    sort(all(h));
    deque<Halfplane> dq;
    for (const auto& cur : h) {
        if (!dq.empty() &&
            abs(cur.angle - dq.back().angle) <= EPS)
            continue;
        while (sz(dq) >= 2 &&
               cur.out(halfplane_intersect(dq[sz(dq) - 2],
                                           dq.back())))
            dq.pop_back();
        while (sz(dq) >= 2 &&
               cur.out(halfplane_intersect(dq[0], dq[1])))
            dq.pop_front();
        if (!dq.empty() &&
            abs(cross(cur.v, dq.back().v)) <= EPS) {
            if (dot(cur.v, dq.back().v) < 0 &&
                cur.out(dq.back().p))
                return {};
        }
        dq.push_back(cur);
    }
    while (sz(dq) >= 3 && dq[0].out(halfplane_intersect(
                              dq[sz(dq) - 2], dq.back())))
        dq.pop_back();
    while (sz(dq) >= 3 &&
           dq.back().out(halfplane_intersect(dq[0], dq[1])))
        dq.pop_front();
    if (sz(dq) < 3) return {};

    vector<Pt> poly;
    for (int i = 0; i < sz(dq); ++i) {
        const auto& a = dq[i];
        const auto& b = dq[(i + 1) % sz(dq)];
        if (abs(cross(a.v, b.v)) <= EPS) return {};
        poly.push_back(halfplane_intersect(a, b));
    }
    return convex_hull(poly);
}
