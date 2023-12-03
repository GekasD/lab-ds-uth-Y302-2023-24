#include <stdio.h>

#define N 4
#define MAX_TITLE_SIZE 25

struct movie {
    int code;
    char title[MAX_TITLE_SIZE];
    int year;
};

int size = N;
int pointer = 0;
struct movie *movies;

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

/*
    Τυπώνει τις πληροφορίες μίας ταινίας σε μορφοποιημένη μορφή
*/
void printmovie(struct movie m) {
    printf("%s (%d) [%d]", m.title, m.year, m.code);
}

/*
    Τυπώνει τις επιλογές που είναι διαθέσιμες στον χρήστη
*/
void print_options() {
    printf(
        "\n(i) Insert\n(s) Search\n(u) Update\n(e) Erase\n(p) Print\n(q) Quit\n\nEnter operation code: "
    );
}

/*
    Σειριακή αναζήτηση του πίνακα movies[] και επιστροφή θέσης της ταινίας,
    Αν δεν βρεθεί ταινία με τον κωδικό που παρέχεται, τότε επιστρέφει -1.
*/
int find(int code) {
    int index = -1;
    for (int i = 0; i < pointer; i++) {
        if (movies[i].code == code) {
            index = i;
            break;
        }
    }

    return index;
}

/*
    Εισάγει μία νέα ταινία στον πίνακα movies[] διαδραστικά
*/
void insert() {
    struct movie m;

    printf("Provide a code for the movie: ");
    scanf("%d", &m.code);
    if (find(m.code) != -1) {
        printf("ERROR: A movie with the code: %d already exists!\n", m.code);
        return;
    };

    printf("Provide the movie's title: ");
    readline(m.title, MAX_TITLE_SIZE);

    printf("Provide the movie's year: ");
    scanf("%d", &m.year);

    if (pointer == size) {
        size += N;
        movies = (struct movie*)realloc(movies, size * sizeof(struct movie));

        if (movies == NULL) {
            printf("ERROR: Failed to allocate %d size to the movies array!\n", N);
            exit(1);
        }
    }

    movies[pointer] = m;

    pointer++;
}

/*
    Αναζητά και τυπώνει τις πληροφορίες μίας ταινίας διαδραστικά
*/
void search() {
    if (pointer < 1) {
        printf("ERROR: There are no movies to search for, please insert some movies first (i)\n");
        return;
    }

    int code;
    printf("Insert a movie's code to search: ");
    scanf("%d", &code);

    int index = find(code);
    if (index != -1) {
        printmovie(movies[index]);
        printf("\n");
    } else {
        printf("ERROR: No movie found that matches the code: %d\n", code);
    }
}

/*
    Ενημερώνει την χρονολογία μίας ταινίας του πίνακα movies[] διαδραστικά
*/
void update() {
    if (pointer < 1) {
        printf("ERROR: There are no movies to update, please insert some movies first (i)\n");
        return;
    }

    int code;
    printf("Insert a movie's code to update it's details: ");
    scanf("%d", &code);

    int index = find(code);
    if (index == -1) {
        printf("ERROR: No movie found with code: %d\n", code);
        return;
    }

    printf("Enter a new year for movie ");
    printmovie(movies[index]);
    printf(": ");
    scanf("%d", &movies[index].year);
}

/*
    Διαγράφει μία ταινία απο τον πίνακα movies[] διαδραστικά
*/
void erase() {
    if (pointer < 1) {
        printf("ERROR: There are no movies to erase, please insert some movies first (i)\n");
        return;
    }

    int code;
    printf("Insert the code of the movie you want to erase: ");
    scanf("%d", &code);

    int index = find(code);
    if (index == -1) {
        printf("ERROR: No movie found with code: %d\n", code);
        return;
    }

    printf("Removing movie: ");
    printmovie(movies[index]);
    printf("\n");

    for (int i = index; i < (pointer - 1); i++) {
        movies[i] = movies[i + 1];
    }

    pointer--;

    if (pointer == size - N && size > N) {
        size -= N;
        movies = (struct movie *)realloc(movies, size * sizeof(struct movie));

        if (movies == NULL) {
            printf("ERROR: There was an error deallocating memory\n");
            exit(1);
        }
    }
}

/*
    Τυπώνει μία ωραία μορφοποιημένη λίστα με κάθε ταινία στον πίνακα movies[]
*/
void print() {
    printf("Movie count: %d\nArray size: %d\n", pointer, size);

    if (pointer > 0) { 
        printf("Format: Title (Year) [Code]\n");
    }

    // Παράδειγμα: "2. Titanic (1997) [729]"
    for (int i = 0; i < pointer; i++) {
        printf("%d. ", i + 1);
        printmovie(movies[i]);
        printf("\n");
    }
}

int main() {
    movies = (struct movie*)malloc(N * sizeof(struct movie));

    if (movies == NULL) {
        printf("ERROR: Failed to allocate %d size to the movies array!\n", N);
        return 1;
    }

    printf("\n-=[ Welcome to the movies administration system! ]=-\n");

    char input;
    print_options();
    scanf("%s", &input);

    while (input != 'q') {
        printf("\n");

        switch (input) {
        case 'i':
            insert();
            break;
        case 's':
            search();
            break;
        case 'u':
            update();
            break;
        case 'e':
            erase();
            break;
        case 'p':
            print();
            break;
        default:
            printf("ERROR: invalid option (%c?), please try again\n", input);
            break;
        }

        print_options();
        scanf("%s", &input);
    }
    
    printf("Exiting...");

    return 0;
}