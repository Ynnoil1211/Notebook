// 3. Tecnica de Contribuciones: Suma de min(a[l..r]) sobre
// todos los
// subarreglos en O(N). Usa '>' a la izq y '>=' a la der
// para empates.
ll sum_all_subarray_mins(const vll& a) {
  int n = sz(a);
  vi L(n, -1), R(n, n);
  stack<int> st;
  for (int i = 0; i < n; ++i) {
    while (!st.empty() && a[st.top()] > a[i]) st.pop();
    L[i] = st.empty() ? -1 : st.top();
    st.push(i);
  }
  while (!st.empty()) st.pop();
  for (int i = n - 1; i >= 0; --i) {
    while (!st.empty() && a[st.top()] >= a[i]) st.pop();
    R[i] = st.empty() ? n : st.top();
    st.push(i);
  }
  ll total = 0;
  for (int i = 0; i < n; ++i) {
    ll ways = 1LL * (i - L[i]) * (R[i] - i);
    total += a[i] * ways;
  }
  return total;
}
