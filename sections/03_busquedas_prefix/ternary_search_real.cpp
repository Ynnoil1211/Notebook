// 1. Ternary Search Continua (halla el MINIMO en [l, r])
// Para hallar el MAXIMO: cambiar f(m1) < f(m2) por f(m1) >
// f(m2)
template <typename F>
double ternary_continuous(double l, double r, F f,
                          int it = 100) {
    for (int i = 0; i < it; ++i) {
        double m1 = l + (r - l) / 3.0;
        double m2 = r - (r - l) / 3.0;
        if (f(m1) < f(m2)) r = m2;
        else l = m1;
    }
    // Retorna el valor minimo (o 'l' para la posicion)
    return f(l);
}
