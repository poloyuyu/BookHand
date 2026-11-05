// TESTED
#include "bin_pow.cpp"
#include "tot_euler.cpp"
ll generator_(ll n) {
  vector<int> facts;
  ll phi_n = tot_euler(n), m = phi_n;
  for (ll i = 2; i * i <= m; ++i) {
    if (m % i == 0) {
      facts.push_back(i);
      while (m % i == 0) m /= i;
    }
  }
  if (m > 1) facts.push_back(m);
  for (ll g = 2; g <= n; ++g) {
    bool ok = true;
    for (size_t i = 0; i < facts.size() && ok; ++i)
      ok &= binpow(g, phi_n / facts[i], n) != 1;
    if (ok) return g;
  }
  return -1;
}