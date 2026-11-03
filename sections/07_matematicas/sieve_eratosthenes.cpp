// 1. Criba de Eratostenes en O(MAX_P log log MAX_P)
const int MAX_P = 1000005;
bool is_prime[MAX_P];
void sieve() {
    is_prime[0] = is_prime[1] = false;
    fill(is_prime + 2, is_prime + MAX_P, true);
    for (ll i = 2; i * i < MAX_P; ++i)
        if (is_prime[i])
            for (ll j = i * i; j < MAX_P; j += i)
                is_prime[j] = false;
}
