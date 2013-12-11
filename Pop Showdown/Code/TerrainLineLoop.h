#ifndef _TERRAINLINELOOP_H_
#define _TERRAINLINELOOP_H_
#include "common.h"
#include "Polygon.h"

class TerrainLineLoop {
private:
	vector<Vertex3f> m_Verticies;
public:
	vector<Vertex3f> GetVerticies() { return m_Verticies; }
	void AddVertex(Vertex3f vert) { m_Verticies.push_back(vert); }
};

#endif