/**
	*  Part of MusicBar, a plugin to control foobar2000/TCMP from the titlebar
	*
	*  Copyright (C) Jory Stone - 2003
	*  Based on code from WinAmpBar Copyright (C) Gavin - 2000-2001
	*
	*  MusicBar is free software; you can redistribute it and/or modify
	*  it under the terms of the GNU General Public License as published by
	*  the Free Software Foundation; either version 2, or (at your option)
	*  any later version.
	*
	*  MusicBar is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	*  GNU General Public License for more details.
	*
	*  You should have received a copy of the GNU General Public License
	*  along with this program; see the file COPYING.  If not, write to
	*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
	*
*/

#ifndef _TITLE_BAR_H_
#define _TITLE_BAR_H_

#define STRICT
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <commctrl.h>
/*
#include <vector>
#include <string.h>
#include <string>
*/
#include <assert.h>
#include "resource.h"
#define TITLE_BAR_VERSION "0.0.4"

#include "DbgOut.h"

class TitleBar_Callback {
public:
	virtual int TitleBar_Pause()=0;		
	virtual int TitleBar_Previous()=0;	
	virtual int TitleBar_Next()=0;	
	virtual int TitleBar_Random()=0;
	// The following functions are called to store various settings
	virtual int TitleBar_Write(const char *name, DWORD value)=0;
	virtual int TitleBar_WriteStr(const char *name, const char *value)=0;
	virtual DWORD TitleBar_Read(const char *name, DWORD defaultValue)=0;
	virtual const char *TitleBar_ReadStr(const char *name)=0;
};

class TitleBar {
public:
	TitleBar(TitleBar_Callback *callback);
	~TitleBar();

	void SetDllHandle(HINSTANCE handle);
	void SetPlayerWindow(HWND window);
	bool Init();
	HWND ShowConfig(HWND parent);

	void SetSongTitle(const TCHAR *title);
	void SetTooltipTitle(const wchar_t *tooltip);
	const char *GetSongFormat();
	const char *GetTooltipFormat();
	void UpdateTime(DWORD elapsed, DWORD total);

	static const char *GetAboutText();
protected:
	/// Attach our bar window to the hwnd window
	void Attach(HWND hwnd, DWORD style);
	/// Find a suitable window and move our bar window over
	/// it to the correct place
	void DoReposition();
	int WindowsFeaturesWidth(HWND hwnd, DWORD style);
	/// Check if the styles are ok to overlay our bar on top of
	bool IsValidStyles(DWORD style, DWORD exStyle);
	/// Is the specified window suitable for attaching to? 
	/// (doesn't check that it's foreground.)
	bool IsWindowSuitable(HWND hwnd, DWORD* style);
	/// Calculate the string width in pixels in the specified font
	int StringWidth(HFONT font, const TCHAR* s, int len, bool hasColors = false);
	void LoadButtonBitmap();
	/// Redraw only the time
	void DrawTime(HDC hdc, RECT* rect);
	/// Redraw the whole thing
	void DrawWholeBar();
	void DoTooltip(HWND hwnd, NMTTDISPINFOW* disp);
	void UpdateButtons(int newButton);
	void HandleButton(UINT msg, int x);

	static LRESULT CALLBACK TitleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK ConfigWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/// Are we enabled?
	bool m_Enabled;
	/// HINSTANCE of our dll, (for resources)
	HINSTANCE m_Dll;
	/// HWND of our bar window
	HWND m_BarWindow;	
	/// Should our bar window be visible?
	bool m_BarVisible;
	/// HWND of our host player app
	HWND m_PlayerWindow;
	/// HWND of the last poor window we hijacked with our bar window :D
	HWND m_LastVictimWindow;
	/// HWND of our configuration window
	HWND m_ConfigWindow;
	/// The font we use to draw the text
	HFONT m_Font;
	/// Color of the text
	COLORREF m_Color;
	/// Should we display the title?
	bool m_DisplayTitle;
	/// Format spec to be used for song title
	char m_SongFormat[512];
	/// Current song title
	TCHAR m_SongTitle[512];
	DWORD m_SongTitleLen;
	
	/// Format spec used for the tooltip
	char m_TooltipFormat[512];
	wchar_t m_TooltipTitle[512];
	/// Should we display the elapsed time?
	bool m_DisplayTime;
	/// Pixel width of reserved space for drawing current playing time
	WORD m_TimeWidth;
	/// Current elapsed time of current track
	DWORD m_CurrentTrackTime;
	/// Current total time of current track
	DWORD m_TotalTrackTime;

	/// Location of custom bitmap
	TCHAR m_BitmapFile[256];
	/// Button image list
	HIMAGELIST m_ButtonImages;
	/// Index of the currently pressed button
	WORD m_ButtonPressed;
	WORD m_ButtonCount;
	/// Space to add in between buttons
	WORD m_ButtonSpacing;
	/// Callback for player interface
	TitleBar_Callback *m_Callback;
};

#endif // _TITLE_BAR_H_
