// 1. Two Pointers: Subarreglo mas largo con suma <= max_sum
// (a[i] >= 0)
int max_subarray_sum_bound(const vll& a, ll max_sum) {
    int n = sz(a), max_len = 0, l = 0;
    ll cur_sum = 0;
    for (int r = 0; r < n; ++r) {
        cur_sum += a[r];
        while (cur_sum > max_sum && l <= r) {
            cur_sum -= a[l++];
        }
        max_len = max(max_len, r - l + 1);
    }
    return max_len;
}
