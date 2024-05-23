#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
int main() {
    char c;
    printf("Welcome to Chirag's OLMS. Are you a user(U) or a librarian(L)?\n");
    scanf("%c",&c);
    switch( c){
	case 'U':
	    printf("Hello user.\n");
	    break;
	case 'L':
	    printf("Hello librarian.\n");
	    break;
	default:
	    printf("Who are you again?\n");
    }
    char *password;
    password = getpass("Enter password: ");
    if (password == NULL) {
        printf("Error reading password.\n");
        return 1;
    }

    printf("You entered: %s\n", password);

    return 0;
}

