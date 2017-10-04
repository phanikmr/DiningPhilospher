#include <stdio.h>
#include <time.h>
#define HAVE_STRUCT_TIMESPEC
#pragma warning(disable : 4996) 
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>
#define NO_OF_PHILOSOPHERS 5

sem_t chopstick[NO_OF_PHILOSOPHERS];
sem_t mutex;
pthread_t philospher[NO_OF_PHILOSOPHERS];
int status[NO_OF_PHILOSOPHERS];

void printStatus() {
	int i;
	for (i = 0; i<NO_OF_PHILOSOPHERS; i++) {
		printf("Philosopher%d is ", i);
		if (status[i])
			printf("eating\n");
		else
			printf("thinking\n");
	}
	printf("\n\n");
}

void * diningPhilosphers(void *id) {
	int *pid = (int *)id;
	do {
		sem_wait(&chopstick[*pid]);
		sem_wait(&chopstick[(*pid + 1) % NO_OF_PHILOSOPHERS]);
		status[*pid] = 1;
		sem_wait(&mutex);
		printStatus();
		Sleep(3000);
		sem_post(&mutex);
		status[*pid] = 0;
		sem_post(&chopstick[*pid]);
		sem_post(&chopstick[(*pid + 1) % NO_OF_PHILOSOPHERS]);
	} while (1);
	return NULL;
}

int main() {
	setbuf(stdout, NULL);
	int i, pid[NO_OF_PHILOSOPHERS];
	sem_init(&mutex, 0, 1);
	for (i = 0; i< NO_OF_PHILOSOPHERS; i++)
	{
		pid[i] = i;
		status[i] = 0;
	}
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		sem_init(&chopstick[i], 0, 1);
	}
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		if (pthread_create(&philospher[i], NULL, &diningPhilosphers, &pid[i]))
			printf("Error creating Thread");
	}
	for (i = 0; i < NO_OF_PHILOSOPHERS; i++) {
		pthread_join(philospher[i], NULL);
	}
	return 0;
}