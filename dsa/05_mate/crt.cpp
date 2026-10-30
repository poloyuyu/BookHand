
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned ll
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN
#define i128 __int128

const ll mod = 1e9 + 7;

//TESTED (PD: for 2x2 system but should work for any nXn)

ll gcdExtended(ll a, ll b, ll &x, ll &y){
    if(b == 0){
        x = 1;
        y = 0;
        return a;
    }
    ll x_, y_, gcd = gcdExtended(b, a%b, x_, y_);
    x = y_;
    y = x_ - y_*(a/b);
    return gcd;
}

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