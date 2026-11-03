// 3. Monotonic Deque: Minimo de cada ventana de longitud K
// Retorna vector con N - K + 1 respuestas. (Para Maximo:
// cambiar >= por <=)
vll sliding_window_min(const vll& a, int k) {
    int n = sz(a);
    // Guarda indices en orden creciente de valor
    deque<int> dq;
    vll ans;
    for (int i = 0; i < n; ++i) {
        if (!dq.empty() && dq.front() <= i - k)
            dq.pop_front();
        while (!dq.empty() && a[dq.back()] >= a[i])
            dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) ans.push_back(a[dq.front()]);
    }
    return ans;
}
