#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "SynqQueue.h"

using namespace std;

int main()
{
	cout << "Queue size : ";
	int size;
	cin >> size;
	SynqQueue queue;
	queue.setSize(size);					//создание очереди
	cout << "Amount Producer's threads : ";
	int producer_threads;
	cin >> producer_threads;
	cout << "Amount Consumer's threads : ";
	int consumer_threads;
	cin >> consumer_threads;

	
	// create pr cons theads amount
	// wait signal from all? pr coms
	// send signal to start all pr cons
	//

	
	cout << "Main works\n";
	cout << "Press any key to exit ";
	cin.ignore();
	cin.get();

	return 0;
}