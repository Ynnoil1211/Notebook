// 1. Funcion Prefijo pi[0..n-1] en O(N)
vi prefix_function(const string& s) {
  int n = sz(s);
  vi pi(n, 0);
  for (int i = 1; i < n; ++i) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}

// 2. Buscar todos los indices 0-based donde 'pat' ocurre en
// 'text'
vi kmp_search(const string& text, const string& pat) {
  vi pi = prefix_function(pat), matches;
  int m = sz(pat), j = 0;
  for (int i = 0; i < sz(text); ++i) {
    while (j > 0 && text[i] != pat[j]) j = pi[j - 1];
    if (text[i] == pat[j]) j++;
    if (j == m) {
      matches.pb(i - m + 1);
      j = pi[j - 1];
    }
  }
  return matches;
}
