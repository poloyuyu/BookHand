double golden_search(double l, double r) {
  double phi = (1 + sqrt(5)) / 2;
  double eps = 1e-9;
  while (r - l > eps) {
    double m1 = r - (r - l) / phi;
    double m2 = l + (r - l) / phi;
    if (f(m1) < f(m2)) l = m1;
    else r = m2;
  }
  return l;
}
