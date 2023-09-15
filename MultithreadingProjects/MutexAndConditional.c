#include "MutexAndConditional.h"


//HANDLE mutexFuel;
CONDITION_VARIABLE condFuel;
CRITICAL_SECTION lockFuel;

int fuel = 0;

DWORD WINAPI fuel_filling(LPVOID arg) {

	for (int i = 0; i < 5; i++) {
		EnterCriticalSection(&lockFuel);
		fuel += 30;
		printf("Filled fuel: %d\n", fuel);
		Sleep(1000);
		WakeConditionVariable(&condFuel);
		LeaveCriticalSection(&lockFuel);
	}

	
	/* Mutex Equivalent in POSIX */
	//DWORD waitResult;
	//BOOL continueLoop = TRUE;
	//while (continueLoop) {

	//		waitResult = WaitForSingleObject(mutexFuel, INFINITE);

	//		switch (waitResult) {
	//		case WAIT_OBJECT_0:
	//			fuel += 30;
	//			printf("Filled fuel: %d\n", fuel);
	//			continueLoop = FALSE;
	//			Sleep(1000);
	//			ReleaseMutex(mutexFuel);
	//			break;

	//		case WAIT_ABANDONED:
	//			return FALSE;
	//		}
	//	}
	//}

	return 0;
}

DWORD WINAPI car(LPVOID arg) {

	EnterCriticalSection(&lockFuel);

	while (fuel < 40) {
		printf("No fuel, waiting...\n");
		SleepConditionVariableCS(&condFuel, &lockFuel, INFINITE);
	}
	fuel -= 40;
	printf("Got fuel, left is: %d\n", fuel);
	Sleep(1000);

	LeaveCriticalSection(&lockFuel);
	
	/* Mutex Equivalent in POSIX */
	//DWORD waitResult;
	//BOOL continueLoop = TRUE;
	//while (continueLoop) {
	//	waitResult = WaitForSingleObject(mutexFuel, INFINITE);
	//	
	//	switch (waitResult) {
	//		case WAIT_OBJECT_0:
	//			//while (fuel < 40) {
	//				//printf("No fuel, waiting...\n");
	//				//SleepConditionVariableCS(&condFuel, NULL, INFINITE);
	//				//SleepConditionVariableSRW(&condFuel, &mutexFuel, INFINITE, 0);
	//				//Sleep(2000);
	//			//}
	//			fuel -= 40;
	//			printf("Got fuel, left is: %d\n", fuel);
	//			continueLoop = FALSE;
	//			Sleep(1000);
	//			ReleaseMutex(mutexFuel);
	//			break;

	//		case WAIT_ABANDONED:
	//			return FALSE;
	//	}
	//}

	return 0;
}

void mutex_and_conditional_example() {
	HANDLE tr[6];
	DWORD id[6];
	
	InitializeConditionVariable(&condFuel);
	InitializeCriticalSection(&lockFuel);
	
	/* Mutex Equivalent in POSIX */
	//mutexFuel = CreateMutex(NULL, FALSE, NULL);
	//if (mutexFuel == NULL) {
	//	printf("CreateMutex error: %d\n", GetLastError());
	//	exit(1);
	//}

	for (int i = 0; i < 6; i++) {
		if (i == 4 || i == 5) {
			tr[i] = CreateThread(NULL, 0, fuel_filling, NULL, 0, &id[i]);
			
			if (tr[i] == NULL) {
				printf("CreateThread error: %d\n", GetLastError());
				exit(1);
			}
		}
		else {
			tr[i] = CreateThread(NULL, 0, car, NULL, 0, &id[i]);
			
			if (tr[i] == NULL) {
				printf("CreateThread error: %d\n", GetLastError());
				exit(1);
			}
		}
	}

	WakeAllConditionVariable(&condFuel);

	WaitForMultipleObjects(6, tr, TRUE, INFINITE);

	for (int i = 0; i < 6; i++) {
		CloseHandle(tr[i]);
	}

	//CloseHandle(mutexFuel);

	printf("Fuel = %d\n", fuel);
}
