#include <windows.h>
#include <stdio.h>


#ifndef _simple_multithread_h
#define _simple_multithread_h


DWORD WINAPI sum_primes(LPVOID arg);

void simple_multithread_example();


#endif