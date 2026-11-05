//TESTED (PD: for 2x2 system but should work for any nXn)
#include"gcdExtended.cpp"

ll mod_inv(ll a, ll m){
    ll x, y;
    ll g = gcdExtended(a, m, x, y);
    x = (x%m + m)%m;
    return x;
}

struct Congruence {
    ll b, n;
};

ll chinese_remainder_theorem(vector<Congruence> const& congruences) {
    ll N = 1;
    for (auto const& congruence : congruences) N *= congruence.n;

    ll x = 0;
    for (auto const& congruence : congruences) {
        ll b_i = congruence.b;
        ll N_i = N / congruence.n;
        ll invN_i = mod_inv(N_i, congruence.n);
        i128 aux = (b_i*N_i)%N;
        aux = (aux*invN_i)%N;
        x = (ll) (x + aux)%N;
    }
    return x;
}
