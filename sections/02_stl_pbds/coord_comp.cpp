// Compresion de Coordenadas: mapea valores arbitrarios a
// [0, K-1]
struct CoordComp {
    vll vals;
    void init(const vll& a) {
        vals = a;
        sort(all(vals));
        vals.erase(unique(all(vals)), vals.end());
    }
    int get_id(ll x) const {  // Indice 0-based en O(log N)
        return lower_bound(all(vals), x) - vals.begin();
    }
    ll get_val(int id) const { return vals[id]; }
    int size() const { return sz(vals); }
};

void vector_and_bs_cheatsheet() {
    vi v = {1, 2, 2, 5, 5, 8, 9};  // DEBE ESTAR ORDENADO

    // 1. Primer elemento >= x (lower_bound)
    // Apunta al 1er 5 (idx 3)
    auto it_ge = lower_bound(all(v), 5);
    int idx_ge = it_ge - v.begin();

    // 2. Primer elemento > x (upper_bound)
    // Apunta al 8 (idx 5)
    auto it_gt = upper_bound(all(v), 5);
    int idx_gt = it_gt - v.begin();

    // 3. Ultimo elemento <= x (predecesor inclusivo)
    auto it_le = upper_bound(all(v), 5);
    if (it_le != v.begin()) {
        int val_le = *prev(it_le);  // 5 (idx 4)
    }

    // 4. Ultimo elemento < x (predecesor estricto)
    auto it_lt = lower_bound(all(v), 5);
    if (it_lt != v.begin()) {
        int val_lt = *prev(it_lt);  // 2 (idx 2)
    }

    // 5. Contar cuantos elementos estan en el rango [L, R]
    int L = 2, R = 8;
    int cnt_in_range =
        upper_bound(all(v), R) -
        lower_bound(all(v), L);  // 5 elementos

    // 6. Utilidades clave de <algorithm> y <numeric>
    ll suma = accumulate(all(v), 0LL);  // ¡Usar 0LL, no 0!
    int mx = *max_element(all(v));
    int mn = *min_element(all(v));

    // 7. Generar todas las permutaciones en O(N! * N)
    vi p = {3, 1, 2};
    sort(all(p));  // OBLIGATORIO ordenar antes del do-while
    do {
        // procesar permutacion 'p'
    } while (next_permutation(all(p)));
}
