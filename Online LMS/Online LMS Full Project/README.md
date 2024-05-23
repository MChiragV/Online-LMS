# Online Library Management System (OLMS)

This project implements an Online Library Management System (OLMS) in C programming language. It consists of a server component (`s2`), a client component (`c2`), a bridge component (`b`), and a main program (`main`) that acts as the entry point for the system.

## Components:

### 1. Server (`s2.c`):
- Created to serve client requests, and acts as the central hub for handling client requests.
- Listens for incoming connections from clients and manages communication.
- Implements functionalities such as searching for books and responding to client requests.
- Utilizes socket programming for communication with clients.

### 2. Client (`c2.c`):
- Handles client requests.
- Initiates communication with the server.
- Sends requests to the server, such as searching for books.
- Receives responses from the server and displays relevant information to the user.
- Utilizes socket programming for communication with the server.

### 3. Bridge (`b.c`):
- Accessed only by librarians. It is used by librarians to add, delete, modify and search for books.
- Serves as a critical component in the Online Library Management System (OLMS).
- Facilitates the interaction between librarians and the underlying data storage file.
- It uses books.txt file to update and keep track of the books.

### 4. Main Program (`main.c`):
- Entry point for the system.
- Provides options for users to identify themselves as librarians or clients.
- Allows librarians to create new accounts or access librarian functionalities.
- User authentication mechanism for librarians with the use of getpass() function for password-protection and prevention of unauthorized access.
- Invokes the bridge component (`b`) and appropriate client or server components based on user input.

## Working:

### 1. `s2.c` (Server Component):
- **Functionality**:
  - Acts as the server-side component of the OLMS.
  - Listens for incoming connections from clients and handles communication.
  - Implements functionalities such as searching for books and responding to client requests.
- **Working**:
  1. **Initialization**:
     - Creates a server socket using the `socket()` function.
     - Binds the socket to a specific port using `bind()`.
     - Listens for incoming connections using `listen()`.
  2. **Accepting Connections**:
     - Accepts incoming client connections using `accept()`.
     - Creates a new process or thread to handle each client connection.
  3. **Handling Client Requests**:
     - Receives client requests, such as searching for books, using `recv()`.
     - Processes the requests and retrieves relevant information from the file.
     - Sends the response back to the client using `send()`.

### 2. `c2.c` (Client Component):
- **Functionality**:
  - Acts as the client-side component of the OLMS.
  - Initiates communication with the server and sends requests.
  - Receives responses from the server and displays information to the user.
- **Working**:
  1. **Initialization**:
     - Creates a client socket using the `socket()` function.
     - Connects to the server using `connect()` and specifies the server's IP address and port.
  2. **Sending Requests**:
     - Sends user input or requests to the server using `send()`.
  3. **Receiving Responses**:
     - Receives responses from the server using `recv()`.
     - Parses the responses and displays relevant information to the user.

### 3.   `b.c` (Bridge):
- **Functionality**:
1.  **Interactive Interface**:
    
    -   Presents a user-friendly interface for users to perform various operations related to book management, including adding, deleting, modifying, and searching for books.
    -   Utilizes standard input and output for user interaction, allowing users to select desired actions via a menu system.
2.  **File Handling**:
    
    -   Manages book records stored in a file named `books.txt`.
    -   Utilizes file input/output operations to read and write book data to and from the file.
3.  **Concurrency Control**:
    
    -   Implements file locking mechanisms to ensure data integrity and prevent race conditions when multiple users concurrently access and modify book records.

- **Working**:

1.  **Initialization**:
    
    -   Opens the `books.txt` file in read-write mode, creating it if it doesn't exist.
    -   Initializes necessary variables and data structures.
2.  **Main Menu Loop**:
    
    -   Displays a menu prompting the user to choose from various book management operations.
    -   Continuously loops until the user selects the option to exit.
3.  **User Input Processing**:
    
    -   Reads the user's choice from standard input and executes the corresponding operation.
4.  **Book Management Operations**:
    
    -   **Add Book**:
        -   Prompts the user to input book details (ID, title, author).
        -   Checks if the book already exists in the system to avoid duplication.
        -   Appends the new book record to the end of the file.
    -   **Delete Book**:
        -   Prompts the user to input the ID of the book to be deleted.
        -   Searches for the book by ID and removes it from the file if found.
    -   **Modify Book**:
        -   Prompts the user to input the ID of the book to be modified.
        -   Searches for the book by ID and allows the user to update its title and author.
    -   **Search Book**:
        -   Prompts the user to input the ID of the book to search for.
        -   Displays the details of the book if found, including its ID, title, and author.
5.  **Concurrency Control**:
    
    -   Acquires an exclusive lock on the file before performing write operations to prevent concurrent modifications by multiple users.
    -   Releases the lock after completing each operation to allow other users to access the file.
6.  **Cleanup**:
    
    -   Closes the file descriptor and exits gracefully after the user chooses to exit the application.

### 4. `main.c` (Main Program):
- **Functionality**:
  - Acts as the entry point for the OLMS.
  - Provides options for users to identify themselves as librarians or clients.
  - Invokes the appropriate components based on user input.
- **Working**:
  1. **User Identification**:
     - Prompts users to choose between librarian or client functionalities.
  2. **Functionality Invocation**:
     - If the user identifies as a librarian, allows account creation or access to librarian functionalities.
     - If the user identifies as a client, invokes the client component to interact with the OLMS.
  3. **Component Interaction**:
     - Invokes the bridge component to facilitate communication between the client and server components.
     - Handles data forwarding and processing based on user actions.


## Usage:
1. Compile the project using a C compiler (e.g., GCC).
2. Run the executable generated from the compilation process.
3. Follow the prompts to choose between librarian and client functionalities.
4. Librarians can create new accounts or access librarian functionalities.
5. Clients can search for books and interact with the library system.
6. For exact order of execution:
- gcc s2.c -o s2
- gcc c2.c -o c2
- gcc b.c -o b
- gcc main.c
- ./a.out
7. users.txt file is created to hold information of all librarians, which is used for user authentication.
8. books.txt file is created to hold attributes of a book.

## Development Environment:
- This project was developed and tested in a Unix-like environment (Linux).
- It utilizes socket programming for communication between components.
- File handling is used for storing user credentials and book details.


---
For detailed documentation, refer to individual source code files (`s2.c`, `c2.c`, `b.c`, `main.c`).
