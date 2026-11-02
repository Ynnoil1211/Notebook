// 4. Criba Segmentada en [L, R] para R <= 10^12 y (R - L +
// 1) <= 10^7
// Retorna vector<bool> donde res[x - L] == true si y solo
// si x es primo.
vector<bool> segmented_sieve(ll L, ll R) {
  ll lim = sqrt(R);
  vector<bool> mark(lim + 1, false);
  vll pr;
  for (ll i = 2; i <= lim; ++i) {
    if (!mark[i]) {
      pr.pb(i);
      for (ll j = i * i; j <= lim; j += i) mark[j] = true;
    }
  }
  vector<bool> res(R - L + 1, true);
  for (ll p : pr) {
    ll start = max(p * p, (L + p - 1) / p * p);
    for (ll j = start; j <= R; j += p) res[j - L] = false;
  }
  for (ll x = max(L, 0LL); x <= min(R,
    1LL); ++x) res[x - L] = false;
  return res;
}
