// 2. Monotonic Stack: Indice del primer elemento > a[i] a
// la derecha
// Retorna -1 si no existe. (Para < a[i], cambiar '<' por
// '>')
vi next_greater_right(const vll& a) {
  int n = sz(a);
  vi nxt(n, -1);
  stack<int> st;
  for (int i = 0; i < n; ++i) {
    while (!st.empty() && a[st.top()] < a[i]) {
      nxt[st.top()] = i;
      st.pop();
    }
    st.push(i);
  }
  return nxt;
}
