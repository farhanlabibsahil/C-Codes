#include <stdio.h>

// Function to get the cofactor of A[p][q] in temp[][]
void getCofactor(int A[10][10], int temp[10][10], int p, int q, int n) {
    int i = 0, j = 0;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (row != p && col != q) {
                temp[i][j++] = A[row][col];
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Recursive function to find determinant
int determinant(int A[10][10], int n) {
    int det = 0;
    if (n == 1)
        return A[0][0];

    int temp[10][10];
    int sign = 1;

    for (int f = 0; f < n; f++) {
        getCofactor(A, temp, 0, f, n);
        det += sign * A[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }

    return det;
}

int main() {
    int n;
    printf("Enter the size of the matrix (n): ");
    scanf("%d", &n);

    int A[10][10]; // assuming n <= 10
    printf("Enter the elements of the matrix:\n");
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    int det = determinant(A, n);
    printf("Determinant of the matrix: %d\n", det);

    return 0;
}


