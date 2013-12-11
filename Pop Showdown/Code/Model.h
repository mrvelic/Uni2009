#ifndef _MODEL_H_
#define _MODEL_H_
#include "common.h"
#include "Polygon.h"

class Model {
public:
	int numPolygons;
	vector<TriPolygon*> polygons;
};

#endif