#ifndef _MENU_H_
#define _MENU_H_

struct BackgroundInfo {
	float top, left, right, bottom, animStep, maxAnim, minAnim, stepAmount;
	int animDirection, screenWidth, screenHeight;
};

int RunMenu(HANDLE hConsole, HANDLE hInput);
void ShowInstructions(vector<GameObject>* vDialogs);
void GenerateBackgroundFrame(vector<GameObject>* pixels, vector<WORD>* colorTable, BackgroundInfo* bgInfo);
void CreateMenuDialog(vector<GameObject>* vDialogs, int x, int y, wstring text);
void DismissDialogs(vector<GameObject>* vDialogs);
void CentreObjects(vector<GameObject>* vObjects);
void ShowGameResult(vector<GameObject>* vDialogs, Score* gameResult);
#endif