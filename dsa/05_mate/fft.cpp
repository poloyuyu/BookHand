#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define cd complex<long double>
const double PI = 3.141592653589793238462643383279502884197169399375105;

//Tested

void fft_bottom_up(vector<cd> &c, bool inverse){
    ll N = 1;
    while(N < c.size()) N<<=1;
    c.resize(N);
    if(N<= 1) return;
    ll cnt = 0;
    for(ll j = 1; j < N; ++j){
        ll bit = N >> 1;
        while(cnt & bit){
            cnt^=bit;
            bit>>=1;
        }
        cnt^=bit;
        if(cnt < j) swap(c[cnt], c[j]);
    }

    //n is the length of the current block of the tree, thanks to bit reversal is in order, n=2 -> depth= D-1, n=4 -> depth=D-2, and so on...
    for(ll n = 2; n <= N; n<<=1){
        double ang = 2*PI/n * (inverse ? -1 : 1);
        cd wn(cos(ang), sin(ang));
        //iterate each block(node from the current depth)
        for(ll currblock = 0; currblock < N; currblock+=n){
            cd w(1);
            //iterate the node of the current block, only recover a half, because butterfly effect(como se diga la wea).
            for(ll curr = 0; curr < n/2; ++curr){
                cd u = c[currblock + curr], v = c[currblock + curr + n/2]*w;
                c[currblock + curr] = u + v;
                c[currblock + curr + n/2] = u - v;
                w*=wn;
            }
        }
    }
    if(inverse){
        for(cd &x : c) x/=N; 
    }
}

vector<cd> conv(vector<cd> c1, vector<cd> c2){
    vector<cd> ans; ll N=1;
    while(N < (c1.size()+c2.size()-1ll)) N<<=1;
    c1.resize(N); c2.resize(N);
    fft_bottom_up(c1, false);
    fft_bottom_up(c2, false);
    ans.resize(N);
    for_(i, N) ans[i] = c1[i]*c2[i];
    fft_bottom_up(ans, true);
    return ans;
}

