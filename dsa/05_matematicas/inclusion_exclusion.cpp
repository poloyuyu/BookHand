// Example cardinality of all subsets of size 1
ll cardinalitySubset(ll subset) { return 1; }
// Template
ll InclusionExclusion() {
  // sets
  ll n = 10;              // example
  ll subsets = (1 << n);  // total subsets
  ll ans = 0;
  // travel on all the subsets, with a particular subset
  for (ll subset = 0; subset < subsets; subset++) {
    // if is odd the subset we sum, otherwise substract, recall, Union of sets =
    // sum sets - intersect sets pairs + intersect group of three - ...
    ans += (__builtin_popcount(subset) & 1 ? cardinalitySubset(subset)
                                           : -cardinalitySubset(subset));
  }
  return ans;
}