vector<fenwick_tree> bit(k + 1, fenwick_tree(m));

ll answer = 0;
for (ll x : v) {
  ll value = compress(x);  // or already compressed
  vector<ll> current(k + 1);
  current[1] = 1;
  for (ll len = 2; len <= k; len++) {
    current[len] = bit[len - 1].sum(value - 1);
  }
  for (ll len = 1; len <= k; len++) {
    bit[len].add(value, current[len]);
  }
  answer += current[k];
}
cout << answer << endl;