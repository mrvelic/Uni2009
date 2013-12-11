#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "TextureOGL.h"

class Sprite {
private:
	Texture2D* m_Texture;
	float m_Width;
	float m_Height;
public:
	void Texture(Texture2D* tex) { m_Texture = tex; }
	Texture2D* Texture() { return m_Texture; }

	void Height(float height) { m_Height = height; }
	void Width(float width) { m_Width = width; }

	float Height() { return m_Height; }
	float Width() { return m_Width; }

	void Scale(float scale) { m_Height = scale; m_Width = scale; }
	float Scale() { return m_Height; }

	Sprite() { m_Width = 20.0f; m_Height = 20.0f; }
	Sprite(string filename) {
		m_Texture = new TextureOGL(filename);
	}

	~Sprite() {
		delete m_Texture;
	}
};

#endif