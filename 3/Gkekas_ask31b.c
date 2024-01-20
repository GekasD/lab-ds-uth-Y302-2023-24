#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Ο τρόπος με τον οποίο γράφτηκε η πρώτη άσκηση κάνει την αλλαγή σε δυναμικό αριθμό thread θέμα αλλαγής μίας μεταβλητής!
*/

int mail_queue = 0;

void *addmailqueue(void *arg) {
	int numMails = *(int *)arg;

	for (int i = 0; i < numMails; i++) {
		mail_queue++;
	}
}

int main(int argc, char const *argv[]) {
	// Διαβάζουμε τον x αριθμό mail απο τα params της main()
	if (argc != 3) {
		printf("Usage: %s <mail_count> <thread_count>, both values need to be numbers.\n", argv[0]);
		return EXIT_FAILURE;
	}
	int mail_count = atoi(argv[1]);
    int thread_count = atoi(argv[2]);

	// Αρχικό μήνυμα
	printf("Initial mail queue: %d\n", mail_queue);

	// Δημιουργία thread_count αριθμό threads
	pthread_t threads[thread_count];
	for (int i = 0; i < thread_count; i++) {
		if (pthread_create(&threads[i], NULL, addmailqueue, (void *)&mail_count) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	// Περιμένουμε τα thread να τελειώσουν την εκτέλεση τους
	for (int i = 0; i < thread_count; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	// Τελικό μήνυμα
	printf("All %d threads finished running!\n", thread_count);
	printf("Final mail queue: %d\n", mail_queue);

	return EXIT_SUCCESS;
}