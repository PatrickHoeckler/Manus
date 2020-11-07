#include "keyboarInput.h"

void processInput(int key)
{
	if (control)
	{
		if (key == '*') control = !control;
		return;
	}
	if (key == '*') control = !control;

	else if (key == 's' && !colorSelection)
	{
		colorSelection = true;
		colorsDefined = 0;
	}
	else if (key == 's') colorSelection = false;
	else if (key == 'm') mirrorImage = !mirrorImage;
	else if (key == '+') alpha += 0.1;
	else if (key == '-' && alpha > 1.0) alpha -= 0.1;
	else if (key == '7') beta += 5;
	else if (key == '4' && beta > 0.0) beta -= 5;
	else if (key == '8') gamma += 1;
	else if (key == '5' && gamma > 0.0) gamma -= 1;
	else if (key == '9') delta += 2;
	else if (key == '6' && delta > 0.0) delta -= 2;
	else if (key == 27) endProgram = true;
	else if (key == 'p')
	{
		system("cls");
		std::cout << "Alpha: " << alpha << "\tBeta: " << beta << std::endl;
		std::cout << "Gamma: " << gamma << "\tDelta: " << delta << std::endl;
	}
}