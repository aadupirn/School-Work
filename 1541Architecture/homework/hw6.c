#include <stdio.h>
#include <pthread.h>
#include <time.h>

int a[array_size];
int global_index = 0;
int sum = 0;

pthread mutex_t mutex1;

void *slave(void *ignored)
{
    int no_threads = argv[1];
    int array_size  = argv[2];
    int local_index, partial_sum = 0;
    do {
        pthread mutex lock(&mutex1);
          local_index = global_index;
          global_index++;
        pthread mutex unlock(&mutex1);

        if (local_index < array_size)
            partial_sum += *(a + local_index);
    } while (local_index < array_size);

    pthread mutex lock(&mutex1);
    sum += partial_sum;
    pthread mutex unlock(&mutex1);

    return();
}

main()
{
    clock_t begin = clock();

    int i;
    pthread_t thread[10];
    pthread_mutex_init(&mutex1, NULL);

    for (i = 0; i < array_size; i++)
        a[i] = i+1;

    for (i = 0; i < no_threads; i++)
        if (pthread create(&thread[i], NULL, slave, NULL) != 0)
            perror("Pthread create fails");

    for (i = 0; i < no_threads; i++)
        if (pthread join(thread[i], NULL) != 0)
            perror("Pthread join fails");

    printf("The sum of 1 to %i is %d\n", array_size, sum);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}
