#include "SynqQueue.h"

SynqQueue::SynqQueue()
{
}

SynqQueue::~SynqQueue()
{
	delete queue;
}

void SynqQueue::insert(int nElenent)
{

}

int SynqQueue::remove()
{

}

void SynqQueue::setSize(int sz)
{
	size = sz;
	queue = new int[size];
}

int SynqQueue::getSize()
{
	return size;
}
