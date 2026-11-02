// 1. Maximizar respuesta discreta: [True, True, ..., False,
// False]
template <typename F>
ll bs_max(ll low, ll high, F check) {
  ll ans = low - 1; // Centinela si ninguno cumple
  while (low <= high) {
    ll mid = low + (high - low) / 2;
    if (check(mid)) {
      ans = mid;      // mid sirve, probar mayores
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return ans;
}

// 2. Minimizar respuesta discreta: [False, False, ...,
// True, True]
template <typename F>
ll bs_min(ll low, ll high, F check) {
  ll ans = high + 1;
  while (low <= high) {
    ll mid = low + (high - low) / 2;
    if (check(mid)) {
      ans = mid;      // mid sirve, probar menores
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return ans;
}
