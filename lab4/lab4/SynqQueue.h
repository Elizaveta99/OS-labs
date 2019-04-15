#pragma once
class SynqQueue
{
private:
	int size;
	int* queue; 
public:
	SynqQueue();
	virtual ~SynqQueue(); // ?virtual?
	void  insert(int nElement);
	int   remove(); 
	void setSize(int sz);
	int getSize();
};

