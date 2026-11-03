#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/rope>
using namespace __gnu_pbds;
using namespace __gnu_cxx;

// 1. Ordered Set (elementos unicos)
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag,
                         tree_order_statistics_node_update>;

// 2. Ordered Multiset seguro con pair<T, int> y less<>
// (NUNCA usar less_equal,
// porque viola asimetria y corrompe find, lower_bound y
// erase).
template <typename T>
using ordered_multiset =
    tree<pair<T, int>, null_type, less<pair<T, int>>,
         rb_tree_tag, tree_order_statistics_node_update>;

void pbds_and_rope_demo() {
    ordered_set<int> os;
    os.insert(10);
    os.insert(20);
    os.insert(50);

    // k-esimo elemento (0-indexed; precondicion: 0 <= k <
    // sz(os)):
    int elem_idx_1 = *os.find_by_order(1);  // 20
    // Cantidad de elementos estrictamente < 25:
    // 2 (el 10 y el 20)
    int menores_25 = os.order_of_key(25);

    // Multiset con duplicados, conteos por rango y borrado
    // de
    // UNA sola copia:
    ordered_multiset<int> oms;
    int uid = 0;
    oms.insert({5, uid++});
    oms.insert({5, uid++});
    oms.insert({10, uid++});
    // 0 (< 5)
    int menores_5 = oms.order_of_key({5, -1});
    // 2 (<= 5)
    int men_ig_5 = oms.order_of_key({5, INF_INT});
    auto it = oms.lower_bound({5, -1});
    // Borra 1 copia de 5
    if (it != oms.end() && it->first == 5) oms.erase(it);
    (void)elem_idx_1;
    (void)menores_25;
    (void)menores_5;
    (void)men_ig_5;

    // 3. Uso de rope<int> o crope (rope<char>)
    // Cortar el rango 0-based [l, r] y moverlo al inicio:
    rope<int> rp;
    // [1,2,3,4,5]
    for (int i = 1; i <= 5; ++i) rp.push_back(i);
    int l = 1, r = 3, len = r - l + 1;
    rope<int> sub = rp.substr(l, len);  // Extrae [2, 3, 4]
    rp.erase(l, len);                   // Queda [1, 5]
    // Queda [2, 3, 4, 1, 5]
    rp.insert(rp.mutable_begin(), sub);
}
