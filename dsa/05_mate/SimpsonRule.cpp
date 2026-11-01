
#include<bits/stdc++.h>

using namespace std;

#define ll long long
#define ld long double
#define ull unsigned long long
#define for_(i, n) for(ll i = 0; i < n; ++i)
#define INF LLONG_MAX
#define NINF LLONG_MIN

const ll mod = 1e9 + 7; 

//TESTED

ld f(ld x) {return x;}

ld simpson_integration(ld a, ld b, ld d){
    const ll N = 10000; // number of steps (already multiplied by 2)
    ld h = (b - a) / N;
    ld s = f(a) + f(b); // a = x_0 and b = x_2n
    for(ll i= 1; i < N; i++) { // Refer to final Simpson's formula
        double xi = a + h * i;
        s += f(xi) * ((i & 1) ? 4 : 2);
    }
    s *= h / 3.0;
    //1e6 is the decimal precision, if you want 7, the 1e7 and so on
    s*=1e6;
    s=llround(s);
    s/=1e6;
    // to print use cout << fixed << setprecision(6) << s;
    return s;
}