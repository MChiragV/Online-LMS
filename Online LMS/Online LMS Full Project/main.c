#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

User users[MAX_USERS]; // Array to store user credentials
int num_users = 0;     // Number of users currently stored

void load_users() {
    // Load user credentials from a file (or database)
    // For simplicity, let's initialize with some default credentials
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s", users[num_users].username, users[num_users].password) == 2) {
            num_users++;
        }
        fclose(file);
    }
}

void save_users() {
    // Save user credentials to a file (or database)
    FILE *file = fopen("users.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < num_users; i++) {
            fprintf(file, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(file);
    }
}

int authenticate_librarian() {
    char username[MAX_USERNAME_LENGTH];
    char *password;
    printf("Enter librarian username: ");
    scanf("%s", username);
    password = getpass("Enter password: ");
    for (int i = 0; i < num_users; i++) {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0) {
            return 1; // Authentication successful
        }
    }
    return 0; // Authentication failed
}

void create_librarian_account() {
    if (num_users < MAX_USERS) {
        printf("Enter new librarian username: ");
        scanf("%s", users[num_users].username);
        strcpy(users[num_users].password, getpass("Enter password: "));
        num_users++;
        save_users();
        printf("Librarian account created successfully.\n");
    } else {
        printf("Cannot create new account. Maximum number of users reached.\n");
    }
}

void librarian_functionality() {
    printf("Librarian, do you want to create a new account (Y/N)? ");
    char choice;
    scanf(" %c", &choice); // Space before %c to consume any leading whitespace or newline
    if (tolower(choice) == 'y') {
        create_librarian_account();
    } else {
        if (!authenticate_librarian()) {
            printf("Authentication failed. Access denied.\n");
            return;
        }
    }

    // Run the b.c file
    system("./b");
}

void client_functionality() {
    // Run the s2.c file here, and c2.c file in new terminal
    system("./s2");
}

int main() {
    load_users(); // Load existing user credentials

    char user_type;

    // Welcome message
    printf("Hello! Welcome to the Online LMS.\n");

    // Ask user if they are a librarian or a client
    printf("Are you a librarian(l) or a client(c)? ");
    scanf("%c", &user_type);

    // Convert user input to lowercase for case-insensitive comparison
        user_type = tolower(user_type);

    // Check user type and call appropriate functionality
    if (user_type=='l') {
        librarian_functionality();
    } else if (user_type=='c') {
        client_functionality();
    } else {
        printf("Invalid user type.\n");
        return 1;
    }

    return 0;
}

