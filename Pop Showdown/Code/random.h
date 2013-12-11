#ifndef _RANDOM_H_
#define _RANDOM_H_
#include "common.h"

class Colour {
public:
	float r, g, b;
	Colour(float R, float G, float B) {
		r = R;
		g = G;
		b = B;
	}
	Colour() {
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
	}

	float GLr() { return r / 255.0f; }
	float GLg() { return g / 255.0f; }
	float GLb() { return b / 255.0f; }
};

class LevelBackground {
public:
	Colour* topColour;
	Colour* bottomColour;

	LevelBackground(Colour* top, Colour* bottom) {
		topColour = top;
		bottomColour = bottom;
	}
	~LevelBackground() {
		delete topColour;
		delete bottomColour;
	}
};

class GameString {
public:
	string Text;
	Colour TextColour;
	float X, Y;
};

class Tools {
public:
	static vector<string> SplitString(string str, char delim) {
		vector<string> output;
		stringstream outSS;
		for(unsigned int i = 0; i < strlen(str.c_str()); i++) {
			char cc = str.c_str()[i];
			if(cc == delim) {
				output.push_back(outSS.str());
				outSS.str("");
			} else {
				outSS << cc;
			}
		}
		output.push_back(outSS.str());
		return output;
	}

};

#endif