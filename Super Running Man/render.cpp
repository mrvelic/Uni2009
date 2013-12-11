// SIT251 Assignment 1
// Mirko Velic - 900216058

#include "common.h"
#include "render.h"

using namespace std;

// pillaged from MSDN
void ClearScreen( HANDLE hConsole )
{
	COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */ 
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
	DWORD dwConSize;                 /* number of character cells in the current buffer */ 

	/* get the number of character cells in the current buffer */ 

	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	/* fill the entire screen with blanks */ 

	bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );

	/* get the current text attribute */ 

	bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );

	/* now set the buffer's attributes accordingly */ 

	bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );

	/* put the cursor at (0, 0) */ 

	bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
	return;
}

// old placecursor function, not used anymore
void PlaceCursor(HANDLE hConsole, const int x, const int y) {
	COORD cCursor = { x, y };
    SetConsoleCursorPosition(hConsole, cCursor);
    return;
}

// gets the width of the console
int GetConsoleWidth(HANDLE hConsole) {
	CONSOLE_SCREEN_BUFFER_INFO cInfo;

	if (GetConsoleScreenBufferInfo(hConsole, &cInfo))
		return cInfo.srWindow.Right + 1;
	else
		return 0;

}

// gets the height of the console
int GetConsoleHeight(HANDLE hConsole) {
	CONSOLE_SCREEN_BUFFER_INFO cInfo;

	if (GetConsoleScreenBufferInfo(hConsole, &cInfo))
		return cInfo.srWindow.Bottom + 1;
	else
		return 0;

}

// calculates the size of a gameobject in width and height
ObjectSize GetObjectSize(GameObject gObject) {
	ObjectSize output = {0, 0};
	int current_line_size = 0;

	// for each character in the string
	for(int i = 0; i < lstrlen(gObject.text.c_str()); i++) {
		wchar_t current_char = gObject.text.c_str()[i];
		
		// if a newline, or the end of the objects text, increment heihgt and reset the current line size to 0
		if(current_char == '\n' || i == (lstrlen(gObject.text.c_str()) - 1)) {
			output.height++;
			current_line_size = 0;
		}
		// per character increase line size
		current_line_size++;

		// if the current line size is bigger than the width, set the width to the line size
		if(current_line_size > output.width) {
			output.width = current_line_size;
		}
	}

	// return object size
	return output;
}

void Render(HANDLE hConsole, vector<GameObject>* gObjects) {
	// Cheapo rendering method
	// this function basically draws all characters to a buffer then uses the windows API to display it

	// Get the size of the console... 
	int size_x = GetConsoleWidth(hConsole);
	int size_y = GetConsoleHeight(hConsole);

	// some variables
	CHAR_INFO* screen; // screen array
	SMALL_RECT srctScreen; // screen size rectangle

	// set screen size based on console size
	srctScreen.Top = 0;
	srctScreen.Left = 0;
	srctScreen.Bottom = size_y;
	srctScreen.Right = size_x;

	// number of total characters on screen
	int numChar = size_y * size_x;

	// make array of "CHAR_INFO"'s equal to the size of characters on screen
	screen = new CHAR_INFO[numChar];
	// screen size co-ordinate
	COORD screenSize = { size_x, size_y };
	// point of origin on screen
	COORD startPos = { 0, 0 };

	// blank out the screen
	for(int i = 0; i < numChar; i++) {
		screen[i].Char.UnicodeChar = wchar_t(0);
		screen[i].Attributes = FOREGROUND_INTENSITY;
	}

	// for every object we have...
	for(int i = (gObjects->size() - 1); i >= 0; i--) {
		GameObject currentObject = gObjects->at(i);
		// check if dead (probably wont end up using this)
		if(currentObject.isDead == 0) {
			int y_offset = 0;
			int x_offset = 0;
			const wchar_t* currentObjectString;
			currentObjectString = currentObject.text.c_str();

			// for every character in the objects text string...
			for(int k = 0; k < lstrlen(currentObjectString); k++) {
				// if we encounter a newline, go down a line but we want to retain our screen position
				// so set offsets accordingly
				wchar_t current_char = currentObjectString[k];

				if(current_char == '\n') { 
					x_offset = 0;
					y_offset++;
				} else {
					// calculate the actual positions of the individual characters

					// most of this stuff is to center the object around the x / y coord
					int y_center_offset = 0;
					int x_center_offset = 0;
					if(currentObject.drawFromCentre == 1) {
						// get object size to change character offset to make it centered..

						ObjectSize objSize = GetObjectSize(currentObject);
						if(objSize.height > 1) {
							int y_calc_offset = (int)ceil((double)objSize.height / 2.0f);
							if(y_calc_offset >= 0) y_center_offset = y_calc_offset;
						}

						if(objSize.width > 1) {
							int x_calc_offset = (int)floor((double)objSize.width / 2.0f);
							if(x_calc_offset >= 0) x_center_offset = x_calc_offset;
						}
					}

					int y_pos = currentObject.y + y_offset - y_center_offset;
					int x_pos = currentObject.x + x_offset - x_center_offset;

					// offset for our Y position since the array for the screen is 1D
					// it needs to be offset by the number of characters we arent using to move to the next line
					int char_offset = 0;

					// calculate the actual offset for the position of Y
					for(int y = 0; y < y_pos; y++) {
						char_offset += x_pos + (size_x - x_pos);
					}

					if(current_char != TP) {
						if(x_pos < size_x) {
							// change the character in the screen array to what we want
							// also dont wanna go outside the array bounds (can happen with the centering offsets, eevil)
							if((x_pos + char_offset) >= 0 && (x_pos + char_offset) < numChar) {
								// only over-write null characters (if overwriting previously set characters, it seems really slow :S)
								if(screen[x_pos + char_offset].Char.UnicodeChar == wchar_t(0)) {
									screen[x_pos + char_offset].Char.UnicodeChar = current_char;
									screen[x_pos + char_offset].Attributes = currentObject.attributes;
								}
							}
						}
					}
					// increment character X position
					x_offset++;
				}
			}
			//delete[] currentObjectString;
		}
	}
	// replace console output with our own...
	WriteConsoleOutput(hConsole, screen, screenSize, startPos, &srctScreen);

	delete[] screen;
}