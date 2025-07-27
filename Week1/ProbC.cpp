#include<bits/stdc++.h>

using namespace std;

main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    cin >> s;
    int n = s.size();
    int start = -1, end = -1;
    for(int i=0; i<n; i++){
        if(s[i] == 'A'){
            start = i;
            break;
        }
    }
    if(start == -1){
        cout << "0";
        return 0;
    }
    for(int i=n-1; i>start; i--){
        if(s[i] == 'Z') {
            end = i;
            break;
        }
    }
    if(end == -1) cout << ("0");
    else cout << end - start + 1;
}
