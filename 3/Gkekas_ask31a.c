#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_COUNT 4
int mail_queue = 0;

void *addmailqueue(void *arg) {
	int numMails = *(int *)arg;

	for (int i = 0; i < numMails; i++) {
		mail_queue++;
	}
}

int main(int argc, char const *argv[]) {
	// Διαβάζουμε τον x αριθμό mail απο τα params της main()
	if (argc != 2) {
		printf("Usage: %s <number>, where number = x amount of mails.\n", argv[0]);
		return EXIT_FAILURE;
	}
	int mail_count = atoi(argv[1]);

	// Αρχικό μήνυμα
	printf("Initial mail queue: %d\n", mail_queue);

	// Δημιουργία THREAD_COUNT αριθμό threads
	pthread_t threads[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; i++) {
		if (pthread_create(&threads[i], NULL, addmailqueue, (void *)&mail_count) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	// Περιμένουμε τα thread να τελειώσουν την εκτέλεση τους
	for (int i = 0; i < THREAD_COUNT; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	// Τελικό μήνυμα
	printf("All %d threads finished running!\n", THREAD_COUNT);
	printf("Final mail queue: %d\n", mail_queue);

	return EXIT_SUCCESS;
}

/**
 * Παρατήρηση:
 * 
 * Η τελική ουρά mail έχει μέγεθος τον αρχικό αριθμό που δώθηκε * τον αριθμό των thread (THREAD_COUNT), δηλαδή 4,
 * αυτό όμως ισχύει μόνο σε μικρότερους αριθμούς, όταν η παράμετρος mail_count (δηλαδή το πρώτο arg της main) ξεπερνάει κάποιο αριθμό 
 * (στον server μου είναι γύρο στα 5000) τότε το αποτέλεσμα γίνεται απρόβλεπτο, αυτό γίνεται γιατί συναντάμε race condition.
 * 
 * Δηλαδή πολλαπλά thread γράφουν στην μεταβλητή mail_queue ταυτόχρονα, και τα 2 thread διαβάζουν την μεταβλητή την ίδια στιγμή και την αυξάνουν
 * την ίδια στιγμή με αποτέλεσμα μία η περισσότερες από τις αυξήσεις να χαθούν.
 * 
 * Aυτό μπορεί να λυθεί με την χρήση μηχανισμών συγχρονισμού, όπως mutex locks, με την χρήση τους μπορούμε να είμαστε σίγουροι ότι μόνο 1 thread μπόρει να
 * διαβάσει και να τροποποιήσει την μεταβλήτη κάθε φορά.
 * 
 * - Γκέκας Δημήτριος
*/