#ifndef _CONFIG_H_
#define _CONFIG_H_

class Config {
private:
	int m_rWidth;
	int m_rHeight;
	int m_rBits;
	bool m_rFullscreen;
	bool m_Active;
	float m_FOV;
	std::string m_Title;

public:
	void WindowWidth(int width) { m_rWidth = width; }
	int WindowWidth() { return m_rWidth; }

	void WindowHeight(int height) { m_rHeight = height; }
	int WindowHeight() { return m_rHeight; }

	void WindowColourDepth(int bits) { m_rBits = bits; }
	int WindowColourDepth() { return m_rBits; }

	void WindowFullscreen(bool fs) { m_rFullscreen = fs; }
	bool WindowFullscreen() { return m_rFullscreen; }

	void Active(bool active) { m_Active = active; }
	bool Active() { return m_Active; }

	void Title(std::string title) { m_Title = title; }
	std::string Title() { return m_Title; }

	void GameFOV(float fov) { m_FOV = fov; }
	float GameFOV() { return m_FOV; }
};

#endif