// 1. Separar string por un caracter delimitador (ej. ',',
// ':')
vector<string> split_delim(const string& s, char delim) {
  vector<string> tokens;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) tokens.push_back(item);
  return tokens;
}

// 2. Separar string por espacios (ignora espacios
// multiples)
vector<string> split_spaces(const string& s) {
  vector<string> tokens;
  stringstream ss(s);
  string w;
  while (ss >> w) tokens.push_back(w);
  return tokens;
}

void string_methods_demo() {
  string s = "icpc_regional_icpc_2026";

  // --- BUSQUEDA (find / rfind) ---
  // Retorna indice 0-based o string::npos si NO existe
  size_t p1 = s.find("icpc");       // 0 (primera aparicion)
  // 14 (busca desde idx 1)
  size_t p2 = s.find("icpc", 1);
  size_t p3 = s.rfind("icpc");      // 14 (ultima aparicion)
  if (s.find("xyz") == string::npos) { /* No esta */ }

  // Hallar TODAS las ocurrencias de un patron 'pat':
  string pat = "icpc";
  for (size_t pos = s.find(pat); pos != string::npos;
       pos = s.find(pat, pos + 1)) {
    // 'pos' es el inicio 0-based de cada coincidencia
  }

  // --- SUBCADENAS (substr) ---
  // ¡CUIDADO! substr(pos_inicio, LONGITUD)
  // Para extraer el rango [L, R] inclusivo: s.substr(L,
  // R-L+1)
  string sub1 = s.substr(5, 8);     // "regional" (8 chars)
  // "2026" (hasta el final)
  string sub2 = s.substr(19);

  // --- EDICION (erase / insert / replace) ---
  s.erase(4, 9);          // Borra 9 chars desde idx 4
  s.insert(4, "_world");  // Inserta en idx 4
  // Reemplaza 4 chars desde 0 por "acm"
  s.replace(0, 4, "acm");

  // --- CONVERSIONES ---
  // string -> long long
  ll val = stoll("1234567890123");
  ll from_bin = stoll("101101", 0, 2); // base 2 -> ll (45)
  // base 16 -> ll (255)
  ll from_hex = stoll("FF", 0, 16);
  double d = stod("3.14159");          // string -> double
  string num_s = to_string(val);       // numero -> string

  // --- MAYUSCULAS / MINUSCULAS ---
  transform(all(s), s.begin(), ::tolower);
  transform(all(s), s.begin(), ::toupper);
}
