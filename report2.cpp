#include <iostream>
#include <vector>
#include <string>
using namespace std;

//반사 관계 판별 기능
bool isR(const auto &V){
    if(V[0][0] && V[1][1] && V[2][2] && V[3][3] && V[4][4]) return true;
    return false;
}
//대칭 관계 판별 기능
bool isS(const auto &V){
    for(int i=0; i<5; i++) for(int j=0; j<5; j++)
        if(V[i][j] && !V[j][i]) return false;
    return true;
}
//추이 관계 판별 기능
bool isT(const auto &V){
    for(int k=0; k<5; k++) for(int i=0; i<5; i++) for(int j=0; j<5; j++)
        if(V[i][k] && V[k][j] && !V[i][j]) return false;
    return true;
}
//반사 페포 구현 기능
void RC(auto &V){
    for(int i=0; i<5; i++) if(!V[i][i]) V[i][i] = 1;
}
//대칭 페포 구현 기능
void SC(auto &V){
    for(int i=0; i<5; i++) for(int j=0; j<5; j++)
        if(V[i][j] && !V[j][i]) V[j][i] = 1;
}
//추이 페포 구현 기능
void TC(auto &V){
    for(int k=0; k<5; k++) for(int i=0; i<5; i++) for(int j=0; j<5; j++)
        if(V[i][k] && V[k][j] && !V[i][j]) V[i][j] = 1;
}
//페포 구현 및 출력 기능
void closure(auto &V, int I){
    switch(I){
        case 0: if(isR(V)) {cout<<"이미 반사관계이다.\n"; return;} break;
        case 1: if(isS(V)) {cout<<"이미 대칭관계이다.\n"; return;} break;
        case 2: if(isT(V)) {cout<<"이미 추이관계이다.\n"; return;} break;
    }
    cout<<"폐포 변환 전 :\n";
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++) cout<<V[i][j]<<" ";
        cout<<'\n';
    }
    switch(I){
        case 0: RC(V); break;
        case 1: SC(V); break;
        case 2: TC(V); break;
    }
    cout<<"폐포 변환 후 :\n";
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++) cout<<V[i][j]<<" ";
        cout<<'\n';
    }
}
// 추가) 동치류에 의한 분할 및 출력 기능
void partition(const auto &V){
    vector<bool> visited(5, false);
    cout << "집합 A의 분할 (Partition A/R) = { ";
    for(int i=0; i<5; i++) {
        if(!visited[i]){
            cout<<"{ ";
            for(int j=0; j<5; j++){
                if(V[i][j]){
                    visited[j] = true;
                    cout<<j+1<<" ";
                }
            }cout << "} ";
        }
    }cout << "}\n";
}
//동치 관계 판별 + 동치류 출력 기능
bool isE(const auto &V){
    if(isR(V) && isS(V) && isT(V)){
        cout<<"이 관계는 동치 관계이다.\n";
        for(int i=0; i<5; i++){
            string S;
            for(int j=0; j<5; j++){
                if(V[i][j]) S += to_string(j+1)+" ";
            }
            cout<<"["<<i+1<<"] = { "<<S<<"}\n";
        }partition(V);
        return true;
    }
    cout<<"이 관계는 동치 관계가 아니다.\n";
    return false;
}

int main() {
    //관계 행렬 입력
    vector<vector<int>> V(5, vector<int>(5));
    for(int i=0; i<5; i++) for(int j=0; j<5; j++) cin>>V[i][j];
    //동치 관계 판별 기능 + 동치류 출력
    if(isE(V)) return 0;
    //페포 구현
    for(int i=0; i<3; i++) closure(V, i);
    //다시 동치 관계 판별
    isE(V);
    return 0;
}