// 0. Trie de Strings (Conteo de prefijos y palabras exactas
// en O(|S|))
struct StringTrie {
  struct Node { int next[26], pref = 0, end_w = 0; Node() {
    fill(begin(next), end(next), -1); } };
  vector<Node> t = {Node()};
  void insert(const string& s) {
    int u = 0; t[u].pref++;
    for (char c : s) {
      int ch = c - 'a';
      if (t[u].next[ch] == -1) { t[u].next[ch] = sz(t);
        t.emplace_back(); }
      u = t[u].next[ch]; t[u].pref++;
    }
    t[u].end_w++;
  }
  int count_prefix(const string& p) const {
    int u = 0;
    for (char c : p) {
      int ch = c - 'a';
      if (t[u].next[ch] == -1) return 0;
      u = t[u].next[ch];
    }
    return t[u].pref;
  }
};
