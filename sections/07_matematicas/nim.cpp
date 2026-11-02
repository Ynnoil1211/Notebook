// 3. Nim Normal (gana quien toma el ultimo) y Misere Nim
// (pierde quien toma el ultimo)
bool nim_normal_win(const vll& piles) {
  ll x = 0;
  for (ll p : piles) x ^= p;
  return x != 0;
}
bool nim_misere_win(const vll& piles) {
  bool all_le_one = true;
  int ones = 0;
  ll x = 0;
  for (ll p : piles) {
    if (p > 1) all_le_one = false;
    if (p == 1) ones++;
    x ^= p;
  }
  return all_le_one ? (ones % 2 == 0) : (x != 0);
}
