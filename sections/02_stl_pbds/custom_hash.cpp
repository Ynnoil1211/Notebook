struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now()
                .time_since_epoch()
                .count();
        return splitmix64(x + FIXED_RANDOM);
    }
    // Permite usar pair<T1, T2> como llave en safe_umap
    template <typename T1, typename T2>
    size_t operator()(const pair<T1, T2>& p) const {
        uint64_t h1 = (*this)(p.first);
        uint64_t h2 = (*this)(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15 + (h1 << 6) +
                     (h1 >> 2));
    }
};

template <typename K, typename V>
using safe_umap = unordered_map<K, V, custom_hash>;

template <typename K>
using safe_uset = unordered_set<K, custom_hash>;

// Aplicacion: Contar pares (i < j) con a[i] == a[j] en O(N)
// Ejemplo: a = [1, 2, 1, 1, 2] -> 3 pares de 1 + 1 par de 2
// = 4
ll count_equal_pairs(const vi& a) {
    safe_umap<int, int> freq;
    freq.reserve(sz(a) * 2);
    freq.max_load_factor(0.25f);
    for (int x : a) freq[x]++;
    ll pairs = 0;
    for (auto& [val, cnt] : freq)
        pairs += 1LL * cnt * (cnt - 1) / 2;
    return pairs;
}
