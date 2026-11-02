mt19937_64 rng(
  chrono::steady_clock::now().time_since_epoch().count()
);

// Entero aleatorio uniforme en el rango cerrado [l, r]
ll rd(ll l, ll r) {
  return uniform_int_distribution<ll>(l, r)(rng);
}

// Real aleatorio uniforme en [l, r]
double rd_real(double l, double r) {
  return uniform_real_distribution<double>(l, r)(rng);
}

// Barajar un vector aleatoriamente:
// shuffle(all(v), rng);
