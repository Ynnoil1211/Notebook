// 3. Eliminacion Gaussiana en punto flotante O(N^3)
// Matriz aumentada 'a' de tamano N x (M + 1).
// Retorna: 0 (sin solucion), 1 (solucion unica en 'ans'), 2
// (infinitas).
int gauss_double(vector<vector<double>> a,
                 vector<double>& ans) {
    int n = sz(a), m = sz(a[0]) - 1;
    vi where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row; i < n; ++i)
            if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
        if (abs(a[sel][col]) < 1e-9) continue;
        swap(a[sel], a[row]);
        where[col] = row;
        for (int i = 0; i < n; ++i)
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j = col; j <= m; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        row++;
    }
    ans.assign(m, 0);
    for (int i = 0; i < m; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < m; ++j) sum += ans[j] * a[i][j];
        if (abs(sum - a[i][m]) > 1e-9) return 0;
    }
    for (int i = 0; i < m; ++i)
        if (where[i] == -1) return 2;
    return 1;
}
