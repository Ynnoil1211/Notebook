// Push-Relabel (Highest-Label Preflow-Push O(V^2 sqrt(E)))
struct PushRelabel {
    struct Edge {
        int to, rev;
        ll cap, flow;
    };
    int n;
    vector<vector<Edge>> adj;
    vector<ll> excess;
    vi height, count;
    vector<bool> active;
    vector<vi> B;
    int b_max = 0;

    PushRelabel(int n)
        : n(n), adj(n + 1), excess(n + 1, 0),
          height(n + 1, 0), count(2 * n + 2, 0),
          active(n + 1, false), B(2 * n + 2) {}

    void add_edge(int from, int to, ll cap) {
        adj[from].push_back({to, sz(adj[to]), cap, 0});
        adj[to].push_back({from, sz(adj[from]) - 1, 0, 0});
    }
    void enqueue(int v) {
        if (!active[v] && excess[v] > 0 && height[v] < n) {
            active[v] = true;
            B[height[v]].push_back(v);
            b_max = max(b_max, height[v]);
        }
    }
    void push(int u, Edge& e) {
        ll push_amt = min(excess[u], e.cap - e.flow);
        if (push_amt == 0 || height[u] <= height[e.to])
            return;
        e.flow += push_amt;
        adj[e.to][e.rev].flow -= push_amt;
        excess[u] -= push_amt;
        excess[e.to] += push_amt;
        enqueue(e.to);
    }
    void gap(int k) {
        for (int v = 1; v <= n; ++v) {
            if (height[v] > k && height[v] < n) {
                count[height[v]]--;
                height[v] = n + 1;
                count[height[v]]++;
                enqueue(v);
            }
        }
    }
    void relabel(int v) {
        int old_h = height[v];
        count[old_h]--;
        int d = 2 * n;
        for (auto& e : adj[v])
            if (e.cap - e.flow > 0)
                d = min(d, height[e.to]);
        height[v] = d + 1;
        count[height[v]]++;
        enqueue(v);
        if (old_h < n && count[old_h] == 0) gap(old_h);
    }
    ll max_flow(int s, int t) {
        if (s == t) return 0;
        fill(all(count), 0);
        count[0] = n - 1;
        count[n] = 1;
        height[s] = n;
        active[s] = active[t] = true;
        for (auto& e : adj[s]) {
            excess[s] += e.cap;
            push(s, e);
        }
        while (b_max >= 0) {
            if (B[b_max].empty()) {
                b_max--;
                continue;
            }
            int v = B[b_max].back();
            B[b_max].pop_back();
            active[v] = false;
            for (auto& e : adj[v]) {
                if (excess[v] == 0) break;
                push(v, e);
            }
            if (excess[v] > 0) relabel(v);
        }
        return excess[t];
    }
};
