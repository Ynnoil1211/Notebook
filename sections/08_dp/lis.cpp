// 3. LIS (Longest Increasing Subsequence) en O(N log N)
int lis_length(const vll& a) {
  vll tails;
  for (ll x : a) {
    // upper_bound para <=
    auto it = lower_bound(all(tails), x);
    if (it == tails.end()) tails.pb(x);
    else *it = x;
  }
  return sz(tails);
}
