// TVRes.cpp : Defines the entry point for the application.
//

#include "TVRes.h"

/*HWND g_hwndDlg = NULL;
HANDLE g_File = NULL;
char g_szBuffer[1025];
DWORD g_dwWritten;*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	int ret = DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CHANGE_RES), NULL, WndProc);
	return 0;
}

BOOL CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message) 
	{
/*		case WM_INITDIALOG:
		{
			g_hwndDlg = hWnd;
			//g_File = CreateFile("TVRes.txt", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, 0);
			//wsprintf(g_szBuffer, "TVRes Log file\n");
			//WriteFile(g_File, g_szBuffer, lstrlen(g_szBuffer), &g_dwWritten, NULL);
			break;
		}
		case WM_KEYDOWN:
		{						
			wsprintf(g_szBuffer, "%X - %X \n", wParam, lParam);
			WriteFile(g_File, g_szBuffer, lstrlen(g_szBuffer), &g_dwWritten, NULL);
		}
		case WM_DESTROY:
		{
			CloseHandle(g_File);
		}*/
		case WM_CLOSE:
		{
			EndDialog(hWnd, IDOK);
			break;
		}
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{		
				case IDC_BUTTON_TV:
				{
					switch (wmEvent) 
					{ 
						case BN_CLICKED: 
						{
							DEVMODE CurrentDM;
							EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &CurrentDM);							
							CurrentDM.dmPelsWidth  = 800;
							CurrentDM.dmPelsHeight = 600;
							ChangeDisplaySettings(&CurrentDM, 0);
							
// Virtual-Key Codes - ms-help://MS.VSCC/MS.MSDNVS/winui/vkeys_529f.htm
							/*
							BYTE bKeyboardOld[256];
							GetKeyboardState(bKeyboardOld);

							BYTE bKeyboard[256];
							GetKeyboardState(bKeyboard);
							bKeyboard[VK_SHIFT] = 0xff;
							bKeyboard[VK_CONTROL] = 0xff;
							bKeyboard[VK_SHIFT] = 0xff;
							bKeyboard[VK_LSHIFT] = 0xff;
							bKeyboard[VK_LCONTROL] = 0xff;
							bKeyboard[VK_LSHIFT] = 0xff;
							bKeyboard[0x54] = 0xff;
							SetKeyboardState(bKeyboard);
							Sleep(50);
							SetKeyboardState(bKeyboardOld);
							*/
							/*
							INPUT inChange[4];
							inChange[0].type = INPUT_KEYBOARD;
							inChange[0].ki.wVk = VK_SHIFT;
							inChange[0].ki.wScan = 0;
							inChange[0].ki.time = 1;
							inChange[0].ki.dwFlags = 0;
							inChange[0].ki.dwExtraInfo = 0;

							inChange[1].type = INPUT_KEYBOARD;
							inChange[1].ki.wVk = VK_CONTROL;
							inChange[1].ki.wScan = 0;
							inChange[1].ki.time = 1;
							inChange[1].ki.dwFlags = 0;
							inChange[1].ki.dwExtraInfo = 0;

							inChange[2].type = INPUT_KEYBOARD;
							inChange[2].ki.wVk = VK_MENU;
							inChange[2].ki.wScan = 0;
							inChange[2].ki.time = 1;
							inChange[2].ki.dwFlags = 0;
							inChange[2].ki.dwExtraInfo = 0;

							inChange[3].type = INPUT_KEYBOARD;
							inChange[3].ki.wVk = 0x54;
							inChange[3].ki.wScan = 0;
							inChange[3].ki.time = 1;
							inChange[3].ki.dwFlags = 0;
							inChange[3].ki.dwExtraInfo = 0;

							SendInput(4, inChange, sizeof(INPUT)*4);
*/

// ms-help://MS.VSCC/MS.MSDNVS/winui/keybinpt_1ltg.htm
							/*UINT SendInput(
  UINT nInputs,     // count of input events
  LPINPUT pInputs,  // array of input events
  int cbSize        // size of structure
);
							
							INPUT inChange;
							inChange.type = INPUT_KEYBOARD;
							inChange.ki.wVk = VK_SHIFT | VK_CONTROL | VK_MENU | 0x53;
							inChange.ki.wScan = 0;
							inChange.ki.time = 0;
							inChange.ki.dwFlags = 0;
							inChange.ki.dwExtraInfo = 0;
							
							SendInput(1, &inChange, sizeof(INPUT));
*/
							//SendMessage(HWND_BROADCAST, WM_HOTKEY, 1, MAKELPARAM(MOD_ALT|MOD_CONTROL|MOD_SHIFT, 0x54));
							CKeystrokeEngine objKeystrokes("<CTRL+ALT+SHIFT>T");
							objKeystrokes.SendKeys();
							break;
						}
					}
					break;
				}
				case IDC_BUTTON_SINGLE:
				{
					switch (wmEvent) 
					{ 
						case BN_CLICKED: 
						{
							CKeystrokeEngine objKeystrokes("<CTRL+ALT+SHIFT>S");
							objKeystrokes.SendKeys();

							DEVMODE CurrentDM;
							EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &CurrentDM);							
							CurrentDM.dmPelsWidth  = 1024;
							CurrentDM.dmPelsHeight = 768;
							ChangeDisplaySettings(&CurrentDM, 0);
							break;
						}
					}
					break;
				}
				case IDC_BUTTON_CLOSE:
				{
					switch (wmEvent) 
					{ 
						case BN_CLICKED: 
						{
							EndDialog(hWnd, IDOK);
							break;
						}
					}
					break;
				}
				default:
					return FALSE;
			}
			break;
	}
	return FALSE;
}

