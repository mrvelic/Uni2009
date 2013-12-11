#ifndef _LEVEL_BOUNDIRES_H_
#define _LEVEL_BOUNDIRES_H_

class LevelBoundries {
public:
	float BoundryLeft;
	float BoundryRight;
	float LevelSpan;
	float FloorLevel;
	float ExitPoint;

	LevelBoundries() {
		BoundryLeft = 0.0f;
		BoundryRight = 0.0f;
		LevelSpan = 0.0f;
		FloorLevel = 0.0f;
		ExitPoint = 0.0f;
	}
};

#endif