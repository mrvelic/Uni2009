#include "common.h"
#include "Terrain.h"

bool Terrain::CheckCollision(Vertex3f point) {
	for(unsigned int l = 0; l < m_LineLoops->size(); l++) {
		vector<Vertex3f> polygon = m_LineLoops->at(l)->GetVerticies();

		if(polygon.size() > 0) {
			unsigned int counter = 0;
			unsigned int i;
			double xinters;
			Vertex3f p1, p2;

			p1 = polygon[0];
			for (i = 1; i <= polygon.size(); i++) {
				p2 = polygon[i % polygon.size()];
				if (point.y > MIN(p1.y,p2.y)) {
					if (point.y <= MAX(p1.y,p2.y)) {
						if (point.x <= MAX(p1.x,p2.x)) {
							if (p1.y != p2.y) {
								xinters = (point.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
								if (p1.x == p2.x || point.x <= xinters)
									counter++;
							}
						}
					}
				}
				p1 = p2;
			}

			if (!(counter % 2 == 0))
				return true;
		}
	}
	return false;
}