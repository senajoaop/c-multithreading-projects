#include <windows.h>
#include <stdio.h>


#ifndef _mutex_and_conditional_h
#define _mutex_and_conditional_h


DWORD WINAPI fuel_filling(LPVOID arg);

DWORD WINAPI car(LPVOID arg);

void mutex_and_conditional_example();


#endif