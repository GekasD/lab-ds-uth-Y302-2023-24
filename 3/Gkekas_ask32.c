#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_MATRIX_DIMENSION_TO_PRINT 14

int **A, **B, **C; // Πίνακες (Matrix)
int N; // Διάσταση τετραγωνικού πίνακα ακεραίων
int T; // Αριθμός thread

/*
 *	Δεσμεύει δυναμικά μνήμη για έναν τετραγωνικό πίνακα NxN.
*/
int **allocate_array_memory() {
	// Γραμμές
	int **matrix = (int **)malloc(N * sizeof(int *));
    if (matrix == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

	// Στήλες
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(N * sizeof(int));
        if (matrix[i] == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

/*
 * Επιστρέφει τα δευτερόλεπτα που έχουν περάσει μεταξύ 2 timeval structs (σε double)
*/
double elapsed_time(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_usec - start.tv_usec) / 1e6; // 1e6 = 1 * 10 στην δύναμη του 6, δηλαδή 1 εκατομμύριο
}

/*
 *	Επιστρέφει τυχαία τιμή στο διάστημα [0, 10]
*/
int random_num() {
	return rand() % 10;
}

/*
 *	Γεμίζει με τυχαίες τιμές στο διάστημα [0,10] έναν τετραγωνικό πίνακα διάστασης Ν
*/
void fill_array(int **arr) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			arr[i][j] = random_num();
		}
	}
}

/*
 *	Εκτυπώνει σε μορφή πίνακα έναν πίνακα NxN, αν η διάσταση (N) είναι μικρότερη η ίση με MAX_MATRIX_DIMENSION_TO_PRINT
*/
void print_array(int **arr) {
	if (N > MAX_MATRIX_DIMENSION_TO_PRINT) {
		printf("Table dimension (%d) is larger than the maximum printable dimension (%d)\n", N, MAX_MATRIX_DIMENSION_TO_PRINT);
		return;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			/*
			 *	Χωρίζουμε με κόμμα ',' αν δεν είναι ο τελευταίος αριθμός,
			 *	ΠΧ: 9, 1, 2, 7, 0, 9, 3, [6] <- (το 6 δεν έχει κόμμα)
			*/
			printf("%d%c ", arr[i][j], j == N - 1 ? ' ' : ','); 
		}
		printf("\n");
	}
}

/*
 *	Βρίσκει το κομμάτι του πίνακα C στο οποίο πρέπει να δουλέψει με την χρήση
 *	της θέσης (i) του thread στον πίνακα των threads και στην συνέχεια,
 *	υπολογίζει την διαφορά των πινάκων A[x][y] - B[x][y] στο κομμάτι που
 *	αντιστοιχεί στο συγκεκριμένο thread, τα κομμάτια είναι *πάντα* ίσα
 *	αφου ελέγχουμε (N % T != 0) στην main().
*/
void *worker(void *arg) {
	int thread_index = *(int *)arg;
	free(arg); // Απελευθερώνουμε την μνήμη από πριν

	int chunk_size = N / T;
    int start_row = thread_index * chunk_size;
    int end_row = start_row + chunk_size - 1;

	for (int i = start_row; i <= end_row; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = A[i][j] - B[i][j];
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <N (Matrix dimension)> <T (Thread count)>\n", argv[0]);
		return EXIT_FAILURE;
	}

	N = atoi(argv[1]);
	T = atoi(argv[2]);

	if (N % T != 0) {
		printf("T (%d) must be a divisor of N (%d)\n", T, N);
		return EXIT_FAILURE;
	}

	// Δέσμευση μνήμης και αρχικοποίηση των πινάκων A, B, C
	A = allocate_array_memory();
	B = allocate_array_memory();
	C = allocate_array_memory();

	// Αρχικοποίηση πινάκων A, B
	fill_array(A);
	fill_array(B);

	// Εκτύπωση των πινάκων A, B
	printf("Matrix 1:\n");
	print_array(A);
	printf("Matrix 2:\n");
	print_array(B);

	/*
	*	Πριν ξεκινήσουμε να κάνουμε τα thread για τον πίνακα C, κάνουμε δύο timeval structs
	*	για να αποθηκεύσουμε την ώρα έναρξης και ολοκλήρωσης των thread,
	*	ξεκινάμε επίσης δηλώνοντας την ώρα έναρξης εδώ (με την χρήση gettimeofday()).
	*/
	struct timeval start, end;
	gettimeofday(&start, NULL);

	// Δημιουργία και τρέξιμο των threads μας.
	pthread_t threads[T];
	for (int i = 0; i < T; i++) {
		/*
		 *	Δεν μπορούμε να περάσουμε κατευθείαν τον pointer της θέσης του thread (i)
		 *	γιατί θα συναντήσουμε race condition (εξηγείται σε σχόλιο στο τέλος του Gkekas_ask31a.c)
		 *	οπότε τον αποθηκεύουμε προσωρινά σε άλλο μέρος στη μνήμη,
		 *	και μέσα στην συνάρτηση worker() την απελευθερώνουμε.
		 * 
		*/
		int *thread_index = malloc(sizeof(int));
		*thread_index = i;

		if (pthread_create(&threads[i], NULL, worker, (void *)thread_index) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	// Περιμένουμε τα thread να τελειώσουν την εκτέλεση τους
	for (int i = 0; i < T; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	// Ώρα ολοκλήρωσης thread
	gettimeofday(&end, NULL);

	// Εκτύπωση του πίνακα C
	printf("Matrix 3:\n");
	print_array(C);

	// Εκτύπωση χρώνου εκτέλεσης των worker threads
	printf("Elapsed time: %f seconds\n", elapsed_time(start, end));

	// Ελευθέρωση μνήμης των πινάκων
	for (int i = 0; i < N; i++) {
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);

	return EXIT_SUCCESS;
}