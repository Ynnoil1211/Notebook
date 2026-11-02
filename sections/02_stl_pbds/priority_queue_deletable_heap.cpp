// 1. Comparador en struct para Min-Heap (invertir el signo
// '<')
struct State {
  int u;
  ll cost;
  bool operator<(const State& o) const {
    return cost > o.cost; // El menor 'cost' queda en top()
  }
};

// 2. Deletable Min-Heap (Borrado arbitrario en O(log N))
template <typename T>
struct DeletableMinHeap {
  priority_queue<T, vector<T>, greater<T>> pq, del;

  void push(T x) { pq.push(x); }
  void erase(T x) { del.push(x); } // Borrado perezoso

  void clean() {
    while (!pq.empty() && !del.empty() && pq.top(
      ) == del.top()) {
      pq.pop();
      del.pop();
    }
  }
  T top() { clean(); return pq.top(); }
  void pop() { clean(); pq.pop(); }
  bool empty() { clean(); return pq.empty(); }
  int size() const { return sz(pq) - sz(del); }
};

void priority_queue_demo() {
  // Max-Heap (por defecto: saca el MAYOR primero)
  priority_queue<ll> max_pq;
  max_pq.push(10); max_pq.push(50);
  ll mayor = max_pq.top(); // 50
  max_pq.pop();

  // Min-Heap (saca el MENOR primero)
  priority_queue<ll, vector<ll>, greater<ll>> min_pq;
  min_pq.push(10); min_pq.push(50);
  ll menor = min_pq.top(); // 10
  min_pq.pop();
}
