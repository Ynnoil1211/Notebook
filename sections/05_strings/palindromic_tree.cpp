// 3. Palindromic Tree (Eertree) en O(N)
struct PalindromicTree {
  struct Node {
    int next[26], len, link;
    ll cnt = 0;
    Node(int l = 0, int lk = 0) : len(l), link(lk) {
      fill(begin(next), end(next), 0);
    }
  };
  vector<Node> st;
  string s;
  int last = 1;

  PalindromicTree(const string& str) : s(str) {
    // Nodo 0: raiz de longitud impar (-1)
    st.pb(Node(-1, 0));
    st.pb(Node(0, 0));  // Nodo 1: raiz de longitud par (0)
    for (int i = 0; i < sz(s); ++i) add(i);
    for (int i = sz(st) - 1; i >= 2; --i)
      st[st[i].link].cnt += st[i].cnt;
  }
  int get_link(int v, int i) const {
    while (
      i - st[v].len - 1 < 0 || s[i - st[v].len - 1] != s[i])
      v = st[v].link;
    return v;
  }
  void add(int i) {
    int c = s[i] - 'a';
    int cur = get_link(last, i);
    if (!st[cur].next[c]) {
      int nw = sz(st);
      int lk = st[get_link(st[cur].link, i)].next[c];
      st.pb(Node(st[cur].len + 2, max(1, lk)));
      st[cur].next[c] = nw;
    }
    last = st[cur].next[c];
    st[last].cnt++;
  }
  // Cantidad de palindromos DISTINTOS en S: sz(st) - 2
};
