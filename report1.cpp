#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<vector<double>> getMinor(const vector<vector<double>>& M, int row, int col) {
    int n = M.size();
    vector<vector<double>> minor;
    for (int i = 0; i < n; i++) {
        if (i == row) continue;
        vector<double> r;
        for (int j = 0; j < n; j++) {
            if (j == col) continue;
            r.push_back(M[i][j]);
        }
        minor.push_back(r);
    }
    return minor;
}

double getDeterminant(const vector<vector<double>>& M) {
    int n = M.size();
    if (n == 1) return M[0][0];
    if (n == 2) return M[0][0] * M[1][1] - M[0][1] * M[1][0];
    double det = 0;
    for (int j = 0; j < n; j++) {
        vector<vector<double>> minor = getMinor(M, 0, j);
        double cofactor = pow(-1, 0 + j) * getDeterminant(minor);
        det += M[0][j] * cofactor;
    }
    return det;
}

vector<vector<double>> getAdjugate(const vector<vector<double>>& M) {
    int n = M.size();
    vector<vector<double>> adj(n, vector<double>(n, 0));
    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<vector<double>> minor = getMinor(M, i, j);
            double cofactor = pow(-1, i + j) * getDeterminant(minor);
            adj[j][i] = cofactor;
        }
    }
    return adj;
}

bool invertMatrixGaussJordan(const vector<vector<double>>& A, vector<vector<double>>& inv) {
    int n = A.size();
    
    vector<vector<double>> aug(n, vector<double>(2 * n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug[i][j] = A[i][j];
        }
        aug[i][i + n] = 1.0;
    }
    for (int i = 0; i < n; i++) {
        int maxRow = i;
        for (int k = i + 1; k < n; k++) {
            if (abs(aug[k][i]) > abs(aug[maxRow][i])) {
                maxRow = k;
            }
        }
        swap(aug[i], aug[maxRow]);
        if (abs(aug[i][i]) < 1e-9) {
            return false; 
        }
        double pivot = aug[i][i];
        for (int j = 0; j < 2 * n; j++) {
            aug[i][j] /= pivot;
        }
        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = aug[k][i];
                for (int j = 0; j < 2 * n; j++) {
                    aug[k][j] -= factor * aug[i][j];
                }
            }
        }
    }
    inv.assign(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv[i][j] = aug[i][j + n];
        }
    }
    return true;
}

vector<vector<double>> multiplyMatrix(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

int main()
{
    //행렬 입력 기능
    cout << "정방행렬의 차수를 입력하세요: ";
    int n;
    if (!(cin >> n) || n <= 0) return 0;
    vector<vector<double>> M(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        cout << i + 1 << "행: ";
        for (int j = 0; j < n; j++) {
            cin >> M[i][j];
        }
    }
    //행렬식을 이용한 역행렬 계산 기능
    double det = getDeterminant(M);
    if (abs(det) < 1e-9){
        cout << "역행렬 없음! 종료!";
        return 0;
    }
    vector<vector<double>> adj = getAdjugate(M);
    vector<vector<double>> inv1(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv1[i][j] = adj[i][j] / det;
        }
    }
    // 가우스-조던 소거법을 이용한 역행렬 계산 기능
    vector<vector<double>> inv2(n, vector<double>(n));
    if (!invertMatrixGaussJordan(M, inv2)) {
        cout << "역행렬 없음! 종료!";
        return 0;
    }
    // 결과 출력 및 비교 기능
    cout << "\n행렬식으로 구한 역행렬:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(inv1[i][j]) < 1e-9) inv1[i][j] = 0.0;
            cout << inv1[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\n가우스-조던 소거법으로 구한 역행렬:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(inv2[i][j]) < 1e-9) inv2[i][j] = 0.0;
            cout << inv2[i][j] << "\t";
        }
        cout << "\n";
    }
    bool isSame = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(inv1[i][j] - inv2[i][j]) > 1e-7) {
                isSame = false;
                break;
            }
        }
        if (!isSame) break;
    }
    if (isSame) {
        cout << "\n두 방법의 결과가 동일합니다.\n";
    } else {
        cout << "\n두 방법의 결과가 동일하지 않습니다.\n";
    }
    // 단위행렬(M * M^-1 = I) 검증 기능
    vector<vector<double>> identityCheck = multiplyMatrix(M, inv2);

    bool isIdentity = true;
    cout << "\nM * M^-1 계산 결과:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (abs(identityCheck[i][j] - expected) > 1e-7) {
                isIdentity = false;
            }
            if (abs(identityCheck[i][j]) < 1e-9) identityCheck[i][j] = 0.0;
            
            cout << identityCheck[i][j] << "\t";
        }
        cout << "\n";
    }
    if (isIdentity) {
        cout << "\nM * M^-1의 결과가 단위행렬입니다!\n";
    } else {
        cout << "\nM * M^-1의 결과가 단위행렬과 다릅니다.\n";
    }
    return 0;
}