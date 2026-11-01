#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define for_(i, n) for(ll i = 0; i < n; ++i)

//TESTED for gcd

//only if it is neccesary, but Z transform work on any module, and without
ll mod = 998244353;

void Z(vector<ll> &C,ll N, bool g) {
    if(g){
        for(ll d = 1; d <= N; d++) {
            for(ll m = 2*d; m <= N; m += d) C[d] = (C[d]+C[m])%mod;
        }
    }else{
        for(ll d = N; d > 0; d--) {
            for(ll m = 2*d; m <= N; m += d) C[m] = (C[d]+C[m])%mod;
        }
    }
}

void Zinv(vector<ll> &C,ll N, bool g) {
    if(g){
        for(ll d = N; d > 0; d--) {
            for(ll m = 2*d; m <= N; m += d) C[d] = (mod + (C[d]-C[m])%mod)%mod;
        }
    }else{
        for(ll d = 1; d <= N; d++) {
            for(ll m = 2*d; m <= N; m += d) C[m] = (mod + (C[m]-C[d])%mod)%mod;
        }
    }
}


vector<ll> conv(vector<ll> A, vector<ll> B, bool g) {
    ll N = A.size() - 1ll;
    Z(A, N, g);
    Z(B, N, g); 
    vector<ll> C(N + 1ll);
    for (ll i = 1; i <= N; i++)  C[i] = (A[i] * B[i])%mod;
    Zinv(C, N, g);
    return C;
}
