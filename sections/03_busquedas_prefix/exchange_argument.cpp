// 3. Argumento de intercambio: minimizar suma(w_i * C_i)
//    Ordena por menor razon t_i / w_i sin punto flotante.
struct JobEx {
    int id;
    ll t, w;
};
vector<JobEx> exchange_argument_order(vector<JobEx> jobs) {
    sort(all(jobs), [](const JobEx& a, const JobEx& b) {
        __int128 lhs = (__int128)a.t * b.w;
        __int128 rhs = (__int128)b.t * a.w;
        return lhs != rhs ? lhs < rhs : a.id < b.id;
    });
    return jobs;
}
