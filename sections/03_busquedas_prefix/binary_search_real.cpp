// 3. Binary Search continua en double (iteraciones fijas)
template <typename F>
double bs_double(double l, double r, F check, int it = 90) {
  for (int i = 0; i < it; ++i) {
    double mid = l + (r - l) / 2.0;
    // Si maximiza (cambiar a r=mid si minimiza)
    if (check(mid)) l = mid;
    else r = mid;
  }
  return l;
}
