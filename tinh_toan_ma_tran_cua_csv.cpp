#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

/* ================= CẤU TRÚC MA TRẬN ================= */
struct Matrix {
    string name;      // matran1, matran2
    string source;    // Book1 hoặc Book2
    int rows = 0;
    int cols = 0;
    vector<vector<int>> data;
};

/* ================= HÀM XỬ LÝ BOM ================= */
void removeBOM(string &s) {
    if (s.size() >= 3 &&
        (unsigned char)s[0] == 0xEF &&
        (unsigned char)s[1] == 0xBB &&
        (unsigned char)s[2] == 0xBF) {
        s = s.substr(3);
    }
}

/* ================= ĐỌC FILE CSV ================= */
map<string, Matrix> readCSV(const string& filename, const string& source) {
    ifstream file(filename);
    map<string, Matrix> matrices;

    if (!file.is_open()) {
        cerr << "Khong mo duoc file: " << filename << endl;
        return matrices;
    }

    string line;
    Matrix current;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // Dòng tên ma trận
        if (line.find(',') == string::npos) {
            if (!current.name.empty()) {
                current.rows = current.data.size();
                current.cols = current.data[0].size();
                matrices[current.name] = current;
            }

            current = Matrix();
            removeBOM(line);          // <<< XỬ LÝ BOM
            current.name = line;
            current.source = source;
        }
        // Dòng dữ liệu
        else {
            stringstream ss(line);
            string value;
            vector<int> row;

            while (getline(ss, value, ',')) {
                row.push_back(stoi(value));
            }
            current.data.push_back(row);
        }
    }

    // Lưu ma trận cuối
    if (!current.name.empty()) {
        current.rows = current.data.size();
        current.cols = current.data[0].size();
        matrices[current.name] = current;
    }

    file.close();
    return matrices;
}

/* ================= IN MA TRẬN ================= */
void printMatrix(const Matrix& m) {
    cout << m.name << " (" << m.source << ") "
         << "(" << m.rows << "x" << m.cols << "):\n";
    for (auto& row : m.data) {
        for (int x : row)
            cout << x << "\t";
        cout << "\n";
    }
    cout << endl;
}

/* ================= PHÉP CỘNG ================= */
bool addMatrix(const Matrix& A, const Matrix& B, Matrix& C) {
    if (A.rows != B.rows || A.cols != B.cols)
        return false;

    C.name = A.name + "_cong_" + B.name;
    C.source = "Ket qua cong (" + A.source + " + " + B.source + ")";
    C.rows = A.rows;
    C.cols = A.cols;
    C.data.assign(A.rows, vector<int>(A.cols));

    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.data[i][j] = A.data[i][j] + B.data[i][j];

    return true;
}

/* ================= PHÉP TRỪ ================= */
bool subMatrix(const Matrix& A, const Matrix& B, Matrix& C) {
    if (A.rows != B.rows || A.cols != B.cols)
        return false;

    C.name = A.name + "_tru_" + B.name;
    C.source = "Ket qua tru (" + A.source + " - " + B.source + ")";
    C.rows = A.rows;
    C.cols = A.cols;
    C.data.assign(A.rows, vector<int>(A.cols));

    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < A.cols; j++)
            C.data[i][j] = A.data[i][j] - B.data[i][j];

    return true;
}

/* ================= PHÉP NHÂN ================= */
bool mulMatrix(const Matrix& A, const Matrix& B, Matrix& C) {
    if (A.cols != B.rows)
        return false;

    C.name = A.name + "_nhan_" + B.name;
    C.source = "Ket qua nhan (" + A.source + " x " + B.source + ")";
    C.rows = A.rows;
    C.cols = B.cols;
    C.data.assign(A.rows, vector<int>(B.cols, 0));

    for (int i = 0; i < A.rows; i++)
        for (int j = 0; j < B.cols; j++)
            for (int k = 0; k < A.cols; k++)
                C.data[i][j] += A.data[i][k] * B.data[k][j];

    return true;
}

/* ================= MAIN ================= */
int main() {
    vector<Matrix> allMatrices;

    auto book1 = readCSV("Book1.csv", "Book1");
    auto book2 = readCSV("Book2.csv", "Book2");

    for (auto& [_, m] : book1) allMatrices.push_back(m);
    for (auto& [_, m] : book2) allMatrices.push_back(m);

    int n = allMatrices.size();

    // Duyệt từng cặp ma trận duy nhất (i < j)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Matrix A = allMatrices[i];
            Matrix B = allMatrices[j];
            Matrix C;

            cout << "====================================\n";
            cout << "Xet cap ma tran:\n";
            cout << "- " << A.name << " (" << A.source << ")\n";
            cout << "- " << B.name << " (" << B.source << ")\n\n";

            // ====== CỘNG (1 lần) ======
            cout << "Tong giua " << A.name << " (" << A.source
                 << ") va " << B.name << " (" << B.source << "):\n";
            if (addMatrix(A, B, C))
                printMatrix(C);
            else
                cout << "Khong thuc hien duoc (khac kich thuoc)\n\n";

            // ====== TRỪ (cả A-B và B-A) ======
            cout << "Hieu giua " << A.name << " (" << A.source
                 << ") va " << B.name << " (" << B.source << "):\n";
            if (subMatrix(A, B, C))
                printMatrix(C);
            else
                cout << "Khong thuc hien duoc (khac kich thuoc)\n\n";

            cout << "Hieu giua " << B.name << " (" << B.source
                 << ") va " << A.name << " (" << A.source << "):\n";
            if (subMatrix(B, A, C))
                printMatrix(C);
            else
                cout << "Khong thuc hien duoc (khac kich thuoc)\n\n";

            // ====== NHÂN (cả A x B và B x A nếu điều kiện thỏa) ======
            cout << "Tich giua " << A.name << " (" << A.source
                 << ") va " << B.name << " (" << B.source << "):\n";
            if (mulMatrix(A, B, C))
                printMatrix(C);
            else
                cout << "Khong thuc hien duoc (cot A != dong B)\n\n";

            cout << "Tich giua " << B.name << " (" << B.source
                 << ") va " << A.name << " (" << A.source << "):\n";
            if (mulMatrix(B, A, C))
                printMatrix(C);
            else
                cout << "Khong thuc hien duoc (cot B != dong A)\n\n";
        }
    }

    return 0;
}
