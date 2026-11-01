#include<bits/stdc++.h>

using namespace std;

#define ll long long

//n-catalan number = (2n)!/((n+1)!n!)
ll getNCatalanNumber(ll n){
    double ans = 1;
    for(ll k = 2; k <= n; k++) ans *= (n+k)/k;
    return (ll)(ans + 0.001);
}
//TESTED
vector<ll> getCatalanNumbers(ll n){
    vector<ll> C(n+1, 0);
    C[0] = C[1] = 1;
    C[2] = 2;
    for(ll i = 3; i <= n; ++i){
        for(ll j = 0; j < i; ++j) C[i] += C[j]*C[i-j-1];
    }
    return C;
}