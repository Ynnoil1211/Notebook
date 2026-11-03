// 1. Convex Hull Trick Dinamico (LineContainer - inserta
// rectas y = k*x + m
// en cualquier orden y consulta el MAXIMO en x en O(log
// N)).
// NOTA: Para consultar el MINIMO, insertar (-k, -m) y
// devolver -query(x).
struct Line {
    mutable ll k, m, p;
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    static const ll inf = LLONG_MAX;
    // Division entera hacia abajo (floor)
    ll div(ll a, ll b) {
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }
    void add(ll k, ll m) {  // Agrega la recta y = k*x + m
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y))
            isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }
    ll query(
        ll x) const {  // Retorna max(k*x + m) en O(log N)
        assert(!empty());
        auto l = *lower_bound(x);
        return l.k * x + l.m;
    }
};
