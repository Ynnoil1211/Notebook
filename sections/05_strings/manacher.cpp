// 2. Manacher (Unificado con '#' para pares e impares)
// Transforma "abba" -> "#a#b#b#a#".
// En el vector retornado 'p', p[i] - 1 es la longitud real
// del mayor
// palindromo centrado en t[i].
// Ademas, devuelve si s[l..r] (0-indexed) es palindromo en
// O(1)!
struct Manacher {
    vi p;
    Manacher(const string& s) {
        string t = "#";
        for (char c : s) {
            t += c;
            t += "#";
        }
        int n = sz(t), l = 0, r = -1;
        p.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            int k =
                (i > r) ? 1 : min(p[l + r - i], r - i + 1);
            while (0 <= i - k && i + k < n &&
                   t[i - k] == t[i + k])
                k++;
            p[i] = k--;
            if (i + k > r) {
                l = i - k;
                r = i + k;
            }
        }
    }
    // Consulta en O(1) si s[l..r] (0-indexed) es
    // palindromo:
    bool is_pal(int l, int r) const {
        int center = l + r + 1;
        int len = r - l + 1;
        return (p[center] - 1) >= len;
    }
    int max_pal_len() const {
        return *max_element(all(p)) - 1;
    }
};
