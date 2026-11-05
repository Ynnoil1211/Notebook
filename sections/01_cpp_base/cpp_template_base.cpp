#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define forn(i, n) for (int i = 0; i < (int)n; ++i)
#define for1(i, n) for (int i = 1; i <= (int)n; ++i)
#define pb push_back
#define fi first
#define se second
#define el '\n'

const int INF_INT = 1e9;
const ll INF = 1e18;
const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

// |a - b| <= EPS no es transitivo: nunca usar eps_lt como
// comparador de std::set/std::map. Para claves, cuantizar
// con quantize_eps.
int eps_sgn(ld x) { return (x > EPS) - (x < -EPS); }
bool eps_eq(ld a, ld b) { return eps_sgn(a - b) == 0; }
bool eps_lt(ld a, ld b) { return eps_sgn(a - b) < 0; }
ll quantize_eps(ld x, ld step = EPS) {
    return llround(x / step);
}

void solve() {
    // Logica de un caso de prueba.
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while (t--) solve();
    return 0;
}
