#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SLOTS_NUM 10
#define CUSTOMER_NUM 15
#define MAX_WASH_TIME_SECONDS 5

sem_t slot_sem;

/*
 *  Επιστρέφει τυχαία τιμή από 1 μέχρι MAX_WASH_TIME_SECONDS
*/
int random_num() {
    return rand() % MAX_WASH_TIME_SECONDS + 1;
}

/*
 *  Σταματάει την εκτέλεση του προγράμματος για random_num()
*/
void simulate_wash() {
    sleep(random_num());
}

void *customer_handler(void *arg) {
    int customer_id = *(int *)arg;
    free(arg);

    printf("Customer (%d) has arrived\n", customer_id);

    sem_wait(&slot_sem);

    printf("Customer (%d) started washing their car\n", customer_id);
    simulate_wash();
    printf("Customer (%d) finished washing their car\n", customer_id);

    sem_post(&slot_sem);
}

int main() {
    pthread_t customer_threads[CUSTOMER_NUM];

    sem_init(&slot_sem, 0, SLOTS_NUM);

    for (int i = 0; i < CUSTOMER_NUM; i++) {
        int *thread_index = malloc(sizeof(int));
		*thread_index = i;

        if (pthread_create(&customer_threads[i], NULL, customer_handler, thread_index) != 0) {
            perror("pthread_create");
        }
    }
    
    for (int i = 0; i < CUSTOMER_NUM; i++) {
        if (pthread_join(customer_threads[i], NULL) != 0) {
            perror("pthread_join");
        }
    }

    printf("All %d customers have finished washing their cars\n", CUSTOMER_NUM);

    sem_destroy(&slot_sem);

    return 0;
}