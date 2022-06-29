#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

// creating the semaphore arrays for the chopsticks
sem_t chopSticks[5];

sem_t statusSaver;

bool status[5] = {false, false, false, false, false};

// the philosphers
void *Philosphers(void *args)
{

    int philospherId = *((int *)args);

    while (1)
    {
        sem_wait(&chopSticks[philospherId % 5]);
        sem_wait(&chopSticks[philospherId + 1 % 5]);

        // updating status
        sem_wait(&statusSaver);

        // wait till both are not free
        while (!(status[philospherId % 5] == false && status[philospherId + 1 % 5] == false))
            ;

        status[philospherId % 5] = true;
        status[philospherId + 1 % 5] = true;
        sem_post(&statusSaver);

        sleep(3);
        printf("I am philospher %d ::::: eating \n\n", philospherId);

        // updating status
        sem_wait(&statusSaver);
        status[philospherId % 5] = false;
        status[philospherId + 1 % 5] = false;
        sem_post(&statusSaver);

        sem_post(&chopSticks[philospherId % 5]);
        sem_post(&chopSticks[philospherId + 1 % 5]);

        sleep(3);
    }
}

int main()
{
    pthread_t philosphers[5]; // the five philosphers thead ids

    // initalizing the semaphores
    for (int i = 0; i < 5; i++)
    {
        sem_init(&chopSticks[i], 0, 1);
    }

    sem_init(&statusSaver, 0, 1);

    // creation of threads
    for (int i = 0; i < 5; i++)
    {
        printf("\n\n I am philospher %d ::::: creation \n\n", i);
        pthread_create(&philosphers[i], NULL, Philosphers, &i);
        sleep(1);
    }

      // joining of threads
    for (int i = 0; i < 5; i++)
    {
        pthread_join(philosphers[i], NULL);
    }

    // destroying the semaphores
    for (int i = 0; i < 5; i++)
    {
        sem_destroy(&chopSticks[i]);
    }

    sem_destroy(&statusSaver);

    return 0;
}
