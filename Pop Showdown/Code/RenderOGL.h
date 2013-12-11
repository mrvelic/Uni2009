#ifndef _RENDER_OGL_H_
#define _RENDER_OGL_H_
#include "common.h"
#include "Renderer.h"

class RenderOGL : public Renderer {
private:
	void Enable2D();
	void Disable2D();
	float drawRatio;
	GLuint m_FontBase;
	GLYPHMETRICSFLOAT m_FontMetrics[256];
public:
	RenderOGL() : Renderer() { drawRatio = 1000.0f; }
	virtual void Draw();
	virtual void BuildFont();
	virtual void DrawText(float x, float y, Colour colour, string text);
	void DrawSprite(Sprite* sprite, float scale);
	void DrawModel(Model* model);
	void DrawBackground();
	void DrawEntities(vector<Entity*>* ents, bool relative, Entity* relEnt);
};

#endif