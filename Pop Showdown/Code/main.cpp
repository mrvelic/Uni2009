/*********************************************************************
* NAME: Mirko Velic
* STUDENT ID: 900216058
* UNIT CODE: SIT251
* DATE: 5th of June 2010
*	
* PLAGIARISM AND COLLUSION
* Plagiarism occurs when a student passes off as the student’s own work, or copies 
* without acknowledgment as to its authorship, the work of any other person.
* Collusion occurs when a student obtains the agreement of another person for a
* fraudulent purpose with the intent of obtaining an advantage in submitting an 
* assignment or other work
*
* DECLARATION
* I certify that the attached work is entirely my own (or where submitted to meet the 
* requirements of an approved group assignment is the work of the group), except where 
* work quoted or paraphrased is acknowledged in the text. I also certify that it has not
* been submitted for assessment in any other unit or course. 
* I agree that Deakin University may make and retain copies of this work for the 
* purposes of marking and review, and may submit this work to an external plagiarism-
* detection service who may retain a copy for future plagiarism detection but will not 
* release it or use it for any other purpose.
*
************************************************************************
*
* References:
* LodePNG: http://lodev.org/lodepng/
* Window Creation Code (and some other small stuff): http://nehe.gamedev.net
* Ke$ha Picture: http://4.bp.blogspot.com/-F_3mP9OZNnw/TYSE8vlPEWI/AAAAAAAAAxY/cqTGTH-bTyc/s1600/Kesha.jpg
* Gun Sprites: http://www.droidz.org/stickmain/weapons.php?page=14
* 
**********************************************************************/

#include "common.h"
#include "main.h"

Application mainApplication;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	srand((int)time(NULL));
	// Run application
	return (mainApplication.Run());
}

