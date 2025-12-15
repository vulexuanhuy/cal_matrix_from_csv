#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;
using Matrix = vector<vector<double>>;

// Đọc file thông minh: Tự động tìm matran1, matran2 và bỏ qua dòng lỗi
void readData(string filename, Matrix &A, Matrix &B) {
    ifstream f(filename);
    if (!f.is_open()) return; 
    
    string line;
    Matrix *ptr = nullptr;
    while (getline(f, line)) {
        if (line.find("matran1") != string::npos) { ptr = &A; continue; }
        if (line.find("matran2") != string::npos) { ptr = &B; continue; }
        
        if (ptr && !line.empty()) {
            vector<double> row;
            stringstream ss(line);
            string val;
            while (getline(ss, val, ',')) {
                try { row.push_back(stod(val)); } catch(...) {}
            }
            if (!row.empty()) ptr->push_back(row);
        }
    }
}

// Hàm in ma trận (hoặc thông báo lỗi nếu rỗng)
void print(const Matrix &m, string name) {
    if (m.empty()) {
        cout << name << ": [Khong thuc hien duoc do sai kich thuoc]\n\n";
        return;
    }
    cout << "--- " << name << " (" << m.size() << "x" << m[0].size() << ") ---\n";
    for (const auto &row : m) {
        for (double val : row) cout << setw(8) << val << " ";
        cout << endl;
    }
    cout << endl;
}

int main() {
    Matrix A, B;
    readData("Book2.csv", A, B); // Đọc file Book2.csv

    // 1. In dữ liệu đầu vào
    print(A, "Ma tran 1 (Input)");
    print(B, "Ma tran 2 (Input)");

    int rA = A.size(), cA = A.empty() ? 0 : A[0].size();
    int rB = B.size(), cB = B.empty() ? 0 : B[0].size();

    // 2. Xử lý phép CỘNG/TRỪ (Yêu cầu cùng kích thước)
    if (rA == rB && cA == cB && rA > 0) {
        Matrix Sum = A, Diff = A;
        for(int i=0; i<rA; i++)
            for(int j=0; j<cA; j++) {
                Sum[i][j] += B[i][j];
                Diff[i][j] -= B[i][j];
            }
        print(Sum, "Ket qua TONG (A+B)");
        print(Diff, "Ket qua HIEU (A-B)");
    } else {
        cout << ">> Phep CONG/TRU: Khong kha thi (Kich thuoc " 
             << rA << "x" << cA << " khac " << rB << "x" << cB << ")\n\n";
    }

    // 3. Xử lý phép NHÂN (Cột A = Dòng B)
    if (cA == rB && cA > 0) {
        Matrix Prod(rA, vector<double>(cB, 0));
        for(int i=0; i<rA; i++)
            for(int j=0; j<cB; j++)
                for(int k=0; k<cA; k++)
                    Prod[i][j] += A[i][k] * B[k][j];
        print(Prod, "Ket qua TICH (A*B)");
    } else {
        cout << ">> Phep NHAN: Khong kha thi (Cot A=" << cA << " != Dong B=" << rB << ")\n";
    }

    return 0;
}