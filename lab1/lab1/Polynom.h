#pragma once

CONST DWORD maxSize = 100;

struct Polynom
{
	DWORD degreeNum;
	DWORD degreeDen;
	DWORD x;
	DWORD value;
	DWORD coefNum[maxSize], coefDen[maxSize];
};
