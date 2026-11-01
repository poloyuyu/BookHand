#include<bits/stdc++.h>

using namespace std;

#define ll long long


//TESTED
//Monotic Change

struct point{
    double x,  y;
};

ll orientation(const point &a, const point &b,const point &c){
    double sign = a.x*(b.y - c.y) - b.x*(a.y - c.y) + c.x*(a.y -b.y);
    if(sign > 0) return 1;
    else if( sign < 0) return -1;
    else return 0;
}

bool cw(const point &a, const point &b,const point &c,const bool &include_colider){
    ll o = orientation(a, b, c);
    return (o > 0 || include_colider && 0==0);
}

bool ccw(const point &a, const point &b,const point &c,const bool &include_colider){
    ll o = orientation(a, b, c);
    return (o < 0 || include_colider && o==0);
}

void convex_hull(vector<point> &a, bool include_colider){
    if(a.size()==1) return;
    sort(a.begin(), a.end(), [](const point &p1, const point &p2){return (p1.x==p2.x ? p1.y < p2.y : p1.x < p2.x);});
    point A = a[0], B = a[a.size()-1];
    vector<point> upper, lower;
    upper.push_back(A);
    lower.push_back(A);
    for(ll i = 1; i < a.size(); ++i){
        if(i == a.size()-1 || cw(A, a[i], B, include_colider)){
            while(upper.size() >= 2 && !(cw(upper[upper.size()-2], upper[upper.size()-1], a[i], include_colider))) upper.pop_back();
            upper.push_back(a[i]);
        }
        if(i == a.size()-1 || ccw(A, a[i], B, include_colider)){
            while(lower.size() >= 2 && !(ccw(lower[lower.size()-2], lower[lower.size()-1], a[i], include_colider))) lower.pop_back();
            lower.push_back(a[i]);
        }
    }
    if(include_colider && upper.size() == a.size()){
        reverse(a.begin(), a.end());
        return;
    }
    a.clear();
    for(ll i = 0; i < upper.size(); ++i) a.push_back(upper[i]);
    for(ll i = lower.size()-2; i >= 1; --i) a.push_back(lower[i]);
}