void set_multiset_map_cheatsheet() {
  set<int> s = {10, 20, 30, 40};
  s.insert(25);
  s.erase(20);                  // Borra el 20 en O(log N)
  // Minimo actual (10) en O(1)
  int mn = *s.begin();
  // Maximo actual (40) en O(1)
  int mx = *s.rbegin();

  // --- BUSQUEDA BINARIA EN SET / MULTISET ---
  // ¡PROHIBIDO usar std::lower_bound(all(s), x) porque es
  // O(N)!
  // Usar SIEMPRE el metodo miembro s.lower_bound(x) ->
  // O(log N):

  int x = 25;
  // 1) Primer elemento >= x:
  auto it_ge = s.lower_bound(x);
  if (it_ge != s.end()) { int val_ge = *it_ge; }

  // 2) Primer elemento > x:
  auto it_gt = s.upper_bound(x);
  if (it_gt != s.end()) { int val_gt = *it_gt; }

  // 3) Mayor elemento < x (predecesor estricto):
  auto it_lt = s.lower_bound(x);
  if (it_lt != s.begin()) { int val_lt = *prev(it_lt); }

  // 4) Mayor elemento <= x (predecesor inclusivo):
  auto it_le = s.upper_bound(x);
  if (it_le != s.begin()) { int val_le = *prev(it_le); }

  // --- TRAMPAS CLASICAS DE MULTISET ---
  multiset<int> ms = {5, 5, 5, 10};
  // Trampa 1: ms.erase(5) borra TODAS las apariciones del
  // 5.
  // Para borrar UNA SOLA copia del 5 en O(log N):
  auto it = ms.find(5);
  if (it != ms.end()) ms.erase(it); // Queda {5, 5, 10}

  // Trampa 2: ms.count(x) es O(k + log N) donde k =
  // repeticiones.
  // Para saber solo si existe en O(log N), usar:
  bool existe = (ms.find(5) != ms.end());

  // --- CUIDADO CON STD::MAP ---
  map<int, int> mp;
  // Hacer if (mp[key] == 0) INSERTA 'key' en el mapa y
  // gasta RAM/tiempo.
  // Para consultar sin insertar:
  if (mp.find(100) != mp.end()) { int val = mp[100]; }
}
