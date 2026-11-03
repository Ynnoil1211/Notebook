// 2. Aho-Corasick (Multi-Pattern Matching + Automaton en
// O(sum|P| * 26))
struct AhoCorasick {
    struct Node {
        int next[26], link = 0, exit_link = 0, cnt = 0;
        Node() { fill(begin(next), end(next), 0); }
    };
    vector<Node> t = {Node()};

    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int ch = c - 'a';
            if (!t[u].next[ch]) {
                t[u].next[ch] = sz(t);
                t.emplace_back();
            }
            u = t[u].next[ch];
        }
        // Cantidad de patrones que terminan exactamente en
        // u
        t[u].cnt++;
    }
    void build() {
        queue<int> q;
        for (int c = 0; c < 26; ++c)
            if (t[0].next[c]) q.push(t[0].next[c]);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            int f = t[u].link;
            // Acumula ocurrencias de sufijos
            t[u].cnt += t[f].cnt;
            t[u].exit_link =
                (t[f].cnt > 0) ? f : t[f].exit_link;
            for (int c = 0; c < 26; ++c) {
                int v = t[u].next[c];
                if (v) {
                    t[v].link = t[f].next[c];
                    q.push(v);
                } else {
                    // Transicion directa de automata
                    t[u].next[c] = t[f].next[c];
                }
            }
        }
    }
    // Cuenta el total de apariciones de todos los patrones
    // dentro de 'text'
    ll count_matches(const string& text) const {
        int u = 0;
        ll total = 0;
        for (char c : text) {
            u = t[u].next[c - 'a'];
            total += t[u].cnt;
        }
        return total;
    }
};
