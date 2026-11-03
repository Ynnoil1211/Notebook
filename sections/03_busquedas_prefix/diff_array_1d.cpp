// 1. Difference Array 1D (1-indexed [1..n])
struct DiffArray1D {
    int n;
    vll diff;
    DiffArray1D(int n) : n(n), diff(n + 2, 0) {}
    void add_range(int l, int r,
                   ll val) {  // 1-based [l, r]
        diff[l] += val;
        diff[r + 1] -= val;
    }
    vll build() const {
        vll res(n + 1, 0);
        for (int i = 1; i <= n; ++i)
            res[i] = res[i - 1] + diff[i];
        return res;  // res[1..n]
    }
};

// 2. Doble Difference Array (Operaciones de rango sobre
// operaciones)
// Se tienen M operaciones (sumar d_i en [l_i, r_i]) y K
// consultas
// que aplican todas las operaciones desde la x_j hasta la
// y_j (CF 295A).
struct Op {
    int l, r;
    ll d;
};
vll apply_double_diff(int n, int m, const vll& base_arr,
                      const vector<Op>& ops,
                      const vector<pii>& queries) {
    DiffArray1D op_diff(m);
    for (auto [x, y] : queries) op_diff.add_range(x, y, 1);
    vll op_cnt = op_diff.build();

    DiffArray1D arr_diff(n);
    for (int i = 1; i <= m; ++i)
        if (op_cnt[i] > 0)
            arr_diff.add_range(ops[i].l, ops[i].r,
                               ops[i].d * op_cnt[i]);

    vll delta = arr_diff.build(), ans(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        ans[i] = base_arr[i] + delta[i];
    return ans;
}
