#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<ll> fb;

void ktao() {
    fb.push_back(1);
    fb.push_back(1);
    for (int i = 2; i <= 77; i++) {
        fb.push_back(fb[i - 1] + fb[i - 2]);
    }
}

bool is_fibo(ll n) {
    return binary_search(fb.begin(), fb.end(), n);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    ktao();
    int t;
    cin >> t;
    while(t--){
        ll n;
        cin >> n;
        while (!is_fibo(n)) {
            auto it = lower_bound(fb.begin(), fb.end(), n);
            if (it == fb.begin()) break;
            --it;
            n -= *it;
        }
        cout << n << '\n';
    }
}
