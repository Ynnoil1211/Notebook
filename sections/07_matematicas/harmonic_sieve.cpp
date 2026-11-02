// 2. Criba Armonica de Divisores en O(N log N)
int num_div[MAX_P];
ll sum_div[MAX_P];
void harmonic_sieve(int n) {
  for (int i = 1; i <= n; ++i)
    for (int j = i; j <= n; j += i) {
      num_div[j]++;
      sum_div[j] += i;
    }
}
