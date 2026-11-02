// 1. Numero a string binario de longitud fija n (s[0] =
// LSB)
string to_binary_string(ll num, int n) {
  string s = "";
  for (int i = 0; i < n; i++) {
    s += (num % 2 == 0 ? '0' : '1');
    num /= 2;
  }
  return s; // s[0] es el bit 0 (menos significativo)
}

// 2. Conversor general entre bases [2, 36] (MSB primero)
int char_to_val(char c) {
  if (isdigit(c)) return c - '0';
  return toupper(c) - 'A' + 10;
}
char val_to_char(int v) {
  return (v < 10) ? ('0' + v) : ('A' + v - 10);
}

string decimal_to_base(ll num, int base) {
  if (num == 0) return "0";
  bool neg = (num < 0);
  if (neg) num = -num;
  string res = "";
  while (num > 0) {
    res += val_to_char(num % base);
    num /= base;
  }
  if (neg) res += '-';
  reverse(all(res));
  return res;
}

ll base_to_decimal(const string& s, int base) {
  if (s.empty()) return 0;
  ll res = 0;
  int st = (s[0] == '-' || s[0] == '+') ? 1 : 0;
  for (int i = st; i < sz(s); ++i)
    res = res * base + char_to_val(s[i]);
  return (s[0] == '-') ? -res : res;
}

// 3. Cambio a base negativa (ej. neg_base = -2)
string to_negative_base(ll n, int neg_base) {
  if (n == 0) return "0";
  string res = "";
  while (n != 0) {
    ll rem = n % neg_base;
    n /= neg_base;
    if (rem < 0) { rem += (-neg_base); n += 1; }
    res += (char)('0' + rem);
  }
  reverse(all(res));
  return res;
}

// 4. Division entera y modulo de numero gigante en string
// Ejemplo: divide_large_number("10010", 14) -> "715"
string divide_large_number(const string& s, ll n) {
  string res = "";
  ll rem = 0;
  for (char c : s) {
    rem = rem * 10 + (c - '0');
    if (!res.empty() || rem / n > 0)
      res += to_string(rem / n);
    rem %= n;
  }
  return res.empty() ? "0" : res;
}

ll mod_large_number(const string& s, ll m) {
  ll rem = 0;
  for (char c : s) rem = (rem * 10 + (c - '0')) % m;
  return rem;
}
