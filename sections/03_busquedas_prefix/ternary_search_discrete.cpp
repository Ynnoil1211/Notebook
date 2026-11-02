// 2. Ternary Search Discreta en enteros [low, high]
// (MINIMO)
template <typename F>
ll ternary_discrete(ll low, ll high, F f) {
  while (high - low >= 3) {
    ll m1 = low + (high - low) / 3;
    ll m2 = high - (high - low) / 3;
    if (f(m1) < f(m2)) high = m2;
    else low = m1;
  }
  ll best = f(low);
  for (ll x = low + 1; x <= high; ++x)
    best = min(best, f(x));
  return best;
}
