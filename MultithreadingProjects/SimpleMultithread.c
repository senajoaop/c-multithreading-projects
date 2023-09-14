#include "SimpleMultithread.h"

#define MAX_THREADS 2

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

DWORD WINAPI sum_primes(LPVOID arg) {
	int index = *(int*)arg;
	int sum = 0;
	
	for (int i = 0; i < 5; i++) {
		sum += primes[index + i];
	}
	printf("Local sum: %d\n", sum);

	return (DWORD)sum;
}

void simple_multithread_example() {
	HANDLE tr[MAX_THREADS];
	DWORD id[MAX_THREADS];

	int* argptr;
	int globalSum = 0;

	printf("Sum of the first ten prime numbers ( ");
	for (int i = 0; i < 10; i++) {
		printf("%d ", primes[i]);
	}
	printf(")\nUsing %d threads to do it concurrently:\n\n", MAX_THREADS);

	for (int i = 0; i < MAX_THREADS; i++) {
		argptr = (int*)malloc(sizeof(int));
		if (argptr == NULL) { exit(1); }
		
		*argptr = i * 5;
		
		tr[i] = CreateThread(NULL, 0, sum_primes, argptr, 0, &id[i]);
		
		if (tr[i] == NULL) {
			printf("Could not create thread\n");
			exit(1);
		}
		else {
			printf("Thread created\n");
		}
	}

	WaitForMultipleObjects(MAX_THREADS, tr, 0, INFINITE);

	DWORD res[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; i++) {
		WaitForSingleObject(tr[i], INFINITE);
		GetExitCodeThread(tr[i], &res[i]);
		printf("%d\n", (int)res[i]);
		globalSum += (int)res[i];
		CloseHandle(tr[i]);
	}

	printf("\nGlobal sum: %d\n", globalSum);

	free(argptr);
	argptr = NULL;
}
