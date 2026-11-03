// 6. Rotating Callipers: Mayor distancia al cuadrado en un
// poligono convexo CCW
ld rotating_callipers_max_dist2(const vector<Pt>& h) {
    int n = sz(h);
    if (n <= 1) return 0;
    if (n == 2) return (h[0] - h[1]).norm2();
    ld max_d2 = 0;
    for (int i = 0, j = 1; i < n; ++i) {
        Pt edge = h[(i + 1) % n] - h[i];
        while (cross(edge, h[(j + 1) % n] - h[i]) >
               cross(edge, h[j] - h[i]))
            j = (j + 1) % n;
        max_d2 = max({max_d2, (h[i] - h[j]).norm2(),
                      (h[(i + 1) % n] - h[j]).norm2()});
    }
    return max_d2;
}
