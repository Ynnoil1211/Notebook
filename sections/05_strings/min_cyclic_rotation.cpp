// Retorna el indice 0-based donde empieza la menor rotacion
// ciclica
// Ejemplo: s = "bbaaca" -> retorna 2 (rotacion "aacabb")
int min_cyclic_rotation(string s) {
  int n = sz(s);
  s += s;
  int i = 0, j = 1, k = 0;
  while (i < n && j < n && k < n) {
    if (s[i + k] == s[j + k]) {
      k++;
    } else {
      if (s[i + k] > s[j + k]) i += k + 1;
      else j += k + 1;
      if (i == j) j++;
      k = 0;
    }
  }
  return min(i, j);
}
