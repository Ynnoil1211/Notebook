// 2. Precalculo de phi[1..n] en O(N log log N) y mu[1..n]
// en O(N log N)
int phi_arr[MAX_P], mu_arr[MAX_P];
void init_phi_and_mobius(int n) {
  for (int i = 1; i <= n; ++i) { phi_arr[i] = i;
    mu_arr[i] = 0; }
  for (int i = 2; i <= n; ++i) {
    if (phi_arr[i] == i) { // i es primo
      for (int j = i; j
        <= n; j += i) phi_arr[j] -= phi_arr[j] / i;
    }
  }
  mu_arr[1] = 1;
  for (int i = 1; i <= n; ++i)
    for (int j = 2 * i; j <= n; j += i)
      mu_arr[j] -= mu_arr[i];
}
