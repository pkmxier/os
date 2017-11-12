#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

int are_cards_equal(int first, int second) {
    return first % 13 == second || first == second % 13;
}

const int DECK_SIZE = 52;

int count = 0;
void *potok(void *arg);

pthread_mutex_t mutex;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Usage: ./a.out <number of rounds> <number of threads>\n");
        return 1;
    }

    int rounds_per_thread = atoi(argv[1]) / atoi(argv[2]);

    pthread_t tid[atoi(argv[2])];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < atoi(argv[2]); ++i) {
        if (pthread_create(&tid[i], NULL, potok, &rounds_per_thread) != 0) {
            perror("Can`t create thread\n");
            return 2;
        }
    }

    for (int i = 0; i < atoi(argv[2]); ++i) {
        if (pthread_join(tid[i], NULL) != 0) {
            perror("Can`t join threads");
            return 3;
        }
    }

    pthread_mutex_destroy(&mutex);

    printf("%lf", (double)count / atoi(argv[1]));
}

void *potok(void * arg) {
    int deck[DECK_SIZE];
    for (int k = 0; k < *((int *)arg); ++k) {
        for (int i = 0; i < DECK_SIZE; ++i) {
            deck[i] = i + 1;
        }

        srand(clock());

        for (int i = DECK_SIZE - 1; i >= 0; --i) {
            int j = (int)rand() % (i + 1);
            int tmp = deck[i];
            deck[i] = deck[j];
            deck[j] = tmp;
        }

        if (are_cards_equal(deck[0], deck[1])) {
            pthread_mutex_lock(&mutex);
            ++count;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(0);
}
