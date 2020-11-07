#ifndef KEYBOARDINPUT
#define KEYBOARDINPUT

#include <iostream>

//Conditions
extern bool endProgram;
extern bool control;
extern bool colorSelection;
extern bool mirrorImage;

//Adjusting values
extern double alpha;
extern double beta;
extern double gamma;
extern double delta;

//Number of hand colors calculated and being used to binarize image
extern int colorsDefined;

void processInput(int key);

#endif // !KEYBOARDINPUT
