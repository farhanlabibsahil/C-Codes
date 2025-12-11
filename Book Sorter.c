#include <stdio.h>
#include <float.h> // For FLT_MAX and FLT_MIN

// Define the structure "Book"
struct Book {
    char title[100];
    char author[100];
    float price;
};

int main() {
    int n;

    
    printf("How many books do you want to enter? ");
    scanf("%d", &n);

    struct Book books[n]; 

    for (int i = 0; i < n; i++) {
        printf("\nInput details for Book %d:\n", i + 1);
        printf("Title: ");
        scanf("%s", books[i].title);  
        printf("Author: ");
        scanf("%s", books[i].author);
        printf("Price: ");
        scanf("%f", &books[i].price);
    }

    struct Book mostExpensive = books[0];
    struct Book lowestPriced = books[0];


    for (int i = 1; i < n; i++) {
        if (books[i].price > mostExpensive.price) {
            mostExpensive = books[i];
        }
        if (books[i].price < lowestPriced.price) {
            lowestPriced = books[i];
        }
    }

    printf("\nMost Expensive Book:\n");
    printf("Title: %s\n", mostExpensive.title);
    printf("Author: %s\n", mostExpensive.author);
    printf("Price: %.2f\n", mostExpensive.price);

    // Display lowest priced
    printf("\nLowest Priced Book:\n");
    printf("Title: %s\n", lowestPriced.title);
    printf("Author: %s\n", lowestPriced.author);
    printf("Price: %.2f\n", lowestPriced.price);

    return 0;
}
