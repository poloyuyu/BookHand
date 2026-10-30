#include<bits/stdc++.h>
#include<iostream>

using namespace std;

#define ll long long
#define ull unsigned ll
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7;

//TESTED

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