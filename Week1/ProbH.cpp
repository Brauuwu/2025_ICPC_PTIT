#include <bits/stdc++.h>
using namespace std;

#define ll long long

// Hàm tìm số bước tối thiểu để làm A = B
int minSteps(int a, int b) {
    // Nếu A và B đã bằng nhau thì không cần bước nào
    if(a == b) return 0;

    // Tính abs giữa A và B
    int diff = abs(a - b);

    // k: số bước thực hiện
    int k = 0;

    // sum: tổng các số từ 1 đến k
    ll sum = 0;

    // Lặp đến khi tìm được k thỏa mãn điều kiện
    while (1) {
        sum += ++k; // tăng k lên 1 và cộng vào sum

        // Điều kiện dừng:
        // 1. Tổng sum phải >= hiệu diff
        // 2. (sum - diff) phải chia hết cho 2
        //
        // Lý do: trong quá trình cộng dồn, ta cần tổng phần tăng thêm phải "chia đôi" để chia đều cho A và B
        if (sum >= diff && (sum - diff) % 2 == 0)
            return k;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        int a, b;
        cin >> a >> b; 
        cout << minSteps(a, b) << '\n';
    }
}
