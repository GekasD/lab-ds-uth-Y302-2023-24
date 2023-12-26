#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define FILENAME_SIZE 20
#define FILEDATA_BUFF 100

char file_name[FILENAME_SIZE];
int fileDiscriptor;

/*
    Διαβάζει μία γραμμή εισόδου χρήστη (δηλαδή και κενά ' ')
*/
int readline(char str[], int n) {
	int ch, i = 0;

	while (isspace(ch = getchar()));

	while (ch != '\n' && ch != EOF) {
		if (i < n) {
			str[i++] = ch;
		}
		
		ch = getchar();
	}

	str[i] = '\0';
	
	return i;
}

void print_options() {
    printf(
        "\n(a) Create/Open file\n(b) Close file\n(c) Read file\n(d) Write in file\n(e) Exit\n\nEnter your choice: "
    );
}

/*
    Επιστρέφει 1 αν το αρχείο είναι ανοικτό, 0 αν δεν είναι 
*/
int is_file_open() {
    return fileDiscriptor != 0;
}

void open_create_file() {
    if (is_file_open()) {
        printf("You already have a file (%s) opened, close it first (b) and then open a new one\n", file_name);
        return;
    }

    printf("Enter the name of the file to create/open (max %d characters): ", FILENAME_SIZE);
    readline(file_name, FILENAME_SIZE);

    // Δημιουργία / Άνοιγμα ενός αρχείου με δικαιώματα ανάγνωσης και εγγραφής για τον χρήστη και κανένα δικαίωμα για τους άλλους.
    fileDiscriptor = open(file_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (fileDiscriptor == -1) {
        printf("Error opening file: %s", file_name);
        exit(EXIT_FAILURE);
    }

    printf("File creation/opening successful!\n");
}

void write_file() {
    if (!is_file_open()) {
        printf("There is no file open, open a file first (a)\n");
        return;
    }

    char buff[FILEDATA_BUFF];
    printf("Enter what you want to write (max %d characters):\n", FILEDATA_BUFF);
    readline(buff, FILEDATA_BUFF);
    
    if (write(fileDiscriptor, buff, strlen(buff)) == -1) {
        printf("Error writing to file: %s", file_name);
        exit(EXIT_FAILURE);
    }

    printf("\nSuccessfully written to file: %s\n", file_name);
}

void close_file() {
    if (!is_file_open()) {
        printf("There is no file open, open a file first (a)\n");
        return;
    }

    if (close(fileDiscriptor) == -1) {
        printf("Error closing file: %s", file_name);
        exit(EXIT_FAILURE);
    }

    fileDiscriptor = 0;

    printf("Successfully closed file: %s\n", file_name);
}

void read_file() {
    if (!is_file_open()) {
        printf("There is no file open, open a file first (a)\n");
        return;
    }

    char buff[FILEDATA_BUFF];
    if (read(fileDiscriptor, buff, FILEDATA_BUFF) == -1) {
        printf("Error reading file: %s", file_name);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buff);
}

int main() {
    char input;
    print_options();
    scanf("%s", &input);
    while (input != 'e') {
        switch (input) {
        case 'a':
            open_create_file();
            break;
        case 'b':
            close_file();
            break;
        case 'c':
            read_file();
            break;
        case 'd':
            write_file();
            break;
        default:
            printf("Invalid option (%c?), please try again\n", input);
            break;
        }

        print_options();
        scanf("%s", &input);
    }
    
    return EXIT_SUCCESS;
}