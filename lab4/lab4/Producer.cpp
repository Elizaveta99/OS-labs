#include <windows.h>
#include <iostream>
#include <string>
#include "SynqQueue.h"

using namespace std;

const CHAR canStartEventProducer[] = "CanStartEventProducer";

DWORD WINAPI Producer(SynqQueue *queue) 
{
	printf("Amount and value of produced numbers by Producer #%d : ", (*queue).numberOfThread);
	int amount, value;
	cin >> amount >> value;
	
	// �������� ������� ��� �������� Main ������� �� ���������� � ������
	HANDLE hReadyForWorkEventProducer = OpenEvent(EVENT_ALL_ACCESS, FALSE, (*queue).getReadyForWorkEvent());
	if (hReadyForWorkEventProducer == NULL)
	{
		cout << "Open event failed. Input any char to exit.\n";
		char c;
		cin >> c;
		return GetLastError();
	}

	SetEvent(hReadyForWorkEventProducer);					// ������� ������ � ����� ����������
	CloseHandle(hReadyForWorkEventProducer);   

	HANDLE hCanStartEventProducer = OpenEvent(EVENT_ALL_ACCESS, FALSE, canStartEventProducer);
	if (hCanStartEventProducer == NULL)
	{
		cout << "Open event failed start. Input any char to exit.\n";
		char c;
		cin >> c;
		return GetLastError();
	}
	WaitForSingleObject(hCanStartEventProducer, INFINITE);	// ��� ������ �� ������ ������

	for (int i = 0; i < amount; i++)
	{
		(*queue).insert(value);
		Sleep(15);
	}
	CloseHandle(hCanStartEventProducer);

	return 0;
}