/**
	*  Part of MusicBar, a plugin to control foobar2000/TCMP from the titlebar
	*
	*  Copyright (C) Jory Stone - 2003
	*
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
//Include the TCMP CDL API
#include "CDLApi.h"
#include <process.h>
#include <string>

TitleBar *bar = NULL;
// Function declares
int CDL_API_CALL CDL_Load(int version);
void CDL_API_CALL CDL_Config(void);
void CDL_API_CALL CDL_Unload(void);
void CDL_API_CALL CDL_EventHandler(int Event, char* Param1, int Param2);

#define REG_KEY "Software\\CoreCodec\\The Core Media Player\\Music (Title)Bar\\"
DWORD MusicBar_GetRegistryValue(const char *value_key, DWORD default_value)
{	
	DWORD ret_value = default_value;
	HKEY key_handle = NULL;
	DWORD lpType = NULL;
	DWORD state = 0;

	RegCreateKeyExA(HKEY_CURRENT_USER, REG_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key_handle, &state);
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, temp, 0, reg_key, 1024, NULL); 
	if(state == 2)
	{
		DWORD size = sizeof(ret_value);
		RegQueryValueEx(key_handle, value_key, 0, &lpType, (BYTE*)&ret_value, &size);
	}
	RegCloseKey(key_handle);
	return ret_value;
}

void MusicBar_SetRegistryValue(const char *value_key, DWORD the_value)
{
	HKEY key_handle = NULL;
	DWORD lpType = NULL;
	DWORD state = 0;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0,1};

	RegCreateKeyExA(HKEY_CURRENT_USER, REG_KEY, 0, "", 0, KEY_WRITE, &sa, &key_handle, &state);

	DWORD size = sizeof(the_value);
	RegSetValueEx(key_handle, value_key, 0, REG_DWORD, (CONST BYTE*)&the_value, size);
	//char *err_key = new char[1024];
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, temp, 0, err_key, 1024, NULL); 
	RegCloseKey(key_handle);
};

const char *MusicBar_GetRegistryValueStr(const char *value_key)
{	
	HKEY key_handle = NULL;
	DWORD lpType = NULL;
	DWORD state = 0;
	static char key_text[MAX_PATH+2];
	static DWORD size = MAX_PATH;

	RegCreateKeyExA(HKEY_CURRENT_USER, REG_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key_handle, &state);
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, temp, 0, reg_key, 1024, NULL); 		
	if(state == REG_OPENED_EXISTING_KEY) {
		RegQueryValueExA(key_handle, value_key, 0, &lpType, (BYTE*)key_text, &size);	
	} else if (state == REG_CREATED_NEW_KEY) {
		lstrcpyA(key_text, "");
	}

	RegCloseKey(key_handle);
	return key_text;
}

void MusicBar_SetRegistryValueStr(const char *value_key, const char *the_value)
{
	HKEY key_handle = NULL;
	DWORD lpType = NULL;
	DWORD state = 0;
	SECURITY_ATTRIBUTES sa = {sizeof(sa), 0,1};

	RegCreateKeyExA(HKEY_CURRENT_USER, REG_KEY, 0, "", 0, KEY_WRITE, &sa, &key_handle, &state);

	DWORD size = lstrlenA(the_value)+1;
	RegSetValueExA(key_handle, value_key, 0, REG_SZ, (CONST BYTE*)the_value, size);
	//char *err_key = new char[1024];
	//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, temp, 0, err_key, 1024, NULL); 
	RegCloseKey(key_handle);
};

// ----------------------------------------------------------------------------

CDL_PluginHeader MyCDL =
{
    TITLE_BAR_VERSION,            // Our version number
    "Music (Title)Bar",				// Name of our plugin
    CDL_Load,               // Point to our load function
    CDL_Config,             // Point to our config procedure
    CDL_Unload,             // Point to our unload procedure
    NULL,
	CORE_NOFLAG
	| CORE_EVENTHANDLER
	,
    NULL,
    NULL,
    NULL,
		CDL_EventHandler,
		NULL,
		NULL
};

PCDL_CoreHeader pCoreCDL = NULL;

class MusicTitleBar_TCMP : public TitleBar_Callback {
protected:
	static void __cdecl TimeUpdateThread(void *dummy) {
		while (pCoreCDL->CDL_GetVersion != (int (CDL_API_CALL *)(void))-1) {
			if ((pCoreCDL->CDL_GetVersion != NULL) && (bar != NULL)) {
				bar->UpdateTime(pCoreCDL->CDL_GetPosition()/1000, pCoreCDL->CDL_GetDuration()/1000);
			}
			Sleep(900);
		}
		_endthread();
	};

public:
	MusicTitleBar_TCMP() {
		_beginthread(TimeUpdateThread, 0, this);
	}
	virtual int TitleBar_Pause() {
		pCoreCDL->CDL_PlayPause();	
		return 0;
	};		
	virtual int TitleBar_Previous() {
		pCoreCDL->CDL_PreviousItem();
		return 0;
	};		
	virtual int TitleBar_Next() {
		pCoreCDL->CDL_NextItem();
		return 0;
	};		
	virtual int TitleBar_Random() {
		pCoreCDL->CDL_SetShuffle();
		return 0;
	};		
	virtual int TitleBar_Write(const char *name, DWORD value) {
		MusicBar_SetRegistryValue(name, value);
		return 0;
	};
	virtual int TitleBar_WriteStr(const char *name, const char *value) {
		MusicBar_SetRegistryValueStr(name, value);
		return 0;
	};
	virtual DWORD TitleBar_Read(const char *name, DWORD defaultValue) {		
		return MusicBar_GetRegistryValue(name, defaultValue);
	};
	virtual const char *TitleBar_ReadStr(const char *name) {
		return MusicBar_GetRegistryValueStr(name);
	};
};
MusicTitleBar_TCMP myCallback;

// Handle to this DLL itself.
HINSTANCE g_hmodThisDll = NULL;	

/// I suppose this is called when TCMP is trying to find out if this DLL is a CDL plugin
CDL_API_EXPORT PCDL_PluginHeader CDL_GetPluginHeader(PCDL_CoreHeader pCoreHeader)
{
	NOTE("PCDL_PluginHeader()");

	pCoreCDL = pCoreHeader;
	return &MyCDL;
}

///The DLL starting point
BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{	
	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			NOTE("DllMain() - DLL_PROCESS_ATTACH");
			// Extension DLL one-time initialization
			g_hmodThisDll = hInst;
			break;
		case DLL_PROCESS_DETACH:
			NOTE("DllMain() - DLL_PROCESS_ATTACH");
			break;
		case DLL_THREAD_ATTACH:
			NOTE("DllMain() - DLL_THREAD_ATTACH");
			break;
		case DLL_THREAD_DETACH:
			NOTE("DllMain() - DLL_THREAD_DETACH");
			break;
		default:
			NOTE("DllMain() - Unknown Reason");
	}
	return TRUE;
}

///This is called by TCMP when TCMP loads this plugin
int CDL_API_CALL CDL_Load(int version)
{
	NOTE1("CDL_Load() - Version: %i", version);

	if(version < 400)
	{
		return CORE_NOT_OK;
	}
	bar = new TitleBar(&myCallback);
	bar->SetDllHandle(g_hmodThisDll);
	bar->Init();
	
	HWND configWindow = bar->ShowConfig(NULL);
	//global_pagedata->g_hTreeDialog = CreateDialog(global_pagedata->g_hAppInst, MAKEINTRESOURCE(IDD_TREEDIALOG), NULL, TreeDlgProc);
	pCoreCDL->CDL_HijackMe(configWindow, "Music TitleBar");
	ShowWindow(configWindow, SW_NORMAL);
	return CORE_OK;
}

/// This is called when the user clicks the 'Configure' button at the Plugin List in the TCMP Options
void CDL_API_CALL CDL_Config(void)
{
	NOTE("CDL_Config()");
	// About box
	MessageBoxA(NULL, TitleBar::GetAboutText(), "About Music (Title)Bar", 0);
}

/// This is called when TCMP is exiting and unloading all the plugins
void CDL_API_CALL CDL_Unload(void)
{
	NOTE("CDL_Unload()");
	if (bar != NULL)
		delete bar;
	bar = NULL;
	pCoreCDL->CDL_GetVersion = (int (CDL_API_CALL *)(void))-1;
	//DestroyWindow(global_pagedata->g_hTreeDialog);
}

/// We get event messages from TCMP here
void CDL_API_CALL CDL_EventHandler(int Event, char* Param1, int Param2)
{
	NOTE("CDL_EventHandler()");
	// The player send you some importants events
	switch(Event)
	{
		case CORE_FILELOADING:
			break;
		case CORE_FILELOADED:
		{
			if(Param1 != NULL) {				
				NOTE("Getting MetaData");
				///*
				//CDL_Metadata metadata = *pCoreCDL->CDL_GetMetadata();
				CDL_Metadata metadata;// = new CDL_Metadata;
				pCoreCDL->CDL_GetMetadata(&metadata);
				//CDL_Metadata* metadata2 = pCoreCDL->CDL_GetMetadata(metadata3);
				//CDL_Metadata *(CDL_API_CALL *pCDL_GetMetadata)(CDL_Metadata *) = pCoreCDL->CDL_GetMetadata;
				/*_asm {
					;Preserve registers
					push EAX
					push EDX
					;push EIP
					push ESP
					push ECX
					;mov eax,dword ptr [pCDL_GetMetadata] 
					call pCDL_GetMetadata 
					mov dword ptr [metadata2],edx 
					;Restore registers
					pop ECX
					pop ESP
					;pop EIP
					pop EDX
					pop EAX
				};*/
				//pCoreCDL->CDL_GetMetadata();
				//CDL_Metadata metadata = metadata2;
				// Format the song title
				std::string titleFormat = bar->GetSongFormat();
				std::string titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());
				//Make titleFormatUp.c_str()
				size_t pos = titleFormatUp.find("%ARTIST%");
				if (pos != titleFormatUp.npos)
					titleFormat.replace(pos, lstrlen("%ARTIST%"), ((metadata.Artist != NULL) && (lstrlenA(metadata.Artist) > 0)) ? metadata.Artist : "?");
				titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());

				pos = titleFormatUp.find("%TITLE%");
				if (pos != titleFormatUp.npos)
					titleFormat.replace(pos, lstrlen("%TITLE%"), ((metadata.Title != NULL) && (lstrlenA(metadata.Title) > 0)) ? metadata.Title : "?");
				titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());

				pos = titleFormatUp.find("%ALBUM%");
				if (pos != titleFormatUp.npos)
					titleFormat.replace(pos, lstrlen("%ALBUM%"), ((metadata.Album != NULL) && (lstrlenA(metadata.Album) > 0)) ? metadata.Album : "?");
				titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());

				pos = titleFormatUp.find("%GENRE%");
				if (pos != titleFormatUp.npos)
					titleFormat.replace(pos, lstrlen("%GENRE%"), ((metadata.Genre != NULL) && (lstrlenA(metadata.Genre) > 0)) ? metadata.Genre : "?");
				titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());

				pos = titleFormatUp.find("%CODEC%");
				if (pos != titleFormatUp.npos)
					titleFormat.replace(pos, lstrlen("%CODEC%"), ((metadata.Codec != NULL) && (lstrlenA(metadata.Codec) > 0)) ? metadata.Codec : "?");
				titleFormatUp = titleFormat;
				CharUpperBuffA((char *)titleFormatUp.c_str(), titleFormatUp.length());

				bar->SetSongTitle(titleFormat.c_str());
				//*/
				//const char *playingTitle = pCoreCDL->CDL_GetItemTitle(pCoreCDL->CDL_GetPlayingItem());
				//bar->SetSongTitle(playingTitle);
				//wchar_t buffer[1025];
				//mbstowcs(buffer, playingTitle, 1024);
				//bar->SetTooltipTitle(buffer);
				bar->UpdateTime(pCoreCDL->CDL_GetPosition()/1000, pCoreCDL->CDL_GetDuration()/1000);
			}
			break;
		}
		case CORE_FILECLOSED:
			
			break;
		case CORE_FILEPLAY:
			break;
		case CORE_FILEPAUSE:
			break;          
		case CORE_FILESTOP:
			break;
		case CORE_FULLSCREEN:
			break;
		case CORE_FILESEEK:
			break;
		case CORE_VIDEOSIZECHANGE:
			break;
	}
}

