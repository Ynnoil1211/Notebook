// 2. Regret Greedy con Max-Heap (Moore-Hodgson):
//    tasks[i] = {duracion t_i, deadline d_i}.
int schedule_deadlines_heap(vector<pll> tasks) {
    sort(all(tasks), [](const pll& a, const pll& b) {
        return a.se < b.se;
    });
    priority_queue<ll> pq;
    ll cur_time = 0;
    for (auto [t, d] : tasks) {
        cur_time += t;
        pq.push(t);
        if (cur_time > d) {
            cur_time -= pq.top();
            pq.pop();
        }
    }
    return sz(pq);
}
