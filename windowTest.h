#include "wordbrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HMENU hMenu;
HWND hString, hLength, hOut;

SCROLLINFO si;

wchar_t lString[255], lLength[255], lOut[255];
int sLength;
char sString[255], sOut[255];

int createWindow(HINSTANCE, HINSTANCE, LPSTR, int);
void AddMenus(HWND);
void AddControls(HWND);
void getText(void);