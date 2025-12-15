#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define OK 0
#define ERR_SIZE 1

typedef struct {
    int rows;
    int cols;
    int data[MAX][MAX];
} Matrix;

// Đọc 2 ma trận từ 1 file CSV
int readTwoMatrix(const char *filename, Matrix *A, Matrix *B) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Khong mo duoc file %s\n", filename);
        return 0;
    }

    char line[256];
    Matrix *current = NULL;
    int row = 0;

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        if (strstr(line, "matran1")) {
            current = A;
            row = 0;
            continue;
        }

        if (strstr(line, "matran2")) {
            current = B;
            row = 0;
            continue;
        }

        int col = 0;
        char *token = strtok(line, ",");

        while (token) {
            current->data[row][col++] = atoi(token);
            token = strtok(NULL, ",");
        }

        current->cols = col;
        row++;
        current->rows = row;
    }

    fclose(f);
    return 1;
}

// In ra terminal
void printMatrix(Matrix m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            printf("%4d ", m.data[i][j]);
        }
        printf("\n");
    }
}

//Cộng
int add(Matrix a, Matrix b, Matrix *r) {
    if (a.rows != b.rows)
        return ERR_SIZE;
    if (a.cols != b.cols)
        return ERR_SIZE;

    r->rows = a.rows;
    r->cols = a.cols;

    for (int i = 0; i < a.rows; i++)
        for (int j = 0; j < a.cols; j++)
            r->data[i][j] = a.data[i][j] + b.data[i][j];

    return OK;
}

//Trừ
int sub(Matrix a, Matrix b, Matrix *r) {
    if (a.rows != b.rows)
        return ERR_SIZE;
    if (a.cols != b.cols)
        return ERR_SIZE;

    r->rows = a.rows;
    r->cols = a.cols;

    for (int i = 0; i < a.rows; i++)
        for (int j = 0; j < a.cols; j++)
            r->data[i][j] = a.data[i][j] - b.data[i][j];

    return OK;
}

//Nhân
#define ERR_MUL 2

int mul(Matrix a, Matrix b, Matrix *r) {
    if (a.cols != b.rows)
        return ERR_MUL;

    r->rows = a.rows;
    r->cols = b.cols;

    for (int i = 0; i < r->rows; i++) {
        for (int j = 0; j < r->cols; j++) {
            r->data[i][j] = 0;
            for (int k = 0; k < a.cols; k++)
                r->data[i][j] += a.data[i][k] * b.data[k][j];
        }
    }
    return OK;
}

int main(int argc, char *argv[]) {
    Matrix A, B, R;

    if (argc < 2) {
        printf("Cach dung: %s <ten_file_csv>\n", argv[0]);
        printf("Vi du: %s Book1.csv\n", argv[0]);
        return 1;
    }

    printf("Dang doc du lieu tu file: %s\n\n", argv[1]);

    if (!readTwoMatrix(argv[1], &A, &B)) {
        return 1;
    }

    printf("Ma tran A:\n");
    printMatrix(A);

    printf("\nMa tran B:\n");
    printMatrix(B);

    // if (add(A, B, &R)) {
    //     printf("\nA + B:\n");
    //     printMatrix(R);
    // } else printf("\nKhong cong duoc\n");
    int ret = add(A, B, &R);
    if (ret == OK) {
        printf("\nA + B:\n");
    printMatrix(R);
    } else {
        printf("\nKhong cong duoc vi:\n");
        printf("- So hang hoac so cot khong bang nhau\n");
        printf("  A: %dx%d, B: %dx%d\n",
            A.rows, A.cols, B.rows, B.cols);
    }

    ret = sub(A, B, &R);
    if (ret == OK) {
        printf("\nA - B:\n");
        printMatrix(R);
    } else {
        printf("\nKhong tru duoc vi:\n");
        printf("- So hang hoac so cot khong bang nhau\n");
        printf("  A: %dx%d, B: %dx%d\n",
            A.rows, A.cols, B.rows, B.cols);
    }

    ret = mul(A, B, &R);
    if (ret == OK) {
        printf("\nA * B:\n");
        printMatrix(R);
    } else {
        printf("\nKhong nhan duoc vi:\n");
        printf("- So cot A (%d) phai bang so hang B (%d)\n",
           A.cols, B.rows);
    }

    return 0;
}