// Splay Tree Node. 'pp' apunta al padre del arbol
// representado
struct LCTNode {
    LCTNode *p = 0, *pp = 0, *c[2] = {0, 0};
    bool flip = 0;
    void fix() {
        for (int i = 0; i < 2; ++i)
            if (c[i]) c[i]->p = this;
    }
    int up() { return p ? (p->c[1] == this) : -1; }
    void push() {
        if (!flip) return;
        flip = 0;
        swap(c[0], c[1]);
        for (int i = 0; i < 2; ++i)
            if (c[i]) c[i]->flip ^= 1;
    }
    void rot(int i, int b) {
        int h = i ^ b;
        LCTNode *x = c[i], *y = (b == 2 ? x : x->c[h]),
                *z = (b ? y : x);
        if ((y->p = p)) p->c[up()] = y;
        c[i] = z->c[i ^ 1];
        if (b < 2)
            x->c[h] = y->c[h ^ 1],
            z->c[h ^ 1] = (b ? x : this);
        y->c[i ^ 1] = (b ? this : x);
        fix();
        x->fix();
        y->fix();
        if (p) p->fix();
        swap(pp, y->pp);
    }
    void splay() {
        for (push(); p;) {
            if (p->p) p->p->push();
            p->push();
            push();
            int c1 = up(), c2 = p->up();
            if (c2 == -1) p->rot(c1, 2);
            else p->p->rot(c2, c1 != c2);
        }
    }
    LCTNode* first() {
        push();
        return c[0] ? c[0]->first() : (splay(), this);
    }
};

struct LinkCut {
    vector<LCTNode> node;
    LinkCut(int n) : node(n + 1) {}

    LCTNode* access(LCTNode* u) {
        u->splay();
        while (LCTNode* pp = u->pp) {
            pp->splay();
            u->pp = 0;
            if (pp->c[1])
                pp->c[1]->p = 0, pp->c[1]->pp = pp;
            pp->c[1] = u;
            pp->fix();
            u = pp;
        }
        return u;
    }
    void make_root(int u) {
        access(&node[u]);
        node[u].splay();
        if (node[u].c[0]) {
            node[u].c[0]->p = 0;
            node[u].c[0]->flip ^= 1;
            node[u].c[0]->pp = &node[u];
            node[u].c[0] = 0;
            node[u].fix();
        }
    }
    bool connected(int u, int v) {
        return access(&node[u])->first() ==
               access(&node[v])->first();
    }
    void link(int u, int v) {
        if (connected(u, v)) return;
        make_root(u);
        node[u].pp = &node[v];
    }
    void cut(int u, int v) {
        make_root(v);
        node[u].splay();
        if (node[u].pp == &node[v]) node[u].pp = 0;
        else {
            node[u].c[0] = node[v].p = 0;
            node[u].fix();
        }
    }
};
