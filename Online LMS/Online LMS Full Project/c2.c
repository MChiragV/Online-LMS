#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_NAME_LENGTH 100

struct Book {
    int id;
    char title[MAX_NAME_LENGTH];
    char author[MAX_NAME_LENGTH];
};

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char server_ip[] = "127.0.0.1"; // Loopback address for local testing

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // User choice
    int choice;
    printf("Do you want to:\n1. Search Book\n2. Exit\nEnter your choice: ");
    scanf("%d", &choice);

    // Send choice to server
    send(client_socket, &choice, sizeof(int), 0);

    if (choice == 1) {
        // If user chose to search for a book, prompt for book ID
        int idToSearch;
        printf("Enter ID of book to search: ");
        scanf("%d", &idToSearch);

        // Send book ID to server
        send(client_socket, &idToSearch, sizeof(int), 0);

        // Receive search result from server
        int found;
        struct Book foundBook;
        recv(client_socket, &found, sizeof(int), 0);
        if (found) {
            recv(client_socket, &foundBook, sizeof(struct Book), 0);
            printf("Book found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\n", foundBook.id, foundBook.title, foundBook.author);
        } else {
            printf("Book not found.\n");
        }
    }

    // Close client socket
    close(client_socket);

    return 0;
}

