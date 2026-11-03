// 2. Suffix Automaton (SAM) en O(N)
struct SuffixAutomaton {
    struct State {
        int len = 0, link = -1;
        int next[26];
        // Frecuencia de aparicion del estado (endpos size)
        ll cnt = 0;
        State() { fill(begin(next), end(next), -1); }
    };
    vector<State> st;
    int last = 0;

    SuffixAutomaton(const string& s) {
        st.reserve(2 * sz(s) + 1);
        st.emplace_back();
        for (char c : s) extend(c - 'a');
        // Propagar frecuencias (cnt) de estados largos a
        // sus
        // suffix links:
        vi ord(sz(st) - 1);
        iota(all(ord), 1);
        sort(all(ord), [&](int a, int b) {
            return st[a].len > st[b].len;
        });
        for (int u : ord) st[st[u].link].cnt += st[u].cnt;
    }
    void extend(int c) {
        int cur = sz(st);
        st.emplace_back();
        st[cur].len = st[last].len + 1;
        st[cur].cnt = 1;
        int p = last;
        while (p != -1 && st[p].next[c] == -1) {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz(st);
                st.pb(st[q]);
                st[clone].len = st[p].len + 1;
                // Los clones no suman nueva ocurrencia base
                st[clone].cnt = 0;
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
    // Total de subcadenas distintas de S en O(|st|):
    ll count_distinct_substrings() const {
        ll ans = 0;
        for (int i = 1; i < sz(st); ++i)
            ans += st[i].len - st[st[i].link].len;
        return ans;
    }
    // Subcadena comun mas larga (LCS) entre S y otro string
    // T
    // en O(|T|):
    string longest_common_substring(const string& t) const {
        int v = 0, l = 0, best_len = 0, best_pos = 0;
        for (int i = 0; i < sz(t); ++i) {
            int c = t[i] - 'a';
            while (v != 0 && st[v].next[c] == -1) {
                v = st[v].link;
                l = st[v].len;
            }
            if (st[v].next[c] != -1) {
                v = st[v].next[c];
                l++;
            }
            if (l > best_len) {
                best_len = l;
                best_pos = i;
            }
        }
        return t.substr(best_pos - best_len + 1, best_len);
    }
};
