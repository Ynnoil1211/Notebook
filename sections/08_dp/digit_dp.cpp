// 4. Digit DP: Cuenta numeros en [0, R] con suma de digitos
// % K == 0
string dig_s;
int K_mod;
ll memo_dig[20][200][2];

ll dp_dig(int pos, int rem, bool tight) {
  if (pos == sz(dig_s)) return (rem == 0) ? 1 : 0;
  ll& res = memo_dig[pos][rem][tight];
  if (res != -1) return res;
  res = 0;
  int lim = tight ? (dig_s[pos] - '0') : 9;
  for (int d = 0; d <= lim; ++d)
    res += dp_dig(pos + 1, (rem + d) % K_mod, tight && (
      d == lim));
  return res;
}
ll solve_digit_dp(ll R, int K) {
  if (R < 0) return 0;
  dig_s = to_string(R); K_mod = K;
  memset(memo_dig, -1, sizeof(memo_dig));
  return dp_dig(0, 0, true);
}
