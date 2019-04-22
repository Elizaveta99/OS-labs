#include <windows.h>
#include <iostream>
#include <string>
#include "SynqQueue.h"

using namespace std;

const CHAR canStartEventConsumer[] = "CanStartEventConsumer";

DWORD WINAPI Consumer(SynqQueue *queue) 
{
	printf("Amount of consumed numbers by Consumer #%d : ", (*queue).numberOfThread);
	int amount;
	cin >> amount;
	
	// открытие события для отправки Main сигнала на готовность к работе
	HANDLE hReadyForWorkEventConsumer = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEventConsumer == NULL)
	{
		cout << "Open event failed. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEventConsumer);					// послали сигнал о своей готовности
	CloseHandle(hReadyForWorkEventConsumer);

	HANDLE hCanStartEventConsumer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventConsumer);
	if (hCanStartEventConsumer == NULL)
	{
		cout << "Open event failed start. Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}
	WaitForSingleObject(hCanStartEventConsumer, INFINITE);

	for (int i = 0; i < amount; i++)
	{
		int value = (*queue).remove();
		Sleep(15);
	}
	CloseHandle(hCanStartEventConsumer);

	return 0;
}