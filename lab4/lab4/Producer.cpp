#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdio>
#include "SynqQueue.h"

using namespace std;

CHAR canStartEventProducer[] = "CanStartEventProducer";

DWORD WINAPI Producer(SynqQueue *queue) 
{
	//cout << "Amount and value of produced numbers by Producer #" << (*queue).numberOfThread << " : ";
	printf("Amount and value of produced numbers by Producer #%d : ", (*queue).numberOfThread);
	int amount, value;
	cin >> amount >> value;

	// �������� ������� ��� �������� Main ������� �� ���������� � ������
	//cout << "name_producer : " << (*queue).getReadyForWorkEvent() << "\n";
	printf("name_producer : %s\n", (*queue).getReadyForWorkEvent());
	HANDLE hReadyForWorkEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEvent == NULL)
	{
		cout << "Open event failed producer." << endl;
		//cout << "Input any char to exit.\n" << endl;
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEvent);					// ������� ������ � ����� ����������
	CloseHandle(hReadyForWorkEvent);   // ?? �����?

	HANDLE hCanStartEventProducer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventProducer);
	if (hCanStartEventProducer == NULL)
	{
		cout << "Open event failed pr start." << endl;
		//cout << "Input any char to exit." << endl;
		char c;
		cin >> c;
		return GetLastError();
	}
	WaitForSingleObject(hCanStartEventProducer, INFINITE);	// ��� ������ �� ������ ������

	CRITICAL_SECTION *cs = (*queue).getCriticalSection();
	EnterCriticalSection(cs);  // ��������� �� ����� ����?
	for (int i = 0; i < amount; i++)
	{
		(*queue).insert(value);
		//cout << "Produced number : " << value << "\n";
		printf("Produced number : %d\n", value);
		Sleep(15);
	}
	LeaveCriticalSection(cs);

	cout << "Producer \n";
	CloseHandle(hCanStartEventProducer);

	return 0;
}