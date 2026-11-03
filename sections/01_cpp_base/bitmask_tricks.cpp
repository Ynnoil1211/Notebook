void bitmask_tricks(ll x) {
    // 1. Funciones de hardware GCC (sufijo 'll' para 64
    // bits)
    // Cantidad de bits en 1
    int ones = __builtin_popcountll(x);
    // Ceros a la derecha (x>0)
    int ctz = __builtin_ctzll(x);
    // Ceros a la izq (x>0)
    int clz = __builtin_clzll(x);
    // floor(log2(x)) exacto
    int msb = 63 - __builtin_clzll(x);

    // 2. Operaciones sobre el k-esimo bit (0-indexed)
    int k = 4;
    x |= (1LL << k);           // Prender bit k
    x &= ~(1LL << k);          // Apagar bit k
    x ^= (1LL << k);           // Invertir (flip) bit k
    bool on = (x >> k) & 1LL;  // Consultar si bit k es 1

    // 3. Trucos del bit menos significativo (LSO / LSB)
    ll lso = x & -x;             // Aisla el 1 mas bajo
    ll clear_lso = x & (x - 1);  // Apaga el 1 mas bajo
    ll set_low0 = x | (x + 1);   // Prende el 0 mas bajo
    bool is_pow2 = (x > 0) && ((x & (x - 1)) == 0);

    // 4. Iterar sobre todas las submascaras no vacias de
    // 'm'
    int m = 0b10110;
    // Orden decreciente:
    for (int sub = m; sub > 0; sub = (sub - 1) & m) {
        // procesar 'sub'
    }
    // Orden creciente:
    for (int sub = 0; (sub = (sub - m) & m);) {
        // procesar 'sub'
    }
}

void bitset_methods_demo() {
    bitset<1005> bs;
    bs.set();            // Pone todos los bits en 1
    bs.reset();          // Pone todos los bits en 0
    bs.set(5, 1);        // Prende el bit 5 (bs[5] = 1)
    bs.reset(5);         // Apaga el bit 5 (bs[5] = 0)
    bs.flip(2);          // Invierte el bit 2
    int c = bs.count();  // Numero de bits en 1: O(N/64)
    bool a = bs.any();   // true si al menos un bit es 1
    bool n = bs.none();  // true si todos son 0

    // Recorrer SOLO los indices con bit en 1 en O(#unos +
    // N/64):
    for (int i = bs._Find_first(); i < (int)bs.size();
         i = bs._Find_next(i)) {
        // 'i' es la posicion del bit encendido
    }
}
