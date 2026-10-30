#include<bits/stdc++.h>

using namespace std;
#define ll long long
#define INF LLONG_MAX

struct p2d {
    ll x, y;
    p2d() {}
    p2d(ll x, ll y): x(x), y(y) {}
    p2d& operator+=(const p2d &p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    p2d& operator-=(const p2d &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    p2d& operator*=(ll c) {
        x *= c;
        y *= c;
        return *this;
    }
    p2d& operator/=(ll c) {
        x /= c;
        y /= c;
        return *this;
    }
    p2d operator+(const p2d &p) const {
        return p2d(*this) += p;
    }
    p2d operator-(const p2d &p) const {
        return p2d(*this) -= p;
    }
    p2d operator*(ll c) const {
        return p2d(*this) *= c;
    }
    p2d operator/(ll c) const {
        return p2d(*this) /= c;
    }
    ll dot(p2d a, p2d b){
        return a.x*b.x + a.y*b.y;
    }
    double norm2(p2d a) {
        return sqrt(dot(a, a));
    }
    double norm2sq(p2d a) {
        return dot(a, a);
    }
    p2d proj(p2d a, p2d b) {
        return b*(dot(a, b) / norm2sq(b));
    }
    double angle(p2d a, p2d b) {
        return acos(dot(a, b) / norm2(a) / norm2(b));
    }
    static ll cross(p2d a, p2d b) {
        return a.x * b.y - a.y * b.x;
    }
    p2d intersect_lines(p2d a1, p2d d1, p2d a2, p2d d2){
        ll crs = cross(d1, d2);
        if(crs==0) return p2d(INF, INF);
        return a1 + d1*(cross(a2 - a1, d2) / crs);
    }
};

struct p3d {
    ll x, y, z;
    p3d() {}
    p3d(ll x,ll y,ll z): x(x), y(y), z(z) {}
    p3d& operator+=(const p3d &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    p3d& operator-=(const p3d &p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
    p3d& operator*=(ll c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }
    p3d& operator/=(ll c) {
        x /= c;
        y /= c;
        z /= c;
        return *this;
    }
    p3d operator+(const p3d &p) const {
        return p3d(*this) += p;
    }
    p3d operator-(const p3d &p) const {
        return p3d(*this) -= p;
    }
    p3d operator*(ll c) const {
        return p3d(*this) *= c;
    }
    p3d operator/(ll c) const {
        return p3d(*this) /= c;
    }
    ll dot(p3d a, p3d b){
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    double norm2(p3d a) {
        return sqrt(dot(a, a));
    }
    double norm2sq(p3d a) {
        return dot(a, a);
    }
    p3d proj(p3d a, p3d b) {
        return b*(dot(a, b) / norm2sq(b));
    }
    double angle(p3d a, p3d b) {
        return acos(dot(a, b) / norm2(a) / norm2(b));
    }
    p3d cross(p3d a, p3d b) {
        return p3d(a.y * b.z - a.z * b.y,
                   a.z * b.x - a.x * b.z,
                   a.x * b.y - a.y * b.x);
    }
    ll triple(p3d a, p3d b, p3d c) {
        return dot(a, cross(b, c));
    }

    p3d intersect_planes(p3d a1, p3d n1, p3d a2, p3d n2, p3d a3, p3d n3){
        p3d x(n1.x, n2.x, n3.x);
        p3d y(n1.y, n2.y, n3.y);
        p3d z(n1.z, n2.z, n3.z); 
        p3d d(dot(a1, n1), dot(a2, n2), dot(a3, n3));
        return p3d(triple(d, y, z),triple(x, d, z), triple(x, y, d)) / triple(n1, n2, n3);
     }
};
//TESTED
//orientation of triangle
ll sign(const p2d& a, const p2d& b, const p2d& c){
    ll val = p2d::cross(b - a, c - a);
    if (val > 0) return 1;
    if (val < 0) return -1;
    return 0;
}
//TESTED
bool inter1(ll a, ll b, ll c, ll d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}
//TESTED
bool intersect_segments(const p2d& a, const p2d& b, const p2d& c, const p2d& d) {
    if (sign(a, b, c) == 0 && sign(a, b, d) == 0) return inter1(a.x, b.x, c.x, d.x) && inter1(a.y, b.y, c.y, d.y);
    ll ori1 = sign(a, b, c);
    ll ori2 = sign(a, b, d);
    ll ori3 = sign(c, d, a);
    ll ori4 = sign(c, d, b);
    return (ori1 != ori2) && (ori3 != ori4);
}