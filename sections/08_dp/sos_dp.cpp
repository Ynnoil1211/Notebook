// 5. SOS DP (Sum Over Subsets) en O(N * 2^N)
vll sos_dp(int N, const vll& A) {
    vll F = A;
    for (int i = 0; i < N; ++i)
        for (int mask = 0; mask < (1 << N); ++mask)
            if (mask & (1 << i))
                F[mask] += F[mask ^ (1 << i)];
    return F;
}
