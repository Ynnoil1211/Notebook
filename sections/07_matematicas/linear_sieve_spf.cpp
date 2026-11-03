// 3. Criba Lineal O(N) con Menor Factor Primo (SPF) y
// Factorizacion O(log X)
const int MAX_SPF = 10000005;
int spf[MAX_SPF];
vi primes_list;
void linear_sieve_spf(int n = MAX_SPF - 1) {
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes_list.pb(i);
        }
        for (int p : primes_list) {
            if (p > spf[i] || 1LL * i * p > n) break;
            spf[i * p] = p;
        }
    }
}
// Retorna factores primos con multiplicidad
vi get_prime_factors(int x) {
    vi f;
    while (x > 1) {
        f.pb(spf[x]);
        x /= spf[x];
    }
    return f;
}
