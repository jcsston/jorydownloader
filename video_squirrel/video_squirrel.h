/*
 *  Video Squirrel, a cross-platform movie cataloger
 *
 *  video_squirrel.h
 *
 *  Copyright (C) Jory Stone - June 2003
 *      
 *  Video Squirrel is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  Video Squirrel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with Video Squirrel; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 */

/*!
	\file video_squirrel.h
	\version \$Id$
	\brief a cross-platform movie cataloger
	\author Jory Stone            <jcsston @ toughguy.net>
*/

#ifndef _VIDEO_SQUIRREL_H_
#define _VIDEO_SQUIRREL_H_

#define DEBUG(x) wxLogDebug(x);
#define Video_Squirrel_Version "v0.1"

//wxWindows includes
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/textdlg.h>
#include <wx/listctrl.h>
#include <wx/listbox.h>
#include <wx/listimpl.cpp>
#include <wx/treectrl.h>
#include <wx/grid.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/datetime.h>
#include <wx/wxhtml.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/splitter.h>
#include <wx/html/htmlwin.h>
#include <wx/html/m_templ.h>
#include <wx/image.h>
#include <wx/fs_zip.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/dnd.h>
#include <wx/statbmp.h>
#include <wx/memory.h>

//libxml2 includes
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifdef AVI_SUPPORT //< Avi Support compilied in
//avilib includes
#ifdef _MSC_VER
extern "C" {
#endif
#include "avilib/avilib.h"
#ifdef _MSC_VER
};
#endif
#endif

#ifdef REALMEDIA_SUPPORT
//realmedia_reader includes
#include "realmedia_reader.h"
#endif

#ifdef MPEG_SUPPORT
//MPEGInfo by Nic function includes
#include "MPEGInfo.h"
#endif

#ifdef MATROSKA_SUPPORT
//MatroskaInfoParser includes
#include "MatroskaUtils.h" // Win32 GCC don't work with these
#endif

class audioData
{
	public:
		audioData();
		void SetNiceCompression(int format_tag);
		
		wxString compression;
		unsigned int channels;
		unsigned short bit_depth;
		float sample_rate;
		unsigned int avg_bitrate;
};

class videoData
{
	public:
		videoData();
		wxString GetNiceDuration();
		
		wxString compressor;
		wxString compressor2;
		unsigned int x;
		unsigned int y;
		unsigned int display_x;
		unsigned int display_y;
		float frame_rate;
		unsigned long duration;
		unsigned int color_depth;
		unsigned int avg_bitrate;
};

class VideoItem
{
	public:
		VideoItem();
		~VideoItem();
		unsigned int GetSize();
		
		long UID;
  	wxString title;
		wxString filename;
		wxString cd;
		unsigned long file_size;
		audioData *audio[255];
		videoData video;
		wxString comment_text;
		VideoItem *next_item;
};

WX_DECLARE_LIST(VideoItem, VideoItemList);
WX_DEFINE_LIST(VideoItemList);

class VideoSquirrelConfiguration
{
	public:
	VideoSquirrelConfiguration();
	~VideoSquirrelConfiguration(); 	

	wxConfig *pConfig;	
	
	//Values read at a object creation and saved when exiting
	wxString html_item_view_template;
	wxString html_item_view_template_audio;
	wxString database_filename;
};

class MainApp : public wxApp
{
	public:
		virtual bool OnInit(void);
};

class DnDFile : public wxFileDropTarget
{
  public:
    DnDFile(wxListCtrl *pOwner) { m_pOwner = pOwner; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

  private:
    wxListCtrl *m_pOwner;
};

class MyHTMLDialog : public wxFrame
{
	public:
		MyHTMLDialog(wxWindow *parent_frame, const wxString &html_code, const wxString &title = _T("HTML Dialog"), const wxSize &size = wxSize(300, 300));

		//HTML Tag functions
		void OnHTMLClose(wxCommandEvent &event);

};

class SearchFrame : public wxFrame
{
	public:
		SearchFrame(wxWindow *parent, const wxString &title);

		void OnSearchFrame_SearchButton(wxCommandEvent &event);  

		private:
  	wxTextCtrl *text_ctrl_search_string;
  	wxTextCtrl *text_ctrl_search_string_backup;
    wxButton *button_start_search;
};

class OptionsFrame: public wxFrame {
public:
    // begin wxGlade: OptionsFrame::ids
    // end wxGlade

    OptionsFrame(wxWindow* parent, int id = -1, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);
		void OnTreeSelectionChange(wxCommandEvent &event);
		
protected:
    // begin wxGlade: OptionsFrame::attributes
    wxTreeCtrl* tree_ctrl_options;
    wxPanel* window_2_pane_1;
    wxStaticText* label_option_name;
    wxObject* current_option_object;
    wxStaticText* label_option_info;
    wxPanel* window_2_pane_2;
    wxSplitterWindow* window_2;
    // end wxGlade
};

class AppFrame : public wxFrame
{
	public:
		AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

		// event handlers
		void OnOpen(wxCommandEvent &event);
		void OnAdd(wxCommandEvent &event);
		void OnAddFolder(wxCommandEvent &event);
		void OnMenuGenerateHTMLListing(wxCommandEvent &event);
  	void OnMenuSearchDatabase(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnMenuShowOptions(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
		void OnAboutHelp(wxCommandEvent &event);
		
  	void OnListViewItemSelected(wxCommandEvent &event);
		void CloseFrame(wxCommandEvent &event);
		// HTML Tag functions
  	void OnHTMLViewItem(wxCommandEvent &event);
  
   	void RefreshVideoList();
	
  	//Database functions
  	/// Formats an VideoItem's data based on the global Item Template
  	wxString CreateHTMLPage(VideoItem *item_to_display, bool fragment = false);
		void AddFolderToDatabase(wxString &folder, wxString group_under = wxEmptyString);
		void AddFileToDatabase(wxString &filename, wxString group_under = wxEmptyString);
		int AddVideoItemToDatabase(VideoItem *new_item);
		long CreateUID();
		void parseXMLFile(wxString filename);
		bool SaveDatabase();
		
		SearchFrame *search_box;
		OptionsFrame *new_options;
		wxMenuBar *menuBar;
		wxMenu *fileMenu;
		wxMenu *itemMenu;
		wxMenu *optionsMenu;
		wxMenu *helpMenu;
  	wxSplitterWindow *splitter_window;
   	wxPanel *panel_1;
   	wxPanel *panel_2;
		DnDFile *dnd_add;
   	//wxListBox *item_list;
    wxListCtrl *database_list_view;
    //The Item View Notebook
    wxNotebook* notebook_item_views;
    //HTML View
    wxPanel* notebook_item_view_pane_html;
    wxHtmlWindow* html_window_item_view;
    //Edit View
    wxPanel* notebook_item_view_pane_edit;
    wxListCtrl* list_ctrl_item_edit;
    
   	//The configuration object
   	VideoSquirrelConfiguration *settings;
				
		VideoItemList *the_database;
	private:
		// any class wishing to process wxWindows events must use this macro
		DECLARE_EVENT_TABLE()
};

enum
{
	// menu items
	MainApp_Quit = 1,
	MainApp_Open,
	MainApp_AddItem,
	MainApp_MenuGenerateHTMLListing,
	MainApp_MenuSearchDatabase,
	MainApp_AddFolder,
	MainApp_DatabaseListView,
	MainApp_ListVideos,
	MainApp_GridItems,
	MainApp_ShowOptions,
	MainApp_About,
	MainApp_AboutHelp,
	MainApp_ListVideoInfo,
	MainApp_ListViewItemEdit,
	HTMLTag_CloseButton,
	HTMLTag_ViewButton,
	SearchFrame_SearchButton,
	OptionsFrame_Tree
};

/**********************************************************
 * Event tables and other macros for wxWindows
 **********************************************************
 * The event tables connect the wxWindows events with the
 * functions (event handlers) which process them. It can
 * be also done at run-time, but for the simple menu
 * events like this the static method is much simpler.
 *
 **********************************************************/

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
	EVT_MENU(MainApp_Open,  AppFrame::OnOpen)
	EVT_MENU(MainApp_AddItem,  AppFrame::OnAdd)
	EVT_MENU(MainApp_AddFolder, AppFrame::OnAddFolder)
	EVT_MENU(MainApp_MenuGenerateHTMLListing, AppFrame::OnMenuGenerateHTMLListing)
	EVT_MENU(MainApp_MenuSearchDatabase, AppFrame::OnMenuSearchDatabase)	
	EVT_MENU(MainApp_Quit, AppFrame::OnQuit)
	EVT_MENU(MainApp_ShowOptions, AppFrame::OnMenuShowOptions)
	EVT_MENU(MainApp_About, AppFrame::OnAbout)
	EVT_MENU(MainApp_AboutHelp, AppFrame::OnAboutHelp)
	EVT_LIST_ITEM_SELECTED(MainApp_DatabaseListView, AppFrame::OnListViewItemSelected)
	EVT_CLOSE(AppFrame::CloseFrame)

	EVT_BUTTON(HTMLTag_CloseButton, MyHTMLDialog::OnHTMLClose)
	EVT_BUTTON(HTMLTag_ViewButton, AppFrame::OnHTMLViewItem)	
	EVT_BUTTON(SearchFrame_SearchButton, SearchFrame::OnSearchFrame_SearchButton)		
	
	EVT_TREE_SEL_CHANGED(OptionsFrame_Tree, OptionsFrame::OnTreeSelectionChange)
END_EVENT_TABLE()

/*****************************************************************
 * Create a new application object: this macro will allow
 * wxWindows to create the application object during program
 * execution (it's better than using a static object for many
 * reasons) and also declares the accessor function wxGetApp()
 * which will return the reference of the right type (i.e. MyApp
 * and not wxApp)
 *
 *****************************************************************/

IMPLEMENT_APP(MainApp)
#endif //_VIDEO_SQUIRREL_H_

