#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_BOOK_TITLE_LEN 100

// Define the structure for storing book information
typedef struct {
    int id;
    char title[MAX_BOOK_TITLE_LEN];
    // Add other book information here if needed
} Book;

// Function to handle client requests
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg); // Free the memory allocated for the argument
    
    char buffer[1024];
    int book_id;
    
    // Receive book ID from the client
    if (recv(client_socket, &book_id, sizeof(book_id), 0) == -1) {
        perror("recv");
        close(client_socket);
        return NULL;
    }
    
    // Open the file to read books
    FILE *file = fopen("books.txt", "r");
    if (file == NULL) {
        perror("fopen");
        close(client_socket);
        return NULL;
    }
    
    char rb[1000];
    // Find the requested book in the file
    ssize_t read;
    size_t len=0;
    char* line=NULL;
    Book requested_book;
    int found = 0;
    while (read=getline(&line,&len, file)!=-1) {
        char* token;
        token=strtok(line,",");
        requested_book.id=atoi(token);
        token=strtok(NULL,",");
        strcpy(requested_book.title,token);
        if (requested_book.id == book_id) {
            found = 1;
            break;
        }
    }
    fclose(file);
    
    // Send the book title to the client
    if (found) {
        if (send(client_socket, requested_book.title, strlen(requested_book.title), 0) == -1) {
            perror("send");
        }
    } else {
        char *not_found_msg = "Book not found.";
        if (send(client_socket, not_found_msg, strlen(not_found_msg), 0) == -1) {
            perror("send");
        }
    }
    
    // Close the client socket
    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    
    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Bind server socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port 8080...\n");
    
    // Accept incoming connections and create threads to handle them
    while (1) {
        client_addr_len = sizeof(client_addr);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("accept");
            continue;
        }
        
        // Allocate memory for client socket descriptor to pass it to the thread
        int *client_socket_ptr = (int *)malloc(sizeof(int));
        *client_socket_ptr = client_socket;
        
        // Create thread to handle client request
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_socket_ptr) != 0) {
            perror("pthread_create");
            free(client_socket_ptr);
            continue;
        }
        
        // Detach the thread
        pthread_detach(tid);
    }
    
    // Close server socket
    close(server_socket);
    
    return 0;
}

