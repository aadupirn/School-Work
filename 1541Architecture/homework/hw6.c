#include <stdio.h>
#include <pthread.h>
#include <time.h>
//N
#define array_size 1000
//P
#define no_threads 16
int a[array_size];
int global_index = 0;
int sum = 0;

pthread_mutex_t mutex1;

void *slave(void *ignored)
{
    int local_index, partial_sum = 0;
    do {
        pthread_mutex_lock(&mutex1);
          local_index = global_index;
          global_index++;
        pthread_mutex_unlock(&mutex1);

        if (local_index < array_size)
        {
            partial_sum += *(a + local_index);
        }
    } while (local_index < array_size);

    pthread_mutex_lock(&mutex1);
    sum += partial_sum;
    pthread_mutex_unlock(&mutex1);

    return(NULL);
}

main()
{
    clock_t begin = clock();
    int i;
    pthread_t thread[10];
    pthread_mutex_init(&mutex1, NULL);

    for (i = 0; i < array_size; i++)
    {
        a[i] = i+1;
    }

    for (i = 0; i < no_threads; i++)
    {
        if (pthread_create(&thread[i], NULL, slave, NULL) != 0)
        {
            perror("Pthread create fails");
        }
    }

    for (i = 0; i < no_threads; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
        {
            perror("Pthread join fails");
        }
    }

    printf("The sum of 1 to %i is %d\n", array_size, sum);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime Spent is: %3.5f\n", time_spent);
}
