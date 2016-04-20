#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

	void *print_message_function1( void *ptr )
	{
		while(1)
		{
			printf("hello from thread 1 - A\n");
			pthread_yield();
			printf("hello from thread 1 - B\n");
		}
	}

	void *print_message_function2( void *ptr )
	{
		while(1)
		{
			printf("hello from thread 2 - A\n");
			pthread_yield();
			printf("hello from thread 2 - B\n");
		}
	}


int main()
{
	pthread_t thread1, thread2;
	const char *message1 = "1-A";
	const char *message2 = "Thread 2";

	pthread_create( &thread1, NULL, print_message_function1, NULL);


	pthread_create( &thread2, NULL, print_message_function2, NULL);

	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	
	return 0;
}