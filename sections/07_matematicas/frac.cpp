// 7. Estructura de Fracciones Exactas (evita errores de
// precision de double)
struct Frac {
  ll num, den;
  Frac(ll n = 0, ll d = 1) : num(n), den(d) {
    if (den < 0) { num = -num; den = -den; }
    ll g = std::gcd(abs(num), den);
    if (g) { num /= g; den /= g; }
  }
  Frac operator+(const Frac& o) const { return {
    num*o.den + o.num*den, den*o.den}; }
  Frac operator-(const Frac& o) const { return {
    num*o.den - o.num*den, den*o.den}; }
  Frac operator*(const Frac& o) const { return {
    num*o.num, den*o.den}; }
  Frac operator/(const Frac& o) const { return {
    num*o.den, den*o.num}; }
  bool operator<(const Frac& o) const {
    return (__int128)num * o.den < (__int128)o.num * den; }
  bool operator==(const Frac& o) const {
    return num == o.num && den == o.den; }
};
