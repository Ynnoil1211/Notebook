// 5. Teorema Chino del Resto General (CRT para modulos NO
// necesariamente coprimos)
// Recibe vectores 'r' y 'm' (x == r[i] mod m[i]).
// Retorna {x_minimo_no_negativo, lcm_total}, o {-1, -1} si
// es incompatible.
pll crt_general(const vll& r, const vll& m) {
  ll r0 = 0, m0 = 1;
  for (int i = 0; i < sz(r); ++i) {
    ll r1 = ((r[i] % m[i]) + m[i]) % m[i], m1 = m[i];
    ll x, y;
    ll g = extgcd(m0, m1, x, y);
    if ((r1 - r0) % g != 0) return {-1, -1}; // Sin solucion
    ll mod_step = m1 / g;
    ll diff = ((r1 - r0) / g) % mod_step;
    if (diff < 0) diff += mod_step;
    ll inv_part = (x % mod_step + mod_step) % mod_step;
    ll step = (ll)((__int128)diff * inv_part % mod_step);
    __int128 lcm128 = (__int128)m0 * mod_step;
    ll lcm = (ll)lcm128;
    r0 = (ll)((r0 + (__int128)step * m0) % lcm128);
    if (r0 < 0) r0 += lcm;
    m0 = lcm;
  }
  return {r0, m0};
}
