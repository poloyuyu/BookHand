#include<bits/stdc++.h>

using namespace std;
#define ll long long

//Tested

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