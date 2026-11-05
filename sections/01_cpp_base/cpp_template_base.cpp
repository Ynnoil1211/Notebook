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

// Grillas 2D: 0..3 son las 4 rectas (U, D, L, R) y 4..7 las
// diagonales (UL, UR, DL, DR). dr filas, dc columnas.
int dr[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
int dc[8] = {0, 0, -1, 1, -1, 1, -1, 1};
// Letra de cada paso, util para reconstruir caminos.
const char* dir_char[8] = {"U", "D", "L", "R",
                           "UL", "UR", "DL", "DR"};

// Flood fill / BFS de grilla: (r, c) dentro de n x m.
bool is_valid(int r, int c, int n, int m) {
    return 0 <= r && r < n && 0 <= c && c < m;
}

// |a - b| <= EPS no es transitivo: nunca usar eps_lt como
// comparador de std::set/std::map. Para claves, cuantizar
// con quantize_eps.
int eps_sgn(ld x) { return (x > EPS) - (x < -EPS); }
bool eps_eq(ld a, ld b) { return eps_sgn(a - b) == 0; }
bool eps_lt(ld a, ld b) { return eps_sgn(a - b) < 0; }
ll quantize_eps(ld x, ld step = EPS) {
    return llround(x / step);
}

// DEBUG: imprime un pii como "(x, y)".
ostream& operator<<(ostream& os, const pii& pa) {
    return os << "(" << pa.fi << ", " << pa.se << ")";
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
