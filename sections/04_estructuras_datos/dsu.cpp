// 1. DSU Clasico (Path Compression + Union by Size)
struct DSU {
    vi parent, sz;
    int num_comp;
    DSU(int n) : parent(n + 1), num_comp(n) {
        sz.assign(n + 1, 1);
        iota(all(parent), 0);
    }
    int find(int i) {
        return (parent[i] == i)
                   ? i
                   : (parent[i] = find(parent[i]));
    }
    bool unite(int i, int j) {
        int ri = find(i), rj = find(j);
        if (ri == rj) return false;
        if (sz[ri] < sz[rj]) swap(ri, rj);
        parent[rj] = ri;
        sz[ri] += sz[rj];
        num_comp--;
        return true;
    }
    bool same(int i, int j) { return find(i) == find(j); }
    int get_size(int i) { return sz[find(i)]; }
};
