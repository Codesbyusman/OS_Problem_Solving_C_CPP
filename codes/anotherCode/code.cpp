#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

// for the threads
void *T1(void *args)
{
    int *value = (int *)args;
    *value = *value + 1;

    return (void *)value;
}

void *T2(void *args)
{
    int *value = (int *)args;
    *value = *value - 1;

    return (void *)value;
}

void *T3(void *args)
{
    int *value = (int *)args;
    cout << "\n T3 reads a as : " << *value << endl;

    return NULL;
}

void *T4(void *args)
{
    int *value = (int *)args;

    cout << "\n T4 reads b as : " << *value << endl;
    return NULL;
}

int main()
{
    void *ap;
    void *bp;

    int a = 7;
    int b = 2;

    // writing int the pipes for the p2
    int fd[2];

    pipe(fd);

    // for making the processes
    pid_t id1 = fork();

    if (id1 == 0)
    {
        //  ---- P1 -----
        pthread_t idt_1, idt_2;

        // thread creation
        pthread_create(&idt_1, NULL, T1, &a);
        pthread_create(&idt_2, NULL, T2, &b);

        // results from threads
        pthread_join(idt_1, &ap);
        pthread_join(idt_2, &bp);

        // updating the values
        a = *(int *)ap;
        b = *(int *)bp;

        // closing the reading end here
        close(fd[0]);

        // string of a and b

        // writing to the pipe
        write(fd[1], &a, sizeof(a));
        write(fd[1], &b, sizeof(b));

        exit(0);
    }
    else
    {

        wait(NULL);

        pid_t id2 = fork();

        if (id2 == 0)
        {
            // ---- P2 ----
            close(fd[1]); // close writing end
                          // for the result
            read(fd[0], &a, sizeof(a));
            read(fd[0], &b, sizeof(b));

            // creating the threads
            pthread_t idt_1, idt_2;

            // thread creation
            pthread_create(&idt_1, NULL, T3, &a);
            pthread_create(&idt_2, NULL, T4, &b);

            // results from threads
            pthread_join(idt_1, NULL);
            pthread_join(idt_2, NULL);
        }
    }

    return 0;
}