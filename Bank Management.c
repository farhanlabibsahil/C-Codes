#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // for sleep() and usleep()
#include <time.h>
#include <termios.h> // For getch() like functionality

// --- ANSI Escape Code Functions for Cross-Platform TUI ---

// Cross-platform gotoxy() using ANSI escape codes
void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Clear screen
void clearscreen() {
    printf("\033[2J\033[1;1H");
}

// Function to clear input buffer (necessary after scanf)
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Structure and Global Variables ---

// Simple structure for account
struct account {
    int number;
    char name[50];
    float balance;
};

struct account accounts[100];
int totalAccounts = 0;

// --- Function Prototypes ---

// System Functions
void loadingScreen();
void saveAccountsToFile();
void loadAccountsFromFile();

// Menu Functions
void loginScreen();
void adminLogin();
void adminMenu();
void customerMenu(int accNo);

// Account Operations
void createAccount();
void viewAccounts();
void depositMoney();
void withdrawMoney(int accNo); // Updated to take account number
void searchAccount(int accNo, int isAdmin); // Updated to take account number and admin flag

// Utility for password input
void getPassword(char *password, int maxLen);

// --- Main Function ---
int main() {
    loadAccountsFromFile(); // Load accounts when starting
    loadingScreen();
    loginScreen();
    saveAccountsToFile(); // Save accounts on exit

    clearscreen();
    gotoxy(20, 7);
    printf("✅ Thank you for using our system!\n");
    sleep(1);

    return 0;
}

// --- Utility Functions ---

// Loading animation
void loadingScreen() {
    clearscreen();
    gotoxy(25, 8);
    printf("Loading");
    fflush(stdout);
    for (int i = 0; i < 5; i++) {
        usleep(400000);
        printf(".");
        fflush(stdout);
    }
    usleep(400000);
}

// Save all accounts to file (Using accounts.txt)
void saveAccountsToFile() {
    // Open in write mode ("w") for text file
    FILE *fp = fopen("accounts.txt", "w");
    if (fp == NULL) {
        if (totalAccounts > 0) printf("Error saving file!\n");
        return;
    }
    
    // Write the total number of accounts first
    fprintf(fp, "%d\n", totalAccounts);

    // Write each account record
    for (int i = 0; i < totalAccounts; i++) {
        // Format: AccountNumber,Name,Balance
        fprintf(fp, "%d,%s,%.2f\n", 
                accounts[i].number, accounts[i].name, accounts[i].balance);
    }
    fclose(fp);
}

// Load all accounts from file (Using accounts.txt)
void loadAccountsFromFile() {
    // Open in read mode ("r") for text file
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) {
        totalAccounts = 0; // No file yet, start empty
        return;
    }
    
    // Read the total number of accounts first
    if (fscanf(fp, "%d\n", &totalAccounts) != 1) {
        // File is empty or corrupt
        totalAccounts = 0;
        fclose(fp);
        return;
    }

    // Read each account record
    for (int i = 0; i < 100 && i < totalAccounts; i++) {
        // Use fscanf with delimiter (comma) for structured text data
        if (fscanf(fp, "%d,%49[^,],%f\n", 
                   &accounts[i].number, accounts[i].name, &accounts[i].balance) != 3) {
            // If reading fails for any record, stop loading and adjust totalAccounts
            totalAccounts = i;
            break; 
        }
    }
    fclose(fp);
}

// Function to read password without echoing characters (like getch)
void getPassword(char *password, int maxLen) {
    // This is a common way to achieve non-echoing input on POSIX systems (Linux/macOS)
    // On Windows, you would typically use <conio.h> and getch()
    struct termios oldt, newt;
    int i = 0;
    int c;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Turn off canonical mode (line buffering) and echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((c = getchar()) != '\n' && c != EOF && i < maxLen - 1) {
        if (c == 127 || c == 8) { // Handle backspace (ASCII 127 and 8)
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            password[i++] = c;
            printf("*");
            fflush(stdout);
        }
    }
    password[i] = '\0';

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}


// --- Menu Functions ---

void loginScreen() {
    int choice;
    do {
        clearscreen();
        gotoxy(20, 3);
        printf("🏦 BANK MANAGEMENT SYSTEM - LOGIN");
        gotoxy(20, 5);
        printf("1. Administrator Login");
        gotoxy(20, 6);
        printf("2. Customer Login");
        gotoxy(20, 7);
        printf("3. Exit");

        gotoxy(20, 9);
        printf("Enter your choice: ");
        
        // Read choice
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer(); // Clear buffer if input fails
            choice = 0;
        } else {
            clearInputBuffer(); // Clear buffer after successful integer read
        }

        switch (choice) {
            case 1: adminLogin(); break;
            case 2:
                {
                    int accNo;
                    clearscreen();
                    gotoxy(20, 4);
                    printf("CUSTOMER LOGIN");
                    gotoxy(20, 6);
                    printf("Enter your Account Number: ");
                    if (scanf("%d", &accNo) != 1) {
                        clearInputBuffer();
                        accNo = 0; // Invalid account number
                    } else {
                        clearInputBuffer();
                    }
                    
                    // Check if account exists before proceeding
                    int found = 0;
                    for(int i = 0; i < totalAccounts; i++) {
                        if (accounts[i].number == accNo) {
                            found = 1;
                            customerMenu(accNo);
                            break;
                        }
                    }
                    if (!found) {
                        gotoxy(20, 8);
                        printf("Account not found!");
                        sleep(2);
                    }
                }
                break;
            case 3: return; // Exit main do-while loop
            default:
                gotoxy(20, 11);
                printf("Invalid choice! Try again.");
                sleep(1);
        }
    } while (choice != 3);
}

void adminLogin() {
    char username[20];
    char password[20];
    clearscreen();
    gotoxy(20, 4);
    printf("ADMINISTRATOR LOGIN");
    gotoxy(20, 6);
    printf("Username: ");
    
    // Use fgets to read username safely
    if (fgets(username, sizeof(username), stdin) == NULL) {
        strcpy(username, "");
    }
    // Remove newline character if present
    username[strcspn(username, "\n")] = 0;

    gotoxy(20, 7);
    printf("Password: ");

    // Using the getPassword utility
    getPassword(password, 20);

    // After getPassword, clear any residual input
    clearInputBuffer(); 

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        adminMenu();
    } else {
        gotoxy(20, 9);
        printf("\nInvalid credentials! Press Enter to continue...");
        getchar();
    }
}

void adminMenu() {
    int choice;
    do {
        clearscreen();
        gotoxy(20, 3);
        printf("👑 ADMIN MENU");
        gotoxy(20, 5);
        printf("1. Create Account");
        gotoxy(20, 6);
        printf("2. View All Accounts");
        gotoxy(20, 7);
        printf("3. Deposit Money (Admin Only)");
        gotoxy(20, 8);
        printf("4. Search Account");
        gotoxy(20, 9);
        printf("5. Logout");

        gotoxy(20, 11);
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = 0;
        } else {
            clearInputBuffer();
        }

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccounts(); break;
            case 3: depositMoney(); break;
            case 4: searchAccount(0, 1); break; // 0 for placeholder, 1 for isAdmin
            case 5:
                gotoxy(20, 13);
                printf("Saving and Logging out...");
                saveAccountsToFile();
                sleep(1);
                return;
            default:
                gotoxy(20, 13);
                printf("Invalid choice! Try again.");
                sleep(1);
        }
    } while (choice != 5);
}

void customerMenu(int accNo) {
    int choice;
    do {
        clearscreen();
        gotoxy(20, 3);
        printf("👤 CUSTOMER MENU (Account: %d)", accNo);
        gotoxy(20, 5);
        printf("1. View My Account Details");
        gotoxy(20, 6);
        printf("2. Withdraw Money");
        gotoxy(20, 7);
        printf("3. Logout");

        gotoxy(20, 9);
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            choice = 0;
        } else {
            clearInputBuffer();
        }

        switch (choice) {
            case 1: searchAccount(accNo, 0); break; // Pass customer's accNo, 0 for isCustomer
            case 2: withdrawMoney(accNo); break; // Pass customer's accNo
            case 3:
                gotoxy(20, 11);
                printf("Saving and Logging out...");
                saveAccountsToFile();
                sleep(1);
                return;
            default:
                gotoxy(20, 11);
                printf("Invalid choice! Try again.");
                sleep(1);
        }
    } while (choice != 3);
}

// --- Account Operation Functions ---

// Create a new account
void createAccount() {
    clearscreen();
    gotoxy(20, 4);
    printf("CREATE NEW ACCOUNT");

    // Input Account Number with basic check for array overflow
    if (totalAccounts >= 100) {
        gotoxy(20, 6);
        printf("Maximum number of accounts reached!");
        sleep(2);
        return;
    }

    // Input data
    gotoxy(20, 6);
    printf("Enter account number: ");
    if (scanf("%d", &accounts[totalAccounts].number) != 1) {
        clearInputBuffer();
        gotoxy(20, 8);
        printf("Error: Invalid input for account number!");
        sleep(2);
        return;
    }
    clearInputBuffer(); // Clear after int read

    // Basic check for duplicate account number (simple linear search)
    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].number == accounts[totalAccounts].number) {
            gotoxy(20, 8);
            printf("Error: Account number already exists!");
            sleep(2);
            return;
        }
    }

    gotoxy(20, 7);
    printf("Enter name: ");
    // Using fgets for string input
    if (fgets(accounts[totalAccounts].name, 50, stdin) == NULL) {
        strcpy(accounts[totalAccounts].name, "N/A");
    }
    accounts[totalAccounts].name[strcspn(accounts[totalAccounts].name, "\n")] = 0;

    gotoxy(20, 8);
    printf("Enter initial balance: ");
    if (scanf("%f", &accounts[totalAccounts].balance) != 1) {
        clearInputBuffer();
        gotoxy(20, 10);
        printf("Error: Invalid input for balance!");
        sleep(2);
        return;
    }
    clearInputBuffer(); // Clear after float read

    totalAccounts++;
    saveAccountsToFile();

    gotoxy(20, 10);
    printf("Account created successfully!");
    sleep(2);
}

// View all accounts (Admin only)
void viewAccounts() {
    clearscreen();
    gotoxy(20, 4);
    printf("ALL ACCOUNT DETAILS (%d Total)\n", totalAccounts);
    gotoxy(10, 6);
    printf("Acc No.\t\tName\t\t\tBalance\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < totalAccounts; i++) {
        printf("%d\t\t%-20s\t%.2f\n", accounts[i].number, accounts[i].name, accounts[i].balance);
    }
    gotoxy(10, 15);
    printf("Press Enter to return to menu...");
    getchar();
}

// Deposit money (Admin only for now, can be extended)
void depositMoney() {
    int accNo;
    float amount;
    clearscreen();
    gotoxy(20, 4);
    printf("DEPOSIT MONEY");
    gotoxy(20, 6);
    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        clearInputBuffer();
        gotoxy(20, 10);
        printf("Error: Invalid input for account number!");
        sleep(2);
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].number == accNo) {
            gotoxy(20, 8);
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1) {
                clearInputBuffer();
                gotoxy(20, 10);
                printf("Error: Invalid input for amount!");
                sleep(2);
                return;
            }
            clearInputBuffer();

            if (amount < 0) {
                gotoxy(20, 10);
                printf("Deposit amount cannot be negative!");
                sleep(2);
                return;
            }
            accounts[i].balance += amount;
            saveAccountsToFile();
            gotoxy(20, 10);
            printf("Amount %.2f deposited successfully! New Balance: %.2f", amount, accounts[i].balance);
            sleep(2);
            return;
        }
    }
    gotoxy(20, 10);
    printf("Account not found!");
    sleep(2);
}

// Withdraw money (Used by both Customer and Admin)
void withdrawMoney(int accNo) {
    float amount;
    int targetAccNo = accNo; // For customer, this is their fixed account number

    clearscreen();
    gotoxy(20, 4);
    printf("WITHDRAW MONEY");

    // If accNo is 0 (Admin call), prompt for account number
    if (accNo == 0) {
        gotoxy(20, 6);
        printf("Enter account number: ");
        if (scanf("%d", &targetAccNo) != 1) {
            clearInputBuffer();
            gotoxy(20, 10);
            printf("Error: Invalid input for account number!");
            sleep(2);
            return;
        }
        clearInputBuffer();
    } else {
        // Customer view, just display the account
        gotoxy(20, 6);
        printf("Account Number: %d", accNo);
    }

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].number == targetAccNo) {
            gotoxy(20, 8);
            printf("Enter amount to withdraw: ");
            if (scanf("%f", &amount) != 1) {
                clearInputBuffer();
                gotoxy(20, 10);
                printf("Error: Invalid input for amount!");
                sleep(2);
                return;
            }
            clearInputBuffer();

            if (amount <= 0) {
                 gotoxy(20, 10);
                printf("Withdrawal amount must be positive!");
            } else if (amount > accounts[i].balance) {
                gotoxy(20, 10);
                printf("Insufficient balance! Current: %.2f", accounts[i].balance);
            } else {
                accounts[i].balance -= amount;
                saveAccountsToFile();
                gotoxy(20, 10);
                printf("Withdrawal successful! New Balance: %.2f", accounts[i].balance);
            }
            sleep(2);
            return;
        }
    }
    gotoxy(20, 10);
    printf("Account not found!");
    sleep(2);
}

// Search an account
void searchAccount(int accNo, int isAdmin) {
    int targetAccNo = accNo;

    clearscreen();
    gotoxy(20, 4);
    printf("SEARCH ACCOUNT");

    // If not a customer search, or if Admin, prompt for account number
    if (accNo == 0 || isAdmin) {
        gotoxy(20, 6);
        printf("Enter account number: ");
        if (scanf("%d", &targetAccNo) != 1) {
            clearInputBuffer();
            gotoxy(20, 10);
            printf("Error: Invalid input for account number!");
            sleep(2);
            return;
        }
        clearInputBuffer();
    } else {
        // Customer search, just display their account
        gotoxy(20, 6);
        printf("Searching for Account Number: %d", accNo);
    }

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].number == targetAccNo) {
            gotoxy(20, 8);
            printf("Account Details:");
            gotoxy(22, 9);
            printf("No: %d", accounts[i].number);
            gotoxy(22, 10);
            printf("Name: %s", accounts[i].name);
            gotoxy(22, 11);
            printf("Balance: %.2f", accounts[i].balance);
            gotoxy(20, 13);
            printf("Press Enter to return to menu...");
            getchar();
            return;
        }
    }
    gotoxy(20, 10);
    printf("Account not found!");
    sleep(2);
}
