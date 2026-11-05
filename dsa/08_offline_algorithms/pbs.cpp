vector<ll>l(q, 0), r(q, m+1);
while(1) {
    vector<vector<ll>> bucket(m+1);
    bool flag = 0;
    for (ll i = 0; i < q; i++) {
        if (r[i] - l[i] <= 1) continue;
        flag = 1;
        bucket[(l[i]+r[i])>>1] = i;
    }
    if (!flag) break;
    //RESET DATA STRUCTURE

    //SWEEP THROUGH POSIBLE ANSWERS POSITIONS
    for (ll i = 1; i<=m; i++) {
        update(i);

        for (auto j: bucket[i]) {
            if (foo(j)) r[j] = i;
            else l[j] = i;
        }
    }
}
