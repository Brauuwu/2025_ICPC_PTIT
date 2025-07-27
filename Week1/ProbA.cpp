#include <bits/stdc++.h>
using namespace std;

struct p {
    int x, y;
};

bool ck(p a, p b, p c) {
    // Diện tích tam giác ≠ 0
    int area = a.x * (b.y - c.y) + 
               b.x * (c.y - a.y) + 
               c.x * (a.y - b.y);
    return area != 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, cnt = 0;
    cin >> n;
    p a[n];
    for (int i = 0; i < n; i++) {
        cin >> a[i].x >> a[i].y;
    }

    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                cnt += ck(a[i], a[j], a[k]);
            }
        }
    }
    cout << cnt;
}
