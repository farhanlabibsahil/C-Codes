#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strrev(char*);

// Function to convert decimal to binary and return as a
// string
char* decimalToBinary(int decimal)
{
    // Allocate space for a 32-bit binary string + '\0'
    char* binary = (char*)malloc(33);

    int i = 0;
    // converting to binary
    while (decimal) {
        binary[i++] = '0' + (decimal & 1);
        decimal >>= 1;
    }
    binary[i] = '\0';
    strrev(binary);
    return binary;
}

// Function to convert binary to decimal and return as an
// integer
int binaryToDecimal(char binary[])
{
    int decimal = 0;
    int length = strlen(binary);
    for (int i = 0; i < length; i++) {
        decimal = decimal * 2 + (binary[i] - '0');
    }
    return decimal;
}

// Function to convert decimal to octal and return as a
// string
char* decimalToOctal(int decimal)
{
    // Allocate space for an octal string
    char* octal = (char*)malloc(12);
    if (octal == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    // Convert decimal to octal
    sprintf(octal, "%o", decimal);
    return octal;
}
// Function to convert octal to decimal and return as an
// integer
int octalToDecimal(char octal[])
{
    int decimal = 0;
    int length = strlen(octal);
    for (int i = 0; i < length; i++) {
        decimal = decimal * 8 + (octal[i] - '0');
    }
    return decimal;
}

// Function to convert hexadecimal to binary and return as a
// string
char* hexadecimalToBinary(char hex[])
{
    // converting hexadecimal string to integer
    unsigned int hexNum;
    sscanf(hex, "%x", &hexNum);
    // string to store binary number
    char binary[33] = "";
    // converting to hexadecimal
    int i = 0;
    while (hexNum) {
        binary[i++] = '0' + hexNum % 2;
        hexNum /= 2;
    }
    binary[i] = '\0';
    strrev(binary);

    return strdup(binary);
}

// Function to convert binary to hexadecimal and return as a
// string
char* binaryToHexadecimal(char binary[])
{
    // Pad the binary string with leading zeros to ensure
    // it's a multiple of 4
    int length = strlen(binary);
    int padding = (4 - (length % 4)) % 4;
    char paddedBinary[129];
    memset(paddedBinary, '0', padding);
    strcpy(paddedBinary + padding, binary);
    // Define a mapping of binary strings to their
    // hexadecimal representations
    char* binaryHexDigits[]
        = { "0000", "0001", "0010", "0011", "0100", "0101",
            "0110", "0111", "1000", "1001", "1010", "1011",
            "1100", "1101", "1110", "1111" };
    char hexadecimal[33] = ""; // Allocate space for an
                               // 8-digit hexadecimal string
    // Iterate through groups of 4 binary digits and convert
    // to hexadecimal
    for (int i = 0; i < length + padding; i += 4) {
        char group[5];
        strncpy(group, paddedBinary + i, 4);
        group[4] = '\0';
        // Find the corresponding hexadecimal digit
        for (int j = 0; j < 16; j++) {
            if (strcmp(group, binaryHexDigits[j]) == 0) {
                // Append the corresponding hexadecimal
                // digit
                char hexDigit[2];
                sprintf(hexDigit, "%X", j);
                strcat(hexadecimal, hexDigit);
                break;
            }
        }
    }
    return strdup(hexadecimal);
}

// driver code
int main()
{
    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Decimal to Binary\n");
        printf("2. Binary to Decimal\n");
        printf("3. Decimal to Octal\n");
        printf("4. Octal to Decimal\n");
        printf("5. Hexadecimal to Binary\n");
        printf("6. Binary to Hexadecimal\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 7) {
            printf("Goodbye!\n");
            break;
        }

        char input[100]; // Buffer for user input
        switch (choice) {
        case 1:
            printf("Enter a decimal number: ");
            scanf("%d", &choice);
            char* result = decimalToBinary(choice);
            printf("Decimal to Binary: %s\n", result);
            free(result);
            break;
        case 2:
            printf("Enter a binary number: ");
            scanf("%s", input);
            int binaryResult = binaryToDecimal(input);
            printf("Binary to Decimal: %d\n", binaryResult);
            break;
        case 3:
            printf("Enter a decimal number: ");
            scanf("%d", &choice);
            result = decimalToOctal(choice);
            printf("Decimal to Octal: %s\n", result);
            free(result);
            break;
        case 4:
            printf("Enter an octal number: ");
            scanf("%s", input);
            int octalResult = octalToDecimal(input);
            printf("Octal to Decimal: %d\n", octalResult);
            break;
        case 5:
            printf("Enter a hexadecimal number: ");
            scanf("%s", input);
            result = hexadecimalToBinary(input);
            printf("Hexadecimal to Binary: %s\n", result);
            free(result);
            break;
        case 6:
            printf("Enter a binary number: ");
            scanf("%s", input);
            result = binaryToHexadecimal(input);
            printf("Binary to Hexadecimal: %s\n", result);
            free(result);
            break;
        default:
            printf("Invalid choice. Please enter a valid "
                   "option.\n");
        }
    }

    return 0;
}

// reversing string
void strrev(char* str)
{
    int i = 0;
    int j = strlen(str) - 1;
    while (i < j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i++;
        j--;
    }
}
