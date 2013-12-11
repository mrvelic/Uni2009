#ifndef _COMMON_H_
#define _COMMON_H_

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <complex>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <gl\gl.h>
#include <gl\glu.h>


using namespace std;

class Point2i {
public:
	int x;
	int y;

	Point2i() { x = 0; y = 0; }
	Point2i(int X, int Y) { x = X; y = Y; }
};

#endif