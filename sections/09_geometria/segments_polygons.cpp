// 3. Segmentos y Rectas: Punto en segmento, Distancia e
// Interseccion
bool on_segment(const Pt& p, const Pt& a, const Pt& b) {
    return sgn(cross(a, b, p)) == 0 &&
           sgn(dot(a - p, b - p)) <= 0;
}
ld dist_point_segment(const Pt& p, const Pt& a,
                      const Pt& b) {
    if (pt_eq_eps(a, b)) return (p - a).norm();
    ld d2 = (b - a).norm2();
    ld t = clamp(dot(p - a, b - a) / d2, (ld)0.0, (ld)1.0);
    Pt proj = a + (b - a) * t;
    return (p - proj).norm();
}
bool segments_intersect(const Pt& a, const Pt& b,
                        const Pt& c, const Pt& d) {
    int d1 = sgn(cross(a, b, c)), d2 = sgn(cross(a, b, d));
    int d3 = sgn(cross(c, d, a)), d4 = sgn(cross(c, d, b));
    if (d1 * d2 < 0 && d3 * d4 < 0) return true;
    return on_segment(c, a, b) || on_segment(d, a, b) ||
           on_segment(a, c, d) || on_segment(b, c, d);
}
// Interseccion de dos rectas infinitas (a->b) y (c->d)
// (asume no paralelas)
Pt line_intersection(const Pt& a, const Pt& b, const Pt& c,
                     const Pt& d) {
    return a + (b - a) * (cross(c - a, d - c) /
                          cross(b - a, d - c));
}
