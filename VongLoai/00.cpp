#include <bits/stdc++.h>
using namespace std;

// Kiểu dữ liệu
#define ll long long
#define ld long double
#define pii pair<int,int>
#define pll pair<ll,ll>

// Truy cập nhanh
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define mp make_pair

// Vòng lặp
#define FOR(i,a,b) for (int i = (a); i <= (b); i++)
#define FOD(i,a,b) for (int i = (a); i >= (b); i--)
#define REP(i,n) for (int i = 0; i < (n); i++)

// Container viết gọn
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

// Hằng số
const int MOD = 1e9+7;
const ll INF = 1e18;

// Hàm tiện dụng
template<class T> bool ckmin(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template<class T> bool ckmax(T &a, const T &b) { return a < b ? a = b, 1 : 0; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Demo
    vector<int> v = {5,2,8,1};
    sort(all(v));
    for (auto x : v) cout << x << " "; // 1 2 5 8
}