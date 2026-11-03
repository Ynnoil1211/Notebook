// 3. Automata KMP: aut[i][c] = nuevo estado si llevamos 'i'
// chars de 'p'
//    coincidentes y agregamos la letra ('a' + c).
vector<vi> build_kmp_automaton(const string& p) {
    int m = sz(p);
    vi pi = prefix_function(p);
    vector<vi> aut(m + 1, vi(26, 0));
    for (int i = 0; i <= m; ++i) {
        for (int c = 0; c < 26; ++c) {
            if (i == m || (i > 0 && 'a' + c != p[i]))
                aut[i][c] = aut[pi[i - 1]][c];
            else aut[i][c] = i + ('a' + c == p[i]);
        }
    }
    return aut;
}
