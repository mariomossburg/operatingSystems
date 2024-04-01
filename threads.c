//Terminal Command
//   gcc threads.c  -o threads
//   ./threads



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_UPDATES 2000000
#define BONUS_THRESHOLD 100
#define THREAD_COUNT 2

/* Struct for shared variable to store result */
struct shared_data {
    int value;
};

/* Struct to encapsulate thread related variables */
struct thread_vars {
    int bonus_ok;
    int bonus_times;
    int updates;
};

/* Global shared variable */
struct shared_data *counter;
/* Mutex lock */
pthread_mutex_t mutex;

/* Error handling function */
void handle_error(int err, const char *msg) {
    fprintf(stderr, "Error: %s --- %d\n", msg, err);
    exit(EXIT_FAILURE);
}

/* Thread one function */
void *thread_one(void *arg) {
    struct thread_vars thread_vars = {0}; // Initialize thread-specific variables

    // Remainder section
    while (thread_vars.updates < MAX_UPDATES) {
        // Entry section
        if (pthread_mutex_lock(&mutex) == 0) {
            // Critical section
            thread_vars.bonus_ok = MAX_UPDATES - thread_vars.updates >= BONUS_THRESHOLD && counter->value % BONUS_THRESHOLD == 0;
            if (thread_vars.bonus_ok) {
                counter->value += BONUS_THRESHOLD;
            } else {
                counter->value += 1;
            }
            // Exit section
            pthread_mutex_unlock(&mutex);
        }

        // Remainder section
        if (thread_vars.bonus_ok) {
            thread_vars.bonus_times += 1;
            thread_vars.updates += BONUS_THRESHOLD;
        } else
            thread_vars.updates += 1;
    }

    // Remainder section
    printf("Thread one completed %d updates with %d bonus times. Final counter value = %d\n",
           thread_vars.updates, thread_vars.bonus_times, counter->value);

    return NULL;
}

/* Thread two function */
void *thread_two(void *arg) {
    struct thread_vars thread_vars = {0}; // Initialize thread-specific variables

    // Remainder section
    while (thread_vars.updates < MAX_UPDATES) {
        // Entry section
        if (pthread_mutex_lock(&mutex) == 0) {
            // Critical section
            counter->value += 1;
            // Exit section
            pthread_mutex_unlock(&mutex);
        }
        thread_vars.updates += 1;
    }

    // Remainder section
    printf("Thread two completed %d updates. Final counter value = %d\n",
           thread_vars.updates, counter->value);

    return NULL;
}

int main() {
    pthread_t tid[THREAD_COUNT];
    int rc;

    /* Allocate memory for shared data */
    counter = (struct shared_data *)malloc(sizeof(struct shared_data));
    if (counter == NULL)
        handle_error(1, "Memory allocation failed");

    counter->value = 0;

    /* Initialize mutex lock */
    if ((rc = pthread_mutex_init(&mutex, NULL)) != 0)
        handle_error(rc, "Mutex initialization failed");

    /* Create threads */
    for (int i = 0; i < THREAD_COUNT; ++i) {
        if ((rc = pthread_create(&tid[i], NULL, i == 0 ? thread_one : thread_two, NULL)) != 0)
            handle_error(rc, i == 0 ? "Thread one creation failed" : "Thread two creation failed");
    }

    /* Wait for threads to finish */
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(tid[i], NULL);
    }

    /* Final result print out */
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("From parent counter = %d\n", counter->value);
        pthread_mutex_unlock(&mutex);
    }

    /* Deallocate memory, clean up */
    pthread_mutex_destroy(&mutex);
    free(counter);
    pthread_exit(NULL);

    return 0;
}
