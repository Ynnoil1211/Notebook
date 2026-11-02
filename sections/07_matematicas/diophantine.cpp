// 4. Ecuacion Diofantica Lineal: a*x + b*y = c
// Retorna true si existe solucion; deja una solucion
// particular en (x0, y0).
// Familia general de soluciones: x = x0 + k*(b/g), y = y0 -
// k*(a/g).
bool find_diophantine(ll a, ll b, ll c, ll& x0, ll& y0,
  ll& g) {
  g = extgcd(abs(a), abs(b), x0, y0);
  if (c % g != 0) return false;
  x0 *= c / g;
  y0 *= c / g;
  if (a < 0) x0 = -x0;
  if (b < 0) y0 = -y0;
  return true;
}
