#ifndef _TEX2D_H_
#define _TEX2D_H_
#include "LodePNG.h"

class Texture2D {
protected:
	unsigned int m_ID;
	int m_Height, m_Width;
public:
	int Width() { return m_Width; }
	int Height() { return m_Height; }
	unsigned int ID() { return m_ID; }
	Texture2D() {}
	Texture2D(string filename) {
		LoadFromFile(filename);
	}

	virtual void LoadFromFile(string filename) {}
};

#endif