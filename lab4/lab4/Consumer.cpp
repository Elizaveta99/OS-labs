#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "SynqQueue.h"

using namespace std;

CHAR canStartEventConsumer[] = "CanStartEventConsumer";

DWORD WINAPI Consumer(SynqQueue *queue) 
{
	//cout << "Amount of consumed numbers by Consumer #" << (*queue).numberOfThread << " : ";
	printf("Amount of consumed numbers by Consumer #%d : ", (*queue).numberOfThread);
	int amount;
	cin >> amount;

	// открытие события для отправки Main сигнала на готовность к работе
	HANDLE hReadyForWorkEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEvent == NULL)
	{
		cout << "Open event failed consumer." << endl;
		//cout << "Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEvent);			// послали сигнал о своей готовности
	CloseHandle(hReadyForWorkEvent);

	HANDLE hCanStartEventConsumer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventConsumer);
	if (hCanStartEventConsumer == NULL)
	{
		cout << "Open event failed cons start." << endl;
		//cout << "Input any char to exit." << endl;
		char c;
		cin >> c;
		return GetLastError();
	}
	WaitForSingleObject(hCanStartEventConsumer, INFINITE);	// ждём сигнал на начало работы

	CRITICAL_SECTION *cs = (*queue).getCriticalSection();
	EnterCriticalSection(cs);  // правильно ли адрес беру?
	for (int i = 0; i < amount; i++)
	{
		int value = (*queue).remove();
		//cout << "Consumed number : " << value << "\n";
		printf("Consumed number : %d\n", value);
		Sleep(15);
	}
	LeaveCriticalSection(cs);

	cout << "Consumer \n";
	CloseHandle(hCanStartEventConsumer);

	return 0;
}