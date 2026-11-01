#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned ll
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7;

//Tested

ll discrete_log(ll a, ll b, ll m) {
    a %= m, b %= m;
    if (a == 0) return b == 0 ? 1 : -1;
    ll k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k)
            return add;
        if (b % g)
            return -1;
        b /= g, m /= g, ++add;
        k = (k * 1ll * a / g) % m;
    }

    ll n = sqrt(m) + 1;
    ll an = 1;
    for_(i, n) an = (an * 1ll * a) % m;

    unordered_map<ll, ll> f2inv;
    for (ll q = 0, f2 = b; q <= n; ++q) {
        f2inv[f2] = q;
        f2 = (f2 * 1ll * a) % m;
    }

    for (ll p = 1, f1 = k; p <= n; ++p) {
        f1 = (f1 * 1ll * an) % m;
        if (f2inv.count(f1)){
            ll x = n * p - f2inv[f1] + add;
            return x;
        }
    }
    return -1;
}