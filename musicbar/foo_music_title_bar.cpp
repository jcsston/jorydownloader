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
#include "..\Foobar2000_SDK\foobar2000\SDK\foobar2000.h"

TitleBar *bar = NULL;

#define REG_KEY "Software\\foobar2000\\Music (Title)Bar\\"
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
	static char key_text[1025];
	DWORD size = 1024;

	ZeroMemory(key_text, MAX_PATH);
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

	RegCreateKeyExA(HKEY_CURRENT_USER, REG_KEY, 0, NULL, 0, KEY_WRITE, &sa, &key_handle, &state);

	DWORD size = lstrlenA(the_value)+1;
	RegSetValueExA(key_handle, value_key, 0, REG_SZ, (CONST BYTE*)the_value, size);
	
	//char err_key[1024];
	//FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, err_key, 1024, NULL); 

	RegCloseKey(key_handle);
};

class play_callback_music_title_bar : public play_callback, public TitleBar_Callback {
public:
	play_callback_music_title_bar() 
		: m_Bar(this) 
	{
		m_Bar.SetDllHandle(core_api::get_my_instance());
		m_Bar.SetPlayerWindow(core_api::get_main_window());
		m_Bar.Init();
		bar = &m_Bar;
	};
	virtual void on_playback_starting() {
	
	};
	virtual void on_playback_new_track(metadb_handle * track) {
		string8 str;
		track->handle_format_title(str, m_Bar.GetSongFormat(), NULL);
		m_Bar.SetSongTitle(str.get_ptr());
		track->handle_format_title(str, m_Bar.GetTooltipFormat(), NULL);
		m_Bar.SetTooltipTitle(string_utf16_from_utf8(str.get_ptr()).get_ptr());
		m_Bar.UpdateTime(0, track->handle_get_length());
	};
	virtual void on_playback_stop(play_control::stop_reason reason) {
	
	};
	virtual void on_playback_seek(double time) {
		
	};
	virtual void on_playback_pause(int state) {
	
	};
	///currently played file got edited
	virtual void on_playback_edited(metadb_handle * track) {
	
	};
	virtual void on_playback_dynamic_info(const file_info * info,bool b_track_change) {
	
	};
	/// called every second
	virtual void on_playback_time(metadb_handle * track,double val) {		
		m_Bar.UpdateTime(val, track->handle_get_length());
	};

	virtual int TitleBar_Pause() {
		//if (playback_core::is_playing()) {
			return menu_manager::run_command("Playback/Pause");
		//} else {
		//	return menu_manager::run_command("Playback/Play");
		//}
	};
	virtual int TitleBar_Previous() {
		return menu_manager::run_command("Playback/Previous");
	};
	virtual int TitleBar_Next() {
		return menu_manager::run_command("Playback/Next");
	};
	virtual int TitleBar_Random() {
		return menu_manager::run_command("Playback/Random");
	};
	/*virtual int TitleBar_Write(const char *name, DWORD value) {
		config_var_int::g_set_value(name, value);
		return 0;
	};
	virtual int TitleBar_WriteStr(const char *name, const char *value) {
		config_var_string::g_set_value(name, value);
		return 0;
	};
	virtual DWORD TitleBar_Read(const char *name, DWORD defaultValue) {		
		config_var_int *ptr = config_var_int::g_find(name);
		if (ptr) {
			defaultValue = ptr->get_value();
			ptr->service_release();
		}

		return defaultValue;
	};
	virtual const char *TitleBar_ReadStr(const char *name) {
		static string8 str("");
		config_var_string *ptr = config_var_string::g_find(name);
		if (ptr) {
			ptr->get_value(str);
			ptr->service_release();
		}		
		return str;
	};*/
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
protected:
	TitleBar m_Bar;
};

class config_music_title_bar : public config
{
	virtual HWND create(HWND parent)
	{
		if (bar == NULL)
			return NULL;
		HWND configWindow = bar->ShowConfig(parent);

		return configWindow;
	}
	virtual const char * get_name() {return "Music (Title)Bar";}
	virtual const char * get_parent_name() {return "Components";}
};

static service_factory_t<play_callback, play_callback_music_title_bar> foo;
static service_factory_single_t<config, config_music_title_bar> foo2;

DECLARE_COMPONENT_VERSION("Music (Title)Bar", "v" TITLE_BAR_VERSION, TitleBar::GetAboutText())
