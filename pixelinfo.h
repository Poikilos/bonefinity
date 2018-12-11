#ifndef PIXELINFO_H
#define PIXELINFO_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include "frameworkdummy.h"
#include "pmemory.h"
#include "preporting.h"

using namespace std;

typedef struct PIXELINFO_STRUCT {
public:
	unsigned char data[4];
	///<summary>
	///Freely usable for distance from a point or other custom use
	///</summary>
	float distance;
	///<summary>
	///Freely usable for weighted averaging or other custom use
	///</summary>
	float weight;
} PIXELINFO, *LPPIXELINFO;

#endif
