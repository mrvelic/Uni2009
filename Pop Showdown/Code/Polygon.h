#ifndef _POLYGON_H_
#define _POLYGON_H_

class Vertex3f {
public:
	float x, y, z;
	Vertex3f(float X, float Y, float Z) { x = X; y = Y, z = Z; }
	Vertex3f(float X, float Y) { x = X; y = Y; z = 0.0f; }
	Vertex3f() { x = 0.0f; y = 0.0f; z = 0.0f; }
};

class TriPolygon {
public:
	Vertex3f verticies[3];
};

#endif