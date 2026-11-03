// 1. Proyeccion, Reflexion y Distancia Punto-Recta infinita
// (a -> b)
Pt proj_point_line(const Pt& p, const Pt& a, const Pt& b) {
    if (pt_eq_eps(a, b)) return a;
    Pt v = b - a;
    return a + v * (dot(p - a, v) / v.norm2());
}
Pt reflect_point_line(const Pt& p, const Pt& a,
                      const Pt& b) {
    Pt pr = proj_point_line(p, a, b);
    return pr * 2.0L - p;
}
ld dist_point_line(const Pt& p, const Pt& a, const Pt& b) {
    if (pt_eq_eps(a, b)) return (p - a).norm();
    return abs(cross(a, b, p)) / (b - a).norm();
}
