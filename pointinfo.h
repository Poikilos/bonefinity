#ifndef POINTINFO_H
#define POINTINFO_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include "frameworkdummy.h"
#include "pmemory.h"
#include "preporting.h"

using namespace std;

typedef struct POINTINFO_STRUCT {
public:
	int x;
	int y;
	float fX;
	float fY;
	double dX;
	double dY;
	///<summary>
	///Warning, integer distance of a point is rounded so almost always not exact
	///</summary>
	int iDist;
	float fDist;
	double dDist;
} POINTINFO, *LPPOINTINFO;

#endif
