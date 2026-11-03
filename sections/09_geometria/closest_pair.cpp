// 7. Par de Puntos Mas Cercanos (Sweep-Line con std::set en
// O(N log N))
// Precondicion: sz(pts) >= 2 (retorna INF si hay menos de 2
// puntos).
ld closest_pair_distance(vector<Pt> pts) {
    if (sz(pts) < 2) return INF;
    sort(all(pts));
    for (int i = 1; i < sz(pts); ++i)
        if (pts[i] == pts[i - 1]) return 0;
    set<pair<ld, ld>> active;  // Guarda {y, x}
    ld best = INF;
    int j = 0;
    for (int i = 0; i < sz(pts); ++i) {
        while (j < i && pts[i].x - pts[j].x >= best) {
            active.erase({pts[j].y, pts[j].x});
            j++;
        }
        auto it1 =
            active.lower_bound({pts[i].y - best, -INF});
        auto it2 =
            active.upper_bound({pts[i].y + best, INF});
        for (auto it = it1; it != it2; ++it) {
            ld dx = pts[i].x - it->second,
               dy = pts[i].y - it->first;
            best = min(best, hypot(dx, dy));
        }
        if (best == 0) return 0;
        active.insert({pts[i].y, pts[i].x});
    }
    return best;
}
