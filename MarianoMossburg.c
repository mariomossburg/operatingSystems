#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_READERS 12

// Struct for shared variable to store result 
struct shared_data {
    int value;
};

// Global shared variable 
struct shared_data *counter;

// Semaphores
sem_t write_access;
sem_t mutex;

int reader_count = 0;

// Error handling function
void handle_error(int err, const char *msg) {
    fprintf(stderr, "Error: %s --- %d\n", msg, err);
    exit(EXIT_FAILURE);
}

// Reader function
void *reader(void *arg) {
    int id = *(int *)arg;

    // Wait to read until writer is done
    sem_wait(&mutex);
    reader_count++;
    if (reader_count == 1) 
    {
        // First reader blocks writers
        sem_wait(&write_access); 
    }
    sem_post(&mutex);

    // Critical section: read the counter value
    printf("I'm reader%d, counter = %d\n", id, counter->value);

    sem_wait(&mutex);
    reader_count--;
    if (reader_count == 0) 
    {
        // Last reader unblocks writers
        sem_post(&write_access); 
    }
    sem_post(&mutex);

    return NULL;
}

// Writer function
void *writer(void *arg) {
    // Writer has exclusive access to write
    sem_wait(&write_access);
    
    // Set the counter value to 25000
    counter->value = 25000;
    printf("Writer Done!\n");

    sem_post(&write_access);
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("Executing... please wait...\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Number of Readers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n_readers = atoi(argv[1]);
    if (n_readers > MAX_READERS || n_readers < 1) {
        fprintf(stderr, "Error: Number of readers must be between 1 and %d.\n", MAX_READERS);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[MAX_READERS + 1];
    int ids[MAX_READERS + 1];

    counter = (struct shared_data *)malloc(sizeof(struct shared_data));
    if (counter == NULL) {
        handle_error(1, "Memory allocation failed");
    }
    counter->value = 0;

    // Initialize semaphores
    if (sem_init(&write_access, 0, 1) != 0 || sem_init(&mutex, 0, 1) != 0) {
        handle_error(1, "Semaphore initialization failed");
    }

    // Create and start the writer thread first
    pthread_create(&threads[0], NULL, writer, NULL);

    // Make sure the writer has finished before creating reader threads
    pthread_join(threads[0], NULL);

    // Create reader threads
    for (int i = 1; i <= n_readers; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, reader, &ids[i]);
    }

    // Wait for all reader threads to complete
    for (int i = 1; i <= n_readers; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    sem_destroy(&write_access);
    sem_destroy(&mutex);
    free(counter);

    return 0;
}