#include "wordbrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HMENU hMenu;
HWND hSize, hString, hLength, hOut;

wchar_t lSize[255], lString[255], lLength[255], lOut[255];
int sSize, sLength;
char sString[255], sOut[255];

int createWindow(HINSTANCE, HINSTANCE, LPSTR, int);
void AddMenus(HWND);
void AddControls(HWND);
void getText(void);