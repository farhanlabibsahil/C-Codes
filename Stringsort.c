#include <stdio.h>

int main() {
    int arr[100], even[100], odd[100];
    int i, n, e = 0, o = 0;

    // Input array size
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Input elements
    printf("Enter %d integers:\n", n);
    for(i = 0; i < n; i++) {
        scanf("%d", &arr[i]);

        // Separate even and odd
        if(arr[i] % 2 == 0) {
            even[e++] = arr[i];
        } else {
            odd[o++] = arr[i];
        }
    }

    // Output
    printf("\nOriginal array: ");
    for(i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\nEven elements: ");
    for(i = 0; i < e; i++) {
        printf("%d ", even[i]);
    }

    printf("\nOdd elements: ");
    for(i = 0; i < o; i++) {
        printf("%d ", odd[i]);
    }

    return 0;
}
