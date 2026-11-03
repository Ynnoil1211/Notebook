// 2. Circulos: Interseccion Recta-Circulo, Circulo-Circulo
// y Tangentes
struct Circle {
    Pt c;
    ld r;
};

vector<Pt> circle_line_intersection(const Circle& C,
                                    const Pt& a,
                                    const Pt& b) {
    if (pt_eq_eps(a, b)) return {};
    Pt pr = proj_point_line(C.c, a, b);
    ld d2 = (pr - C.c).norm2(), r2 = C.r * C.r;
    if (d2 > r2 + EPS) return {};
    ld h = sqrt(max((ld)0.0, r2 - d2));
    Pt dir = (b - a) / (b - a).norm();
    if (h <= EPS) return {pr};
    return {pr - dir * h, pr + dir * h};
}

// Retorna {estado, puntos}: -1 coincidentes, 0 disjuntos, 1
// tangente, 2 secantes
pair<int, vector<Pt>> circle_circle_intersection(
    const Circle& A, const Circle& B) {
    Pt v = B.c - A.c;
    ld d = v.norm();
    if (d <= EPS)
        return (abs(A.r - B.r) <= EPS)
                   ? make_pair(-1, vector<Pt>{})
                   : make_pair(0, vector<Pt>{});
    if (d > A.r + B.r + EPS ||
        d + min(A.r, B.r) < max(A.r, B.r) - EPS)
        return {0, {}};
    ld x = (d * d + A.r * A.r - B.r * B.r) / (2.0L * d);
    ld h2 = A.r * A.r - x * x;
    Pt base = A.c + v * (x / d);
    if (h2 <= EPS) return {1, {base}};
    ld h = sqrt(max((ld)0.0, h2));
    Pt nrm = v.perp() * (h / d);
    return {2, {base - nrm, base + nrm}};
}

// Puntos de contacto de las tangentes a C trazadas desde un
// punto exterior P
vector<Pt> circle_tangents_from_point(const Circle& C,
                                      const Pt& p) {
    Pt v = p - C.c;
    ld d2 = v.norm2(), r2 = C.r * C.r;
    // Punto estrictamente interior
    if (d2 < r2 - EPS) return {};
    if (abs(d2 - r2) <= EPS) return {p};
    ld x = r2 / d2,
       h = sqrt(max((ld)0.0, d2 - r2)) * C.r / d2;
    Pt base = C.c + v * x, nrm = v.perp() * h;
    return {base - nrm, base + nrm};
}
