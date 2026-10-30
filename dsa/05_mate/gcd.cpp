#include<bits/stdc++.h>

using namespace std;
#define ll long long

//Tested meme
ll gcd(ll a, ll b){
    if(b==0) return a;
    return gcd(b, a%b);
}

ll gcd(ll a, ll b){
    while(b) { ll r = a % b; a = b; b = r; }
    return a;
}