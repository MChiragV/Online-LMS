#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int book_id;
    
    // Create client socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    // Prompt user to enter book ID
    printf("Enter the book ID: ");
    scanf("%d", &book_id);
    
    // Send book ID to server
    if (send(client_socket, &book_id, sizeof(book_id), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    
    // Receive book title from server
    if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(buffer,"Book not found.")==0)
	    printf("%s\n",buffer);
    else
    {
	// Print the received book title
        printf("Book title: %s\n", buffer);
    }
    // Close client socket
    close(client_socket);
    
    return 0;
}

