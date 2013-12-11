#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include "TerrainLineLoop.h"

class Terrain {
private:
	vector<TerrainLineLoop*>* m_LineLoops;
public:
	Terrain() { m_LineLoops = new vector<TerrainLineLoop*>; }
	void AddLineLoop(TerrainLineLoop* loop) { m_LineLoops->push_back(loop); }

	bool CheckCollision(Vertex3f point);

	vector<TerrainLineLoop*>* GetLineLoops() { return m_LineLoops; }
	~Terrain() { delete m_LineLoops; }
};

#endif