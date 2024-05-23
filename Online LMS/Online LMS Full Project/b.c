#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_NAME_LENGTH 100
#define MAX_BOOKS 100

// Structure for book details
struct Book {
    int id;
    char title[MAX_NAME_LENGTH];
    char author[MAX_NAME_LENGTH];
};

// Function prototypes
void addBook(int fd);
void deleteBook(int fd);
void modifyBook(int fd);
int searchBook(int fd, int idToSearch);
int lockFile(int fd);
int unlockFile(int fd);

int main() {
    int choice;
    int fd;

    printf("Hello! Welcome to Chirag's OLMS.\n");

    // Open file for reading and writing
    fd = open("books.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }

    do {
        printf("Do you want to:\n1. Add Book\n2. Delete Book\n3. Modify Book\n4. Search Book\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
		lockFile(fd);
                addBook(fd);
		unlockFile(fd);
                break;
            case 2:
		lockFile(fd);
                deleteBook(fd);
		unlockFile(fd);
                break;
            case 3:
		lockFile(fd);
                modifyBook(fd);
		unlockFile(fd);
                break;
            case 4: 
		lockFile(fd);
                int idToSearch;
                printf("Enter ID of book to search: ");
                scanf("%d", &idToSearch);
                searchBook(fd, idToSearch);
		unlockFile(fd);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    close(fd);
    return 0;
}

void addBook(int fd) {
    struct Book newBook;
    printf("Enter book ID: ");
    scanf("%d", &newBook.id);
    if (searchBook(fd, newBook.id)) {
        printf("Book already exists, please try again. If you want to modify the book, choose option 3.\n");
        return;
    }

    printf("Enter book title: ");
    getchar(); // Clear the newline character left in the buffer
    fgets(newBook.title, sizeof(newBook.title), stdin);
    // Remove trailing newline character
    newBook.title[strcspn(newBook.title, "\n")] = '\0';

    printf("Enter book author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    // Remove trailing newline character
    newBook.author[strcspn(newBook.author, "\n")] = '\0';

    // Move file pointer to end for appending
    if (lseek(fd, 0, SEEK_END) == -1) {
        perror("Error seeking file");
        return;
    }

    // Write book details to file
    if (write(fd, &newBook, sizeof(struct Book)) == -1) {
        perror("Error writing to file");
        return;
    }

    printf("Book has been successfully added.\n");

}


void deleteBook(int fd) {

    int idToDelete;
    printf("Enter ID of book to delete: ");
    scanf("%d", &idToDelete);

    struct Book tempBook;
    off_t pos = lseek(fd, 0, SEEK_SET);
    if (pos == -1) {
        perror("Error seeking file");
        return;
    }

    int found = 0;
    off_t deletePos = -1; // Position of the book to delete
    while (read(fd, &tempBook, sizeof(struct Book)) == sizeof(struct Book)) {
        if (tempBook.id == idToDelete) {
            found = 1;
            deletePos = lseek(fd, -sizeof(struct Book), SEEK_CUR);
            if (deletePos == -1) {
                perror("Error seeking file");
                return;
            }
            break;
        }
    }

    if (!found) {
        printf("Book does not exist. If you want to add it, choose option 1.\n");
        return;
    }

    // Shift remaining records to cover the deleted one
    off_t readPos = lseek(fd, deletePos + sizeof(struct Book), SEEK_SET);
    if (readPos == -1) {
        perror("Error seeking file");
        return;
    }

    while (read(fd, &tempBook, sizeof(struct Book)) == sizeof(struct Book)) {
        off_t writePos = lseek(fd, deletePos, SEEK_SET);
        if (writePos == -1) {
            perror("Error seeking file");
            return;
        }
        if (write(fd, &tempBook, sizeof(struct Book)) == -1) {
            perror("Error writing to file");
            return;
        }
        deletePos = lseek(fd, 0, SEEK_CUR); // Update the position to write next record
        if (deletePos == -1) {
            perror("Error seeking file");
            return;
        }
    }

    // Truncate file to remove extra data at the end
    if (ftruncate(fd, deletePos) == -1) {
        perror("Error truncating file");
        return;
    }

    printf("Book has been successfully deleted.\n");

}


void modifyBook(int fd) {

    int idToModify;
    printf("Enter ID of book to modify: ");
    scanf("%d", &idToModify);

    if (!searchBook(fd, idToModify)) {
        printf("Book does not exist. If you want to add the book, choose option 1.\n");
        return;
    }

    // Search for the book with given ID
    struct Book tempBook;
    off_t pos = lseek(fd, 0, SEEK_SET);
    if (pos == -1) {
        perror("Error seeking file");
        return;
    }

    while (read(fd, &tempBook, sizeof(struct Book)) == sizeof(struct Book)) {
        if (tempBook.id == idToModify) {
            // Modify the book
            printf("Enter new title: ");
            getchar(); // Clear the newline character left in the buffer
            fgets(tempBook.title, sizeof(tempBook.title), stdin);
            // Remove trailing newline character
            tempBook.title[strcspn(tempBook.title, "\n")] = '\0';

            printf("Enter new author: ");
            fgets(tempBook.author, sizeof(tempBook.author), stdin);
            // Remove trailing newline character
            tempBook.author[strcspn(tempBook.author, "\n")] = '\0';

            // Write modified book details back to file
            if (lseek(fd, -sizeof(struct Book), SEEK_CUR) == -1) {
                perror("Error seeking file");
                return;
            }
            if (write(fd, &tempBook, sizeof(struct Book)) == -1) {
                perror("Error writing to file");
                return;
            }
            break;
        }
    }

    printf("Book has been successfully modified.\n");

}


int searchBook(int fd, int idToSearch) {

    struct Book tempBook;
    off_t pos = lseek(fd, 0, SEEK_SET);
    if (pos == -1) {
        perror("Error seeking file");
        return 0;
    }

    int found = 0;
    while (read(fd, &tempBook, sizeof(struct Book)) == sizeof(struct Book)) {
        if (tempBook.id == idToSearch) {
            found = 1;
            printf("Book found:\n");
            printf("ID: %d\nTitle: %s\nAuthor: %s\n", tempBook.id, tempBook.title, tempBook.author);
            break;
        }
    }

    if (!found) {
        printf("Book not found.\n");
        return 0;
    }

    return 1;
}

int lockFile(int fd) {
    struct flock lock;
    lock.l_type = F_WRLCK; // Exclusive write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error locking file");
        return -1;
    }
    return 0;
}

int unlockFile(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error unlocking file");
        return -1;
    }
    return 0;
}

