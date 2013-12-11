#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "common.h"
#include "Entity.h"
#include "Camera.h"
#include "Terrain.h"
#include "random.h"
#include "WindowOGL.h"

class Renderer {
protected:
	vector<Entity*>* m_Entities;
	Terrain* m_Terrain;
	Camera* m_CurrentCamera;
	LevelBackground* m_Background;
	bool m_CameraSet, m_EntitiesSet, m_TerrainSet;
	Window* m_Window;
public:
	virtual void Draw() {}
	void SetEntities(vector<Entity*>* entities) { m_Entities = entities; m_EntitiesSet = true; }
	void SetCamera(Camera* cam) { m_CurrentCamera = cam; m_CameraSet = true; }
	void SetTerrain(Terrain* terrain) { m_Terrain = terrain; m_TerrainSet = true; }
	void SetBackground(LevelBackground* bg) { m_Background = bg; }
	void SetWindow(Window* win) { m_Window = win; }
	virtual void BuildFont() = 0;
	virtual void DrawText(float x, float y, Colour colour, string text) = 0;

	void ResetRenderer() {
		m_CameraSet = false;
		m_TerrainSet = false;
		m_EntitiesSet = false;
	}

	Renderer() {
		m_CameraSet = false;
		m_EntitiesSet = false;
		m_TerrainSet = false;
	}
};

#endif