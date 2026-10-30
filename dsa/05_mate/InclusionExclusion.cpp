
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ull unsigned long long
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7; 

//Example cardinality of all subsets of size 1
ll cardinalitySubset(ll subset){
    return 1;
}

//Template
ll InclusionExclusion(){
    //sets
    ll n = 10;//example
    ll subsets = (1<<n);//total subsets
    ll ans = 0;
    //travel on all the subsets, with a particular subset
    for(ll subset = 0; subset < subsets; subset++){
        //if is odd the subset we sum, otherwise substract, recall, Union of sets = sum sets - intersect sets pairs + intersect group of three - ...
        ans += (__builtin_popcount(subset)&1 ? cardinalitySubset(subset) : -cardinalitySubset(subset));
    }
    return ans;
}