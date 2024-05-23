#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define MAX_NAME_LENGTH 100

struct Book {
    int id;
    char title[MAX_NAME_LENGTH];
    char author[MAX_NAME_LENGTH];
};

void handle_client(int client_socket) {
    int choice;
    int idToSearch;
    struct Book tempBook;

    // Receive choice from client
    recv(client_socket, &choice, sizeof(int), 0);

    switch (choice) {
        case 1: {
            // Receive ID to search from client
            recv(client_socket, &idToSearch, sizeof(int), 0);

            // Search for the book
            FILE *file = fopen("books.txt", "r");
            int found = 0;
            while (fread(&tempBook, sizeof(struct Book), 1, file)) {
                if (tempBook.id == idToSearch) {
                    found = 1;
                    break;
                }
            }
            fclose(file);

            // Send search result to client
            send(client_socket, &found, sizeof(int), 0);
            if (found) {
                send(client_socket, &tempBook, sizeof(struct Book), 0);
            }
            break;
        }
        case 2:
            // Client requested to exit
            printf("Client requested to exit.\n");
            break;
        default:
            printf("Invalid choice received from client.\n");
    }

    // Close client socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addr_len = sizeof(client_addr);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server is ready and listening on port %d...\nGo to new terminal and enter ./c2 to run as a client.\n", PORT);

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
            close(server_socket);
	    int choice;
            while(1)
	    {
		    handle_client(client_socket);
		    // Receive choice from client to determine if it wants to continue
        	    if (recv(client_socket, &choice, sizeof(int), 0) < 0 || choice == 2) {
        	        printf("Client requested to exit.\n");
               	        break;
		    }
	    }
	    close(client_socket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(client_socket);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}

