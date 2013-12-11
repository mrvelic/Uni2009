#ifndef _TEXTURE_OGL_H_
#define _TEXTURE_OGL_H_
#include "Texture.h"
class TextureOGL : public Texture2D {
private:
	void LoadPNG(string filename);
public:
	virtual void LoadFromFile(string filename);
	TextureOGL(string filename) {
		LoadFromFile(filename);
	}
};
#endif