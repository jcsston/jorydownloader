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

#include "TitleBar.h"

#ifdef FOOBAR2000_PLUGIN
// Goody, we can use the colored text out
#include "..\Foobar2000_SDK\foobar2000\utf8api\utf8api.h"

//GetTextExtentPoint32 wrapper, removes color marks
int get_text_width_color_TitleBar(HDC dc,const char* src,int len)
{
	SIZE s;
	int ptr = 0;
	int start = 0;
	int rv = 0;
	if (len<0) len = lstrlenA(src);
	while(ptr<len)
	{
		if (src[ptr]==3)
		{
			if (uGetTextExtentPoint32(dc, src+start, ptr-start, &s))
				rv += s.cx;
			ptr++;
			while(ptr<len && src[ptr]!=3) ptr++;
			if (ptr<len) ptr++;
			start = ptr;
		}
		else ptr++;
	}
	if (uGetTextExtentPoint32(dc, src+start, ptr-start, &s))
		rv += s.cx;
	return rv;
}
#endif

typedef BOOL (WINAPI *SetLayerWinAttrFn)( HWND, COLORREF, BYTE, DWORD );

#ifndef WS_EX_LAYERED
# define WS_EX_LAYERED    0x00080000L
# define WS_EX_NOACTIVATE 0x08000000L

# define LWA_COLORKEY  1
# define LWA_ALPHA     2
#endif

#define BUTTON_HEIGHT 12
#define BUTTON_WIDTH 12

TitleBar::TitleBar(TitleBar_Callback *callback)
{	
	assert(callback != NULL);
	m_Callback = callback;

	m_Enabled = m_Callback->TitleBar_Read("Enabled", true);
	m_BarWindow = NULL;
	m_BarVisible = false;
	m_PlayerWindow = NULL;
	m_LastVictimWindow = NULL;
	m_ConfigWindow = NULL;
	m_Font = NULL;
	m_DisplayTitle = m_Callback->TitleBar_Read("Display Song Title", true);
	m_DisplayTime = m_Callback->TitleBar_Read("Display Time", true);
	m_TimeWidth = 0;
	m_CurrentTrackTime = 0;
	m_TotalTrackTime = 0;
	m_ButtonSpacing = m_Callback->TitleBar_Read("Button Spacing", 4);
	m_TimeWidth = 0;
	m_Color = m_Callback->TitleBar_Read("Default Color", RGB(255, 255, 255));
	m_ButtonCount = 4;
	m_ButtonImages = NULL;
	m_ButtonPressed = -1;
	ZeroMemory(m_SongFormat, sizeof(m_SongFormat));	
	lstrcpynA(m_SongFormat, m_Callback->TitleBar_ReadStr("Song Format"), sizeof(m_SongFormat)-1);
	if (lstrlenA(m_SongFormat) == 0) {
		lstrcpyA(m_SongFormat, "%artist% - %title%");
	}
	ZeroMemory(m_SongTitle, sizeof(m_SongTitle));

	ZeroMemory(m_TooltipFormat, sizeof(m_TooltipFormat));	
	lstrcpynA(m_TooltipFormat, m_Callback->TitleBar_ReadStr("Tooltip Format"), sizeof(m_TooltipFormat)-1);
	if (lstrlenA(m_TooltipFormat) == 0) {
#ifdef FOOBAR2000_PLUGIN
		lstrcpyA(m_TooltipFormat, "%artist% - %title% - $if2($pad(%__bitrate_dynamic%,3)'kbps',$if2(%__bitrate%'kbps',))");
#else
		lstrcpyA(m_TooltipFormat, "%artist% - %title%");
#endif
	}	
	ZeroMemory(m_TooltipTitle, sizeof(m_TooltipTitle));	
	ZeroMemory(m_BitmapFile, sizeof(m_BitmapFile));	
	lstrcpyn(m_BitmapFile, m_Callback->TitleBar_ReadStr("Button Bitmap"), sizeof(m_BitmapFile)-1);
	m_SongTitleLen = 0;
};

TitleBar::~TitleBar()
{	
	if (m_Callback != NULL) {
		m_Callback->TitleBar_Write("Enabled", m_Enabled);
		m_Callback->TitleBar_Write("Display Song Title", m_DisplayTitle);
		m_Callback->TitleBar_Write("Display Time", m_DisplayTime);
		m_Callback->TitleBar_Write("Button Spacing", m_ButtonSpacing);
		m_Callback->TitleBar_Write("Default Color", m_Color);
		m_Callback->TitleBar_WriteStr("Tooltip Format", m_TooltipFormat);
		m_Callback->TitleBar_WriteStr("Song Format", m_SongFormat);
		m_Callback->TitleBar_WriteStr("Button Bitmap", m_BitmapFile);
	}
};

void TitleBar::SetDllHandle(HINSTANCE handle)
{
	m_Dll = handle;
};

void TitleBar::SetPlayerWindow(HWND window)
{
	m_PlayerWindow = window;
};
// /me is trying to port his favorite WA plugin 'WinampBar'

bool TitleBar::Init() 
{	
	// Setup the common controls
  INITCOMMONCONTROLSEX commonControlsInit;
  ZeroMemory(&commonControlsInit, sizeof(commonControlsInit));
  commonControlsInit.dwSize = sizeof(commonControlsInit);
  commonControlsInit.dwICC = ICC_BAR_CLASSES | ICC_LISTVIEW_CLASSES | ICC_UPDOWN_CLASS;
  if (!InitCommonControlsEx(&commonControlsInit))
		return false;

	WNDCLASS wnd;
  wnd.style         = CS_VREDRAW | CS_HREDRAW;
  wnd.lpfnWndProc   = TitleWndProc;
  wnd.cbClsExtra    = 0;
  wnd.cbWndExtra    = 0;
  wnd.hInstance     = m_Dll;
  wnd.hIcon         = NULL;
  wnd.hCursor       = LoadCursor( NULL, IDC_ARROW );
  /* Same colour as given to layering function below */
  wnd.hbrBackground = (HBRUSH)( COLOR_ACTIVECAPTION + 1);
  wnd.lpszMenuName  = NULL;
  wnd.lpszClassName = _T("TitleBar");
  if (!RegisterClass(&wnd))
    return false;

	NONCLIENTMETRICS metrics;
  /* Need to use the exact caption fount, since I'm using this to measure the
     size of a window's existing caption */
  metrics.cbSize = sizeof(metrics);
  if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &metrics, FALSE))
    m_Font = CreateFontIndirect( &metrics.lfCaptionFont );
  if (!m_Font)
    return false;

  m_BarWindow = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
                      L"TitleBar", NULL, WS_POPUP,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      NULL, NULL, m_Dll, (LPVOID)this);

	//m_BarWindow = CreateDialogParam(m_Dll, MAKEINTRESOURCE(IDD_DIALOGBAR), NULL, TitleWndProc, (LPARAM)this);
	if (m_BarWindow == NULL)
		return false;


	//SetWindowPos(m_BarWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE);
  
	SetLayerWinAttrFn setLayerWinAttr = (SetLayerWinAttrFn)GetProcAddress(GetModuleHandle(_T("USER32.DLL")), "SetLayeredWindowAttributes");
  if (setLayerWinAttr != NULL)
    setLayerWinAttr(m_BarWindow, GetSysColor(COLOR_ACTIVECAPTION), 0, LWA_COLORKEY);

	m_TimeWidth = StringWidth(m_Font, _T("99:99"), 5);

	/* CREATE A TOOLTIP WINDOW
  HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST,
      TOOLTIPS_CLASS,
      NULL,
      WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      m_BarWindow,
      NULL,
      m_Dll,
      NULL
      );

  SetWindowPos(hwndTT,
      HWND_TOPMOST,
      0,
      0,
      0,
      0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

  // GET COORDINATES OF THE MAIN CLIENT AREA
	RECT rect;
  GetClientRect(m_BarWindow, &rect);

  // INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
	TOOLINFO ti;
  ti.cbSize = sizeof(TOOLINFO);
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = m_BarWindow;
  ti.hinst = m_Dll;
  ti.uId = 123;
  ti.lpszText = "test";
      // ToolTip control will cover the whole window
  ti.rect.left = rect.left;    
  ti.rect.top = rect.top;
  ti.rect.right = rect.right;
  ti.rect.bottom = rect.bottom;
  
  // SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
  SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
*/
  TOOLINFOW info;

  HWND TooltipWindow =
      CreateWindowExW( 0, TOOLTIPS_CLASSW, NULL,
                      WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT,
                      m_BarWindow, NULL, m_Dll, NULL );
  if( !TooltipWindow )
    return FALSE;

  SetWindowPos( TooltipWindow, HWND_TOPMOST, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );

  info.cbSize = sizeof( info );
  info.uFlags = TTF_IDISHWND | TTF_TRANSPARENT | TTF_SUBCLASS;
  info.hwnd = m_BarWindow;
  info.uId = (UINT)m_BarWindow;
  info.hinst = m_Dll;
  info.lpszText = LPSTR_TEXTCALLBACKW;
  ZeroMemory( &info.rect, sizeof( info.rect ) );
  if( !SendMessageW( TooltipWindow, TTM_ADDTOOL, 0, (LPARAM)&info ) )
    return FALSE;

  SendMessageW( TooltipWindow, TTM_ACTIVATE, TRUE, 0 );

	return true;
};

HWND TitleBar::ShowConfig(HWND parent) {
	NOTE1("TitleBar::ShowConfig() - parent: 0x%X", parent);
	m_ConfigWindow = CreateDialogParam(m_Dll, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), parent, ConfigWndProc, (LPARAM)this);	
	
	if (m_ConfigWindow == NULL) {
		// Try other window
		m_ConfigWindow = CreateDialogParam(m_Dll, MAKEINTRESOURCE(IDD_DIALOG_CONFIG_POPUP), parent, ConfigWndProc, (LPARAM)this);	
	}
	/*
	TCHAR err_msg[256];
	DWORD error_code = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, err_msg, 255, NULL);
	*/

	return m_ConfigWindow;
};

void TitleBar::SetSongTitle(const TCHAR *title) {
	assert(title != NULL);	
	
	lstrcpyn(m_SongTitle, title, sizeof(m_SongTitle)/sizeof(TCHAR)-1);
	
	m_SongTitleLen = lstrlen(m_SongTitle);
};

void TitleBar::SetTooltipTitle(const wchar_t *tooltip) {
	assert(tooltip != NULL);	
	
	lstrcpynW(m_TooltipTitle, tooltip, sizeof(m_TooltipTitle)/sizeof(wchar_t)-1);
};

const char *TitleBar::GetSongFormat() {
	return m_SongFormat;
}

const char *TitleBar::GetTooltipFormat() {
	return m_TooltipFormat;
}

void TitleBar::UpdateTime(DWORD elapsed, DWORD total) {
	m_CurrentTrackTime = elapsed;
	m_TotalTrackTime = total;
};

const char *TitleBar::GetAboutText() {
	return 
		"Music (Title)Bar v" TITLE_BAR_VERSION "\n"
		"by Jory Stone \n"
		"One of my all-time favorite WinAmp plugins has been the "
		"Winamp title bar control. It creates a small set of controls "
		"in the corner of the currently active window and also displays "
		"the title and time. \n"
		"Music (Title)Bar is a port of that plugin to TCMP and foobar2000, "
		"with some new features, namely custom formating strings for foobar2000. \n"
		"http://webjory.tripod.com/musicbar/ \n";
}
void TitleBar::Attach(HWND hwnd, DWORD style)
{
  TCHAR title[256];
  RECT r;
  int left, top, width, height, chars;

  /* SetParent would be wonderful for sorting out most of the positioning,
     repainting and clipping difficulties, except for one thing - it only
     lets the child window appear in the parent's _client_ area, so it's
     actually totally useless here.
          SetParent( WinampBarWindow, hwnd );
     Shame, because it could have made it very easy to ensure that the
     plugin window was exactly on top of the appropriate application
     instead of floating on top of everything. */

  GetWindowRect( hwnd, &r );

  /* The offset should take into account the system menu icon and border
     widths, and a bit more to keep the real title and the song title
     separate, but I'm too lazy, and "a bit" (30) will do! */
  left = 30 + r.left;

  chars = GetWindowText( hwnd, title, sizeof( title ) / sizeof( TCHAR ) );
  left += StringWidth( m_Font, title, chars );

  width = r.right - left - WindowsFeaturesWidth(hwnd, style) - m_ButtonSpacing;

  if( width > 0 )
  {
    /* Where the plugin window last was */
    static int lastLeft, lastTop, lastWidth, lastHeight;

    /* See if the space we need is smaller than the available, and shrink
       our request accordingly */
    int requiredWidth = 100;//NumButtons * BUTTON_WIDTH + BUTTON_GAP;
    if (m_DisplayTitle)
    {
      requiredWidth += StringWidth(m_Font, m_SongTitle, m_SongTitleLen);
    }
    if (m_DisplayTime)
      requiredWidth += m_TimeWidth;

    if (requiredWidth < width)
    {
      left += width - requiredWidth;
      width = requiredWidth;
    }

    top = r.top + GetSystemMetrics( ( style & WS_THICKFRAME ) ?
                                                 SM_CYSIZEFRAME :
                                                 SM_CYFIXEDFRAME );
    height = GetSystemMetrics( SM_CYSIZE );

    /* SetWindowPos() is expensive, so avoid if possible */
    if (lastLeft != left || lastTop != top ||
        lastWidth != width || lastHeight != height ||
        m_LastVictimWindow != hwnd)
    {
#ifdef _DEBUG
        TCHAR buff[ 64 ];
        if( !GetWindowText( hwnd, buff, sizeof( buff ) / sizeof( TCHAR ) ) )
          buff[ 0 ] = 0; /* Ensure string is NUL terminated on failure */
        NOTE10("Repositioning to window %X \"%s\" "
              "window (%d,%d)+(%d,%d) plugin (%d,%d)+(%d,%d)",
              hwnd, buff,
              r.left, r.top, r.right - r.left, r.bottom - r.top,
              left, top, width, height);
#endif

      SetWindowPos( m_BarWindow, NULL,
                    left, top, width, height,
                    SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSENDCHANGING |
                        SWP_NOZORDER | SWP_NOOWNERZORDER );
      lastLeft = left; lastTop = top;
      lastWidth = width; lastHeight = height;
      m_LastVictimWindow = hwnd;
    }

    if( !m_BarVisible )
    {
      ShowWindow( m_BarWindow, SW_SHOWNA );
      m_BarVisible = true;
    }
  }
	else{
    NOTE1("Not enough room for plugin on window %X", hwnd);
	}
}

void TitleBar::DoReposition()
{
	/// We scan the top 300 windows
  int i = 300;
  HWND hwnd;

  // This gets the topmost child of the desktop
  for (hwnd = GetWindow(GetDesktopWindow(), GW_CHILD); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT))
  {
    DWORD style;
    if (IsWindowSuitable(hwnd, &style))
    {
      Attach(hwnd, style);
      return;
    }
    if (!--i)
      hwnd = NULL;
  }

  NOTE1("Nothing found after %d attempts", 300 - i);

  // Getting here means that nothing suitable was found, 
	// so just hide the bar window
  ShowWindow(m_BarWindow, SW_HIDE);
  m_BarVisible = false;
}

/* Most captioned windows have things like minimise or close buttons on
   the right: this routine calculates the width of these features on the
   passed in window. Because the callers of this routine typically use
   the style already, this function accepts that as an argument too, a
   bit quicker than getting it again. Unfortunately, not all the sizes
   associated with the placement of these features are defined by Windows
   metrics, so some, I am afraid, are just magic numbers found by careful
   measurement. Additionally, I grab the other sizes every time, instead
   of caching them, just in case they are changed under my feet! */

int TitleBar::WindowsFeaturesWidth(HWND hwnd, DWORD style)
{
  /* The reasoning behind this is: if a Window has a system menu specified,
     it'll have a close button; additionally it can have nothing else OR a
     help button OR both minimise and maximise buttons, and in that latter
     case, the buttons are adjacent so trim off the inter-button spacing.
     Note that the buttons are actually 2 pixels smaller than the system
     metric, as it appears that that includes the spacing. */
  int buttonWidth = GetSystemMetrics( SM_CXSIZE );
  int width = 2 + GetSystemMetrics( ( style & WS_THICKFRAME ) ?
                                         SM_CXSIZEFRAME : SM_CXFIXEDFRAME );
  if( style & WS_SYSMENU )
    width += buttonWidth;
  /* I think I should be able to check for maximise and minimise buttons
     separately, but some applications (all?), such as calculator, display
     both even though one is not enabled - since it's better to have a gap
     I assume that if one's there both are */
  if( style & ( WS_MAXIMIZEBOX | WS_MINIMIZEBOX ) )
    width += buttonWidth * 2 - 2; /* -2 since the buttons touch */
  else if( GetWindowLong( hwnd, GWL_EXSTYLE ) & WS_EX_CONTEXTHELP )
    width += buttonWidth;

  return width;
}

bool TitleBar::IsValidStyles(DWORD style, DWORD exStyle) {
# define STYLES_OF_INTEREST   ( WS_CAPTION | WS_VISIBLE | WS_CHILD )
# define STYLES_WANTED        ( WS_CAPTION | WS_VISIBLE )
# define EXSTYLES_OF_INTEREST ( WS_EX_TOOLWINDOW | WS_EX_MDICHILD )
# define EXSTYLES_WANTED      0
  return ( style   & STYLES_OF_INTEREST   ) == STYLES_WANTED &&
         ( exStyle & EXSTYLES_OF_INTEREST ) == EXSTYLES_WANTED;
/*
	const DWORD STYLES_OF_INTEREST = (WS_CAPTION | WS_VISIBLE | WS_CHILD);
	const DWORD STYLES_WANTED = (WS_CAPTION | WS_VISIBLE);
	const DWORD EXSTYLES_OF_INTEREST = (WS_EX_TOOLWINDOW | WS_EX_MDICHILD);
	const DWORD EXSTYLES_WANTED = 0;

  return (style & STYLES_OF_INTEREST) == STYLES_WANTED &&
         (exStyle & EXSTYLES_OF_INTEREST) == EXSTYLES_WANTED;*/
}

bool TitleBar::IsWindowSuitable(HWND hwnd, DWORD* style)
{
	bool bSuitable = true;
	DWORD localStyle = 0;

	// Invisible Windows are not suitable for overlaying our control window on
	bSuitable = IsWindowVisible(hwnd);
	// Minimized Windows are no use :P
	bSuitable = bSuitable && !IsIconic(hwnd);
	// What kind of styles does our target window have?
	localStyle = GetWindowLong(hwnd, GWL_STYLE);
	bSuitable = bSuitable && IsValidStyles(localStyle, GetWindowLong(hwnd, GWL_EXSTYLE));
	if (style != NULL)
		*style = localStyle;

	// We don't need the controls overlaped on the actual player
	bSuitable = bSuitable && (hwnd != m_PlayerWindow);
	// Ow, It would be very bad to try to create our window inside our own
	bSuitable = bSuitable && (hwnd != m_BarWindow);

  return bSuitable;
}

/// Calculate the string width in pixels in the specified font
int TitleBar::StringWidth(HFONT font, const TCHAR* s, int len, bool hasColors)
{
  int width = 0;
  // Any old DC will do, since we're drawing nothing, so pick one that is
  // guaranteed to exist always - the entire desktop
  HDC hdc = GetDC(NULL);
  if (hdc != NULL)
  {
    SIZE size;
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
//#pragma message("TODO: Add Color Support for title formating?")
#if FOOBAR2000_PLUGIN
		if (hasColors) {
			width = get_text_width_color_TitleBar(hdc, s, len);
		}
#endif
		if (width == 0) {
			if (GetTextExtentPoint32(hdc, s, len, &size))      
				width = size.cx;
		}
    SelectObject(hdc, oldFont);
    ReleaseDC(NULL, hdc);
  }
  return width;
}

/// Redraw only the time
void TitleBar::DrawTime(HDC hdc, RECT* rect)
{
  TCHAR buff[16];
  int chars, t1, t2;

  if( rect->right - m_TimeWidth < rect->left )
    return; /* Not enough space to draw time */

  int t = m_CurrentTrackTime;
  if( t < 0 )
    return;
  t1 = t / 60;
  t2 = t % 60;

  chars = wsprintf(buff, _T("%u:%02u"), t1, t2 );
  DrawText( hdc, buff, chars, rect,
            DT_SINGLELINE | DT_VCENTER | DT_RIGHT | DT_NOPREFIX );
}

void TitleBar::LoadButtonBitmap()
{
  /* The following, and the similar for resources, looks like a quick and easy
     way to read the image list, but it seems to restrict the bitmap to 4 bit
     colours... Instead, I laborously read the bitmap and then force it into
     the image list. Ho hum.

   ButtonImages = ImageList_LoadImage( NULL, BitmapFile,
                                       BUTTON_WIDTH, 0, CLR_NONE, IMAGE_BITMAP,
                                       LR_DEFAULTCOLOR | LR_LOADFROMFILE );
  */

  HBITMAP bmp;

	if (!m_BitmapFile[0] ||
      (bmp = (HBITMAP)LoadImage(NULL, m_BitmapFile, IMAGE_BITMAP,
                         BUTTON_WIDTH * m_ButtonCount * 2, BUTTON_HEIGHT,
                         LR_DEFAULTCOLOR | LR_LOADFROMFILE)) == NULL)

   bmp = (HBITMAP)LoadImage(m_Dll, MAKEINTRESOURCE(IDB_BITMAP_BUTTONS), IMAGE_BITMAP,
                     BUTTON_WIDTH * m_ButtonCount * 2, BUTTON_HEIGHT,
                     LR_DEFAULTCOLOR | LR_SHARED);

  if (bmp) // I'll be very surprised if the resource read fails, but...
  {
    HDC screen = GetDC(NULL);
    int colourDepth = GetDeviceCaps(screen, PLANES) * GetDeviceCaps(screen, BITSPIXEL);
    /* Use the fact that ILC_COLORx has the value of the colour depth for
       common sizes */
    m_ButtonImages = ImageList_Create( BUTTON_WIDTH, BUTTON_HEIGHT, colourDepth,
                                     m_ButtonCount * 2, 0 );
    if (m_ButtonImages)
      ImageList_Add(m_ButtonImages, bmp, NULL);
    DeleteObject(bmp);
  }

  NOTE1(m_ButtonImages ? "Bitmap loaded" : "Bitmap load failed %d", GetLastError());
}

void TitleBar::DrawWholeBar()
{
  const BOOL drawAll = TRUE;

  PAINTSTRUCT ps;
  int x;
  RECT rect;
  HDC hdc = BeginPaint(m_BarWindow, &ps);
  HFONT oldFont = (HFONT)SelectObject(hdc, m_Font );

  GetClientRect(m_BarWindow, &rect);

  NOTE2("Drawing size (%d,%d)", rect.right, rect.bottom);

  /* Initialise to where to start drawing things */
  x = rect.right;

  /* Draw the buttons */
  if (drawAll)
  {
    int y = ( rect.bottom - BUTTON_HEIGHT ) / 2;
    int i;

    if (!m_ButtonImages)
      LoadButtonBitmap();

    for(i = m_ButtonCount - 1; i >= 0; --i)
    {
      BYTE button = i;
      x -= BUTTON_WIDTH;
      if (i == m_ButtonPressed) // Select the "inverse" bitmap
        button += m_ButtonCount;
      ImageList_Draw(m_ButtonImages, button, hdc, x, y, ILD_NORMAL);
      NOTE4("Drawing button %d (%d) at (%d,%d)", i, button, x, y);
    }
  }
  else

    x -= m_ButtonCount * BUTTON_WIDTH;

  x -= m_ButtonSpacing; /* Arbitrary small gap between text and buttons */

  SetTextColor(hdc, m_Color);
  SetBkMode(hdc, TRANSPARENT);

  /* Draw the time field */
  if (m_DisplayTime)
  {
    rect.right = x;
    DrawTime(hdc, &rect);

    x -= m_TimeWidth;
  }

  /* Finally, draw the song title */
  if (m_DisplayTitle && drawAll)
  {
    rect.right = x;
#pragma message("TODO: Add Color Support for title formating?")
#ifdef FOOBAR2000_PLUGIN
		int renderedWidth = StringWidth(m_Font, m_SongTitle, m_SongTitleLen, true);	
		int centerHeight = (rect.bottom - uGetTextHeight(hdc)) / 2;
		if (renderedWidth > x)
			uTextOutColors(hdc, m_SongTitle, m_SongTitleLen, 0, centerHeight, &rect, FALSE, m_Color);
		else
			uTextOutColors(hdc, m_SongTitle, m_SongTitleLen, x-renderedWidth, centerHeight, &rect, FALSE, m_Color);
#else
		DrawText(hdc, m_SongTitle, m_SongTitleLen, &rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT | DT_NOPREFIX | DT_END_ELLIPSIS);
#endif
  }

  SelectObject(hdc, oldFont);
  EndPaint(m_BarWindow, &ps);
};

void TitleBar::UpdateButtons(int newButton)
{
  if( newButton != m_ButtonPressed )
  {
    RECT windowRect;
    LONG x, y;
    HDC hdc = GetDC( m_BarWindow );
    GetClientRect( m_BarWindow, &windowRect );
    x = windowRect.right - m_ButtonCount * BUTTON_WIDTH;
    y = ( windowRect.bottom - BUTTON_HEIGHT ) / 2;

    /* First restore the old button */
    if( m_ButtonPressed >= 0 )
      ImageList_Draw( m_ButtonImages, m_ButtonPressed,
                      hdc, x + m_ButtonPressed * BUTTON_WIDTH, y, ILD_NORMAL );

    /* Now "press" the new one */
    if( newButton >= 0 )
      ImageList_Draw(m_ButtonImages,newButton + m_ButtonCount,
                      hdc, x + newButton * BUTTON_WIDTH, y, ILD_NORMAL );

    ReleaseDC( m_BarWindow, hdc );

    /* Finally, update the note of the currently pressed button */
    m_ButtonPressed = newButton;
    //TimeSincePressed = 0;
  } /* end if buttons different */
}

void TitleBar::DoTooltip(HWND hwnd, NMTTDISPINFOW* disp)
{
	static long tipX;
	DWORD pos = GetMessagePos();
	POINT pt;
	pt.x = GET_X_LPARAM(pos);
	// Only do something if the pointer has moved since last time
	long a = pt.x - tipX;
	a = a >= 0 ? a : -a ;
	if (a > BUTTON_WIDTH / 2 )
	{
		RECT r;
		int buttonStart;
		pt.y = GET_Y_LPARAM( pos );
		ScreenToClient( hwnd, &pt );
		tipX = pt.x;

		NOTE2("Request for tooltip at (%d, %d)", pt.x, pt.y);

		GetClientRect( hwnd, &r );
		buttonStart = r.right - (int)m_ButtonCount * BUTTON_WIDTH;
		// Tooltips appear only if the pointer is over the button part
		if ( pt.x >= buttonStart && pt.x < r.right )
		{
			int button = ( pt.x - buttonStart ) / BUTTON_WIDTH;
			disp->lpszText = (wchar_t *)m_TooltipTitle;//ButtonDetails[ Buttons[ button ] ].tip;
		}
	} // end if pointer has moved since last tip
}

void TitleBar::HandleButton(UINT msg, int x)
{
  RECT rect;
  int buttonOffset = -1, newButton;
  GetClientRect( m_BarWindow, &rect );
  if( x >= rect.right - (int)m_ButtonCount * BUTTON_WIDTH && x < rect.right )
    buttonOffset = x - ( rect.right - m_ButtonCount * BUTTON_WIDTH );

  NOTE3("Mouse (0x%X) at %d (%d)", msg, x, buttonOffset);

  switch( msg )
  {
    case WM_LBUTTONDOWN:
      UpdateButtons(buttonOffset >= 0 ? buttonOffset / BUTTON_WIDTH : -1);
      break;

    case WM_MOUSEMOVE:
      if ( ( GetAsyncKeyState( GetSystemMetrics( SM_SWAPBUTTON ) ?
                               VK_RBUTTON : VK_LBUTTON ) & 0x8000 ) &&
          buttonOffset >= 0 )
        newButton = buttonOffset / BUTTON_WIDTH;
      else
        newButton = -1;
      if( newButton != m_ButtonPressed )
        UpdateButtons( newButton );
      break;

    case WM_LBUTTONUP:
      if ( buttonOffset >= 0 )
      {
        /* Just in case there was a missed move, recalculate the button
           rather than assume it was the last pressed one */
        m_ButtonPressed = buttonOffset / BUTTON_WIDTH;
				if (m_ButtonPressed == 0) {
					// Previous
					m_Callback->TitleBar_Previous();
				} else if (m_ButtonPressed == 1) {
					// Pause
					m_Callback->TitleBar_Pause();
				} else if (m_ButtonPressed == 2) {
					// Next
					m_Callback->TitleBar_Next();
				} else if (m_ButtonPressed == 3) {
					// Random
					m_Callback->TitleBar_Random();
				}

        NOTE2("Press button %d out of %d", m_ButtonPressed, m_ButtonCount);        
        UpdateButtons(-1);
      }
      break;
  }
}

LRESULT CALLBACK TitleBar::TitleWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TitleBar *myClass = NULL;//(TitleBar *)GetWindowLong(hWnd, DWL_USER);
	switch (message) 
	{
		case WM_CREATE:
		{
			CREATESTRUCT *createStruct = (CREATESTRUCT *)lParam;
			if (createStruct != NULL) {
				myClass = (TitleBar *)createStruct->lpCreateParams;
				SetWindowLong(hWnd, DWL_USER, (LONG)myClass);
				SetTimer(hWnd, 456, 250, NULL);
			}
			break;
		}
    case WM_TIMER:
		{
			assert(myClass != NULL);
			if (wParam == 456) {
				if (!myClass->m_Enabled) {
					if (myClass->m_BarVisible) {
						ShowWindow(myClass->m_BarWindow, SW_HIDE);
						myClass->m_BarVisible = false;
					}
					break;
				}
				
				//HandleAutorepeat();
				myClass->DoReposition();
				InvalidateRect(myClass->m_BarWindow, NULL, TRUE);
			}
      return 0;
		}
		case WM_DESTROY:
		{
			KillTimer(hWnd, 456);
			break;
		}
		case WM_PAINT:
		{
			assert(myClass != NULL);
			if (myClass->m_Enabled) {
				myClass->DrawWholeBar();
			}
			return 0;
		}
    case WM_NOTIFY:
		{
      if(((NMHDR*)lParam)->code == TTN_GETDISPINFOW) {
				myClass->DoTooltip(hWnd, (NMTTDISPINFOW*)lParam);
			}
			break;
		}
		case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
		{
      assert(myClass != NULL);
			if (myClass->m_Enabled) {
				myClass->HandleButton(message, GET_X_LPARAM(lParam));
			}
      return 0;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
};

BOOL CALLBACK TitleBar::ConfigWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TitleBar *myClass = (TitleBar *)GetWindowLong(hWnd, DWL_USER);
	switch (message) 
	{
		case WM_INITDIALOG:
		{												
			myClass = (TitleBar *)lParam;	
			//Store the TitleBar class in the DWL_USER			
			SetWindowLong(hWnd, DWL_USER, (LONG)myClass);
//#ifdef FOOBAR2000_PLUGIN
			SetDlgItemTextA(hWnd, IDC_EDIT_TITLE_FORMATING_STRING, myClass->m_SongFormat);
			SetDlgItemTextA(hWnd, IDC_EDIT_TOOLTIP_FORMATING_STRING, myClass->m_TooltipFormat);			
/*#else
			ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TITLE_FORMATING_STRING), SW_HIDE);			
			ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TITLE_FORMATING_STRING), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_STATIC_TOOLTIP_FORMATING_STRING), SW_HIDE);			
			ShowWindow(GetDlgItem(hWnd, IDC_EDIT_TOOLTIP_FORMATING_STRING), SW_HIDE);
#endif*/
			CheckDlgButton(hWnd, IDC_CHECK_ENABLE_TITLEBAR, myClass->m_Enabled);
			SetDlgItemText(hWnd, IDC_EDIT_CUSTOM_BUTTON_BITMAP, myClass->m_BitmapFile);
			SetTimer(hWnd, 456, 250, NULL);
			break;
		}
		case WM_COMMAND:
		{
			assert(myClass != NULL);
			WORD wmId    = LOWORD(wParam); 
			WORD wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{		
				case IDC_CHECK_ENABLE_TITLEBAR:
				{
					switch (wmEvent) 
					{ 
						case BN_CLICKED: 
						{
							myClass->m_Enabled = IsDlgButtonChecked(hWnd, IDC_CHECK_ENABLE_TITLEBAR);
							break;
						}
					}
					break;
				}
				case IDC_EDIT_TITLE_FORMATING_STRING:
				{
					switch (wmEvent) 
					{ 
						case EN_CHANGE: 
						{
							GetDlgItemText(hWnd, IDC_EDIT_TITLE_FORMATING_STRING, myClass->m_SongFormat, sizeof(myClass->m_SongFormat)/sizeof(char)-1);
							//myClass->m_Callback->TitleBar_Pause();
							break;
						}
					}
					break;
				}
				case IDC_EDIT_TOOLTIP_FORMATING_STRING:
				{
					switch (wmEvent) 
					{ 
						case EN_CHANGE: 
						{
							GetDlgItemText(hWnd, IDC_EDIT_TOOLTIP_FORMATING_STRING, myClass->m_TooltipFormat, sizeof(myClass->m_TooltipFormat)/sizeof(char)-1);
							//myClass->m_Callback->TitleBar_Pause();
							break;
						}
					}
					break;
				}
				case IDC_EDIT_CUSTOM_BUTTON_BITMAP:
				{
					switch (wmEvent) 
					{ 
						case EN_CHANGE: 
						{
							GetDlgItemText(hWnd, IDC_EDIT_CUSTOM_BUTTON_BITMAP, myClass->m_BitmapFile, sizeof(myClass->m_BitmapFile)/sizeof(TCHAR)-1);
							//myClass->m_Callback->TitleBar_Pause();
							break;
						}
					}
					break;
				}
				case IDC_BUTTON_BROWSE_CUSTOM_BUTTON_BITMAP:
				{
					switch (wmEvent) 
					{ 
						case BN_CLICKED: 
						{
							OPENFILENAME openFile;
							TCHAR fileName[256];
							memset(&openFile, 0, sizeof(OPENFILENAME));
							memset(fileName, 0, sizeof(fileName));
							openFile.lStructSize =  sizeof(OPENFILENAME);
							openFile.hwndOwner = hWnd;									
							openFile.lpstrFile = fileName;
							openFile.nMaxFile	= sizeof(fileName)/sizeof(TCHAR);
							openFile.lpstrFilter = _T("Bitmaps (*.bmp)\0 *.bmp\0\0");
							openFile.lpstrTitle	= _T("Open custom button bitmap...");
							openFile.Flags = OFN_EXPLORER;
							if (GetOpenFileName(&openFile)) {
								lstrcpy(myClass->m_BitmapFile, fileName);
								SetDlgItemText(hWnd, IDC_EDIT_CUSTOM_BUTTON_BITMAP, myClass->m_BitmapFile);
							}
							break;
						}
					}
					break;
				}
			}
			break;
		};
	}
	return FALSE;
};
