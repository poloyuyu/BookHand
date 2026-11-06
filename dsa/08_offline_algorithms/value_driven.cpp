struct event {
  ll l, r, x;  // For update value, for query threshold
  ll id, q;    // query type 0 update
};
vector<ll> ans;
void value_driven(ll l, ll r, vector<event>& v) {
  if (v.empty()) return;
  if (r - l <= 1) {
    // COUNTING -> return;
    for (auto& i : v) {
      if (i.q == 1) ans[i.id] = l;
    }
    return;
  }
  ll m = (l + r) >> 1;
  vector<event> vl, vr;
  for (auto& i : v) {
    if (i.q == 0) {
      if (i.x >= m) {
        // UPDATE
        // st.add(i.l, 1);
        vr.push_back(i);
      } else {
        vl.push_back(i);
      }
    } else {
      // ll cnt = query;
      // ll cnt = st.sum(i.l, i.r);
      if (cnt >= i.x) {
        vr.push_back(i);
      } else {
        // COUNTING -> ans[i.id] += cnt; push_back(i);
        event ni = i;
        ni.x -= cnt;
        vl.push_back(ni);
      }
    }
  }
  // ROLLBACK
  // for (auto &i: v) {
  //     if (i.q == 0 && i.x >= m) {
  //         st.add(i.l, -1);
  //     }
  // }
  value_driven(l, m, vl);
  value_driven(m, r, vr);
}
