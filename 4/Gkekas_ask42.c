#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_COOK_TIME_SECONDS 10
#define STORAGE_TIME_SECONDS 5

int participant_count;

sem_t waiting_room_access;
sem_t storage_room_access;

void action_msg(int id, char *msg) {
    printf("Participant [%d]: %s\n", id, msg);
}

int cooking_time() {
    return rand() % MAX_COOK_TIME_SECONDS + 1;
}

void *participant_handler(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while (1) // Ξεκινάει η περιπέτεια..
    {
        action_msg(id, "Preparing their recipe");
        sleep(cooking_time()); // Προσομοίωση μαγειρέματος

        if (sem_trywait(&waiting_room_access) == 0) // Αν η αίθουσα αναμονής είναι διαθέσιμη...
        { 
            // Μπαίνουμε στην αίθουσα αναμονής...

            action_msg(id, "Entered the waiting room");
        
            sem_wait(&storage_room_access); // Αναμονή διαθεσημότητας αποθήκης

            // Μπαίνουμε στην αποθήκη

            sem_post(&waiting_room_access); // Αφήνουμε τον επόμενο να μπεί στην αίθουσα αναμονής

            action_msg(id, "Entered the storage room, and started collecting ingredients");

            sleep(STORAGE_TIME_SECONDS); // Προσομοίωση αναζήτησης συστατικών στην αποθήκη

            sem_post(&storage_room_access); // Αφήνουμε τον επόμενο να μπεί στην αποθήκη

        } else // Αν κάποιος κάθεται ήδη στην καρέκλα της αίθουσας αναμονής...
        { 
            action_msg(id, "Found the waiting room occupied, returning to kitchen empty handed :(");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <Participant Count> (Number)\n", argv[0]);
        return EXIT_FAILURE;
    }
    participant_count = atoi(argv[1]);

    // Αρχικοποίηση σημαφόρων & mutex
    sem_init(&waiting_room_access, 0, 1);
    sem_init(&storage_room_access, 0, 1);

    // Threads διαγωνιζόμενων
    pthread_t participant_threads[participant_count];
    for (int i = 0; i < participant_count; i++) {
        int *thread_index = malloc(sizeof(int));
		*thread_index = i;

        if (pthread_create(&participant_threads[i], NULL, participant_handler, thread_index) != 0) {
            perror("pthread_create (articipant)");
        }
    }

    // Join threads
    for (int i = 0; i < participant_count; i++) {
        if (pthread_join(participant_threads[i], NULL) != 0) {
            perror("pthread_join (articipant)");
        }
    }

    /*
        Το πρόγραμμα δεν πρόκειται να τερματιστεί ποτέ, δεν υπάρχει λόγος να κάνουμε κάτι άλλο εδώ...
    */
}