struct ImplicitTreap {
    struct Node {
        ll val, sum, lazy_add;
        int pr, cnt;
        bool rev;
        Node *l = nullptr, *r = nullptr;
        Node(ll v)
            : val(v), sum(v), lazy_add(0), pr((int)rng()),
              cnt(1), rev(false) {}
    };
    using pnode = Node*;
    pnode root = nullptr;

    int cnt(pnode t) const { return t ? t->cnt : 0; }
    ll sum(pnode t) const { return t ? t->sum : 0; }

    void apply_add(pnode t, ll add_v) {
        if (!t) return;
        t->val += add_v;
        t->sum += add_v * t->cnt;
        t->lazy_add += add_v;
    }
    void apply_rev(pnode t) {
        if (!t) return;
        t->rev ^= 1;
        swap(t->l, t->r);
    }
    void push(pnode t) {
        if (!t) return;
        if (t->rev) {
            apply_rev(t->l);
            apply_rev(t->r);
            t->rev = false;
        }
        if (t->lazy_add != 0) {
            apply_add(t->l, t->lazy_add);
            apply_add(t->r, t->lazy_add);
            t->lazy_add = 0;
        }
    }
    void pull(pnode t) {
        if (!t) return;
        t->cnt = 1 + cnt(t->l) + cnt(t->r);
        t->sum = t->val + sum(t->l) + sum(t->r);
    }
    // Divide 't' en 'l' (primeros k elementos) y 'r' (el
    // resto)
    void split(pnode t, pnode& l, pnode& r, int k) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        push(t);
        if (cnt(t->l) >= k) {
            split(t->l, l, t->l, k);
            r = t;
        } else {
            split(t->r, t->r, r, k - cnt(t->l) - 1);
            l = t;
        }
        pull(t);
    }
    void merge(pnode& t, pnode l, pnode r) {
        push(l);
        push(r);
        if (!l || !r) t = l ? l : r;
        else if (l->pr > r->pr) {
            merge(l->r, l->r, r);
            t = l;
        } else {
            merge(r->l, l, r->l);
            t = r;
        }
        pull(t);
    }
    void push_back(ll val) {
        merge(root, root, new Node(val));
    }

    // Invierte el subarreglo 0-indexed [ql, qr]
    void reverse_range(int ql, int qr) {
        pnode t1, t2, t3;
        split(root, t1, t2, ql);
        split(t2, t2, t3, qr - ql + 1);
        apply_rev(t2);
        merge(root, t1, t2);
        merge(root, root, t3);
    }
    // Suma 'val' al subarreglo 0-indexed [ql, qr]
    void add_range(int ql, int qr, ll val) {
        pnode t1, t2, t3;
        split(root, t1, t2, ql);
        split(t2, t2, t3, qr - ql + 1);
        apply_add(t2, val);
        merge(root, t1, t2);
        merge(root, root, t3);
    }
    // Consulta suma en 0-indexed [ql, qr]
    ll query_sum(int ql, int qr) {
        pnode t1, t2, t3;
        split(root, t1, t2, ql);
        split(t2, t2, t3, qr - ql + 1);
        ll res = sum(t2);
        merge(root, t1, t2);
        merge(root, root, t3);
        return res;
    }
};
