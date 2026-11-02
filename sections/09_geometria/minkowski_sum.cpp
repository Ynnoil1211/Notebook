// Suma de Minkowski P + Q para dos conjuntos convexos CCW
// (admite punto o segmento)
vector<Pt> minkowski_sum(vector<Pt> P, vector<Pt> Q) {
  if (P.empty() || Q.empty()) return {};
  if (sz(P) == 1) {
    for (auto& q : Q) q = q + P[0];
    return Q;
  }
  if (sz(Q) == 1) {
    for (auto& p : P) p = p + Q[0];
    return P;
  }
  auto reorder = [](vector<Pt>& pts) {
    int pos = 0;
    for (int i = 1; i < sz(pts); ++i)
      if (pts[i].y < pts[pos].y || (
        pts[i].y == pts[pos].y && pts[i].x < pts[pos].x))
        pos = i;
    rotate(pts.begin(), pts.begin() + pos, pts.end());
  };
  reorder(P); reorder(Q);
  int n = sz(P), m = sz(Q);
  P.push_back(P[0]); P.push_back(P[1]);
  Q.push_back(Q[0]); Q.push_back(Q[1]);

  vector<Pt> res;
  int i = 0, j = 0;
  while (i < n || j < m) {
    res.push_back(P[i] + Q[j]);
    ld c = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
    bool adv_i = (i < n) && (j == m || c >= -EPS);
    bool adv_j = (j < m) && (i == n || c <= EPS);
    if (adv_i) i++;
    if (adv_j) j++;
  }
  return res;
}
