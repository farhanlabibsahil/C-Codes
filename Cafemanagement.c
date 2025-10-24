#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Menu
{
    int id;
    char name[50];
    float price;
};

struct OrderItem
{
    int id;
    char name[50];
    float price;
    int quantity;
    float total;
};

//File paths
const char *menuFile = "menu.txt";
const char *ratingFile = "rating.txt";
const char *orderFile = "order.txt";
const char *statsFile = "stat.txt";

//  Admin Login
int adminLogin()
{
    char userid[50], password[50];
    printf("----- Admin Login -----\n");
    printf("User Id: ");
    scanf("%s", userid);
    printf("Password: ");
    scanf("%s", password);
    return (strcmp(userid, "admin") == 0 && strcmp(password, "1234") == 0);
}

//Admin Functions
void addMenuItem()
{
    FILE *fp = fopen(menuFile, "a");
    struct Menu item;
    int n;

    if (fp==NULL)
    {
        printf("Couldn't open the file\n");
        return;
    }

    printf("How many items do you want to add?: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("Enter ID, Name and Price of the item %d: ", i + 1);
        scanf("%d %s %f", &item.id, item.name, &item.price);
        fprintf(fp, "%d %s %.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
    printf("Menu added successfully by the admin\n");
}

void deleteMenuItem()
{
    int target, found = 0;
    struct Menu item;
    FILE *fp = fopen(menuFile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the ID of the target: ");
    scanf("%d", &target);

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF)
    {
        if (target != item.id)
        {
            fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.price);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove(menuFile);
    rename("temp.txt", menuFile);

    printf(found ? "Item deleted successfully\n" : "Item not found\n");
}

void modifyMenuItem()
{
    int target, found = 0;
    struct Menu item;
    FILE *fp = fopen(menuFile, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter the ID of the target: ");
    scanf("%d", &target);

    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF)
    {
        if (target == item.id)
        {
            printf("Enter new name: ");
            scanf("%s", item.name);
            printf("Enter new price: ");
            scanf("%f", &item.price);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
    fclose(temp);
    remove(menuFile);
    rename("temp.txt", menuFile);

    printf(found ? "Item modified successfully\n" : "Item not found\n");
}

void ratingView()
{
    FILE *fp = fopen(ratingFile, "r");
    char name[50];
    int rating;

    if (!fp)
    {
        printf("No ratings found.\n");
        return;
    }

    printf("\n==== Customer Ratings ====\n");
    printf("Name\tRating\n-------------------\n");
    while (fscanf(fp, "%s %d", name, &rating) != EOF)
    {
        printf("%-10s%d\n", name, rating);
    }

    fclose(fp);
}

void viewStats()
{
    FILE *fp = fopen(statsFile, "r");
    int customers;
    float revenue;

    if (!fp)
    {
        printf("No stats available yet.\n");
        return;
    }

    fscanf(fp, "%d %f", &customers, &revenue);
    printf("\n==== Cafe Stats for Today ====\n");
    printf("Total Customers: %d\n", customers);
    printf("Total Revenue: %.2f\n", revenue);

    fclose(fp);
}

//Customer Functions
void displayMenu()
{
    FILE *fp;
    struct Menu item;
    fp = fopen(menuFile, "r");

    if (fp == NULL)
    {
        printf("No item added. Please inform admin to do so.\n");
        return;
    }

    printf("---------- Welcome to Our Cafe ----------\n");
    printf("ID\tName\tPrice\n");
    printf("---------------------------\n");
    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF)
    {
        printf("%d\t%s\t%.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
}

void placeOrder()
{
    FILE *fp = fopen(menuFile, "r");
    FILE *order = fopen(orderFile, "a");
    struct Menu item;
    struct OrderItem orderItem[100];
    int orderCount = 0, id, quantity;
    float total = 0;

    if (!fp || !order)
    {
        printf("Error opening file.\n");
        return;
    }

    displayMenu();
    printf("Enter the Item ID, (0 to finish): ");
    while (1)
    {
        scanf("%d", &id);
        if (id == 0) break;
        rewind(fp);
        int found = 0;
        while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF)
        {
            if (item.id == id)
            {
                printf("Enter the quantity for %s: ", item.name);
                scanf("%d", &quantity);

                strcpy(orderItem[orderCount].name, item.name);
                orderItem[orderCount].id = item.id;
                orderItem[orderCount].price = item.price;
                orderItem[orderCount].quantity = quantity;
                orderItem[orderCount].total = item.price * quantity;

                total += orderItem[orderCount].total;
                orderCount++;
                found = 1;
                break;
            }
        }
        if (!found)
            printf("Invalid item ID.\n");
        else
            printf("Enter next order ID, (0 to finish): ");
    }

    for (int i = 0; i < orderCount; i++)
    {
        fprintf(order, "%d %s %.2f %d %.2f\n",
                orderItem[i].id,
                orderItem[i].name,
                orderItem[i].price,
                orderItem[i].quantity,
                orderItem[i].total);
    }
    fprintf(order, "Total: %.2f\n", total);

    fclose(fp);
    fclose(order);
    printf("Order placed successfully\n");
}

void viewBill()
{
    FILE *fp = fopen(orderFile, "r");
    char line[100];

    if (!fp)
    {
        printf("No order found.\n");
        return;
    }

    printf("\n===== Order Summary =====\n");
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }

    fclose(fp);
}

void updateCustomerandRevenue(float totalBill)
{
    int customers = 0;
    float revenue = 0;
    FILE *fp = fopen(statsFile, "r");

    if (fp)
    {
        fscanf(fp, "%d %f", &customers, &revenue);
        fclose(fp);
    }

    customers++;
    revenue += totalBill;

    fp = fopen(statsFile, "w");
    if (fp)
    {
        fprintf(fp, "%d %.2f", customers, revenue);
        fclose(fp);
    }
}

void makePayment()
{
    FILE *fp = fopen(orderFile, "r");
    float total = 0, amount, due;
    char temp[100];

    if (!fp)
    {
        printf("No order to pay.\n");
        return;
    }


    while (fgets(temp, sizeof(temp), fp))
    {
        if (sscanf(temp, "Total: %f", &total) == 1) break;
    }
    fclose(fp);

    if (total <= 0)
    {
        printf("No payment required.\n");
        return;
    }

    due = total;
    printf("Your total bill is: %f\n", total);

    while (due > 0)
    {
        printf("Enter payment amount: ");
        scanf("%f", &amount);

        if (amount < due)
        {
            due -= amount;
            printf("Partial payment accepted. Remaining due: %f\n", due);
        }
        else if (amount == due)
        {
            printf("Payment complete. Thank you!\n");
            due = 0;
        }
        else
        {
            printf("Payment complete. Change: %f\n", amount - due);
            due = 0;
        }
    }

    updateCustomerandRevenue(total);
    remove(orderFile);
}

void giveFeedback()
{
    char name[50];
    int rating;
    FILE *fp = fopen(ratingFile, "a");

    if (!fp)
    {
        printf("Error opening rating file.\n");
        return;
    }

    printf("Enter your name: ");
    scanf("%s", name);
    printf("Rate your experience (1 to 5): ");
    scanf("%d", &rating);

    if (rating < 1 || rating > 5)
    {
        printf("Invalid rating. Please enter between 1 to 5.\n");
        fclose(fp);
        return;
    }

    fprintf(fp, "%s %d\n", name, rating);
    fclose(fp);
    printf("Thanks for your feedback!\n");
}

//Menus
void adminMenu()
{
    int choice;
    do
    {
        printf("\n\n==== Admin Panel ====\n");
        printf("1. Add Menu Item\n");
        printf("2. Display Menu\n");
        printf("3. Delete Menu Item\n");
        printf("4. Modify Menu Item\n");
        printf("5. View Ratings\n");
        printf("6. View Today's Stats\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addMenuItem();
            break;
        case 2:
            displayMenu();
            break;
        case 3:
            deleteMenuItem();
            break;
        case 4:
            modifyMenuItem();
            break;
        case 5:
            ratingView();
            break;
        case 6:
            viewStats();
            break;
        case 7:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    while (choice != 7);
}

void customerMenu()
{
    int choice;
    do
    {
        printf("\n\n==== Customer Menu ====\n");
        printf("1. View Menu\n");
        printf("2. Place Order\n");
        printf("3. View Bill\n");
        printf("4. Pay Bill\n");
        printf("5. Give Feedback\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayMenu();
            break;
        case 2:
            placeOrder();
            break;
        case 3:
            viewBill();
            break;
        case 4:
            makePayment();
            break;
        case 5:
            giveFeedback();
            break;
        case 6:

        {
            FILE *fp = fopen(orderFile, "r");
            if (fp)
            {
                fclose(fp);
                printf("You must pay your bill before exiting!\n");
                choice = 0;
            }
            else
            {
                printf("Thank you! Visit again.\n");
            }
        }
        break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    while (choice != 6);
}


int main()
{
    int role;
    printf("\n==== Welcome to Smart Cafe ====\n");
    printf("1. Admin\n2. Customer\nSelect your role: ");
    scanf("%d", &role);

    if (role == 1)
    {
        if (adminLogin())
        {
            adminMenu();
        }
        else
        {
            printf("Access Denied!\n");
        }
    }
    else if (role == 2)
    {
        customerMenu();
    }
    else
    {
        printf("Invalid option.\n");
    }

    return 0;
}
