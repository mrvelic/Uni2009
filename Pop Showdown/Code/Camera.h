#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera {
private:
	float m_X, m_Y, m_Z;
public:
	void X(float x) { m_X = x; }
	void Y(float y) { m_Y = y; }
	void Z(float z) { m_Z = z; }
	float X() { return m_X; }
	float Y() { return m_Y; }
	float Z() { return m_Z; }

	Camera() {}
	Camera(float x, float y, float z) {
		m_X = x;
		m_Y = y;
		m_Z = z;
	}

	~Camera() {}
};
#endif
