
#ifndef _TVRes_H_
#define _TVRes_H_

#include "KeystrokeEngine.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "resource.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
BOOL CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif // _TVRes_H_
