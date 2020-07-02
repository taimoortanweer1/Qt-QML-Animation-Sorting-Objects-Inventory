#ifndef MISCFUNC_H_INCLUDED
#define MISCFUNC_H_INCLUDED


#include <string>



using namespace std;

string doubleToString(double input, int minDecPlaces = 0, int maxDecPlaces = 10);
//string longToString(long input);
double stringToDouble(const string* input);
double absd(double input);
long absl(long input);
double stdDev(double *array, int arraySize);
//string resizeLeft(string input, int newSize, char padChar = ' ');
//string capitalize(string input);
//bool calSwitch();

//void funcKey(CVga *vga, int buttonNum, CFont *font, char *text);

#endif // MISCFUNC_H_INCLUDED
