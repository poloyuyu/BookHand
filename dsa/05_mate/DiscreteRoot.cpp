#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned ll
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7;

//TESTED

ll gcd(ll a, ll b) {
    if(b==0) return a;
    return gcd(b, a%b);
}

ll binpow(ll a, ll b, ll m){
    ll ans = 1;
    while(b > 0){
        if(b&1) ans = (ans*a)%m;
        a=(a*a)%m;
        b>>=1;
    }
    return ans;
}

vector<ll> tot_euler(ll n){
    vector<ll> tot(n+1, 0);
    for_(i, n+1) tot[i]=i;
    for(ll i = 2; i <= n; ++i){
        if(tot[i] == i){
            for(ll j = i; j <= n; j+=i) tot[j] -= tot[j]/i;
        }
    }
    return tot;
}

ll generator_(ll n) {
    vector<int> facts;
    ll phi_n = tot_euler(n)[n], m = phi_n;
    for(ll i=2; i*i<=m; ++i){
        if(m % i == 0) {
            facts.push_back(i);
            while (m % i == 0) m/= i;
        }
    }
    if(m > 1) facts.push_back(m);
    for (ll g=2; g<=n; ++g) {
        bool ok = true;
        for (size_t i=0; i<facts.size() && ok; ++i) ok &= binpow(g, phi_n/facts[i], n) != 1;
        if (ok)  return g;
    }
    return -1;
}

vector<ll> DiscreteRoot(ll n,ll k,ll a){
    if(a==0) return {0};
    ll g = generator_(n);

    ll sq = (ll)sqrt(n) + 1;
    vector<pair<ll, ll>> dec(sq);
    for(ll i = 1; i <= sq; ++i)
        dec[i-1] = {binpow(g, i * sq * k % (n - 1), n), i};
    sort(dec.begin(), dec.end());
    ll h0 = -1;
    for_(i, sq) {
        ll my = binpow(g, i * k % (n - 1), n) * a % n;
        auto it = lower_bound(dec.begin(), dec.end(), make_pair(my, 0));
        if (it != dec.end() && it->first == my) {
            h0 = it->second * sq - i;
            break;
        }
    }
    if(h0 == -1) return {-1};

    ll delta = (n-1)/gcd(k, n-1);
    vector<ll> x;
    for (ll h = h0 % delta; h < n-1; h += delta)
        x.push_back(binpow(g, h, n));
    sort(x.begin(), x.end());
    return x;
}