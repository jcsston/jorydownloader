/*
 *  Video Squirrel, a cross-platform movie cataloger
 *
 *  video_squirrel.cpp
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
	\file video_squirrel.cpp
	\version \$Id$
	\brief a cross-platform movie cataloger
	\author Jory Stone            <jcsston @ toughguy.net>
*/

// Here are the defines for each file format support
#define AVI_SUPPORT
#define REALMEDIA_SUPPORT
#define MPEG_SUPPORT
#define MATROSKA_SUPPORT

#include "video_squirrel.h"
#include "images.h"

AppFrame *frame;

// program execution "starts" here
bool MainApp::OnInit()
{
	DEBUG(_T("MainApp::OnInit called"));

	// I only use PNG images so I don't need the other handlers
	wxImage::AddHandler(new wxPNGHandler);
	wxFileSystem::AddHandler(new wxZipFSHandler);

	frame = new AppFrame(_T("Video Squirrel"), wxPoint(50, 50), wxSize(450, 340));
	frame->Show(TRUE);

	/* success: wxApp::OnRun() will be called which will enter the main
	 * message loop and the application will run. If we returned FALSE
	 * here, the application would exit immediately.
	 */
	return TRUE;
};

/*
TAG_HANDLER_BEGIN(CLOSEBUTTON, "CLOSEBUTTON")

TAG_HANDLER_PROC(tag)
{
	DEBUG(_T("CLOSEBUTTON Tag Handler start"));
	wxButton *wnd;
	int fl = 0;

	wnd = new wxButton(m_WParser->GetWindow(), HTMLTag_CloseButton, tag.GetParam(_T("NAME")), wxPoint(0,0));
	wnd->Show(TRUE);

	m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(wnd, fl));
	
 	wxWindow *test = m_WParser->GetWindow();
 	if (test != NULL)
 	{
 		DEBUG(_T("window not NULL"));
 		wxWindow *good_window = test->GetParent();
 		if (good_window != NULL)
 		{
 			good_window->SetTitle(_T("found window"));
 		{
	}
	else
	{
		DEBUG(_T("window is NULL"));
	}
	DEBUG(_T("CLOSEBUTTON Tag Handler exit"));
	return FALSE;
}

TAG_HANDLER_END(CLOSEBUTTON)


TAG_HANDLER_BEGIN(VIEWBUTTON, "VIEWBUTTON")

TAG_HANDLER_PROC(tag)
{
	DEBUG(_T("VIEWBUTTON Tag Handler start"));
	wxButton *wnd;
	int fl = 0;

	wnd = new wxButton(m_WParser->GetWindow(), HTMLTag_ViewButton, tag.GetParam(wxT("NAME")), wxPoint(0,0));
	long *index = new long;
	tag.GetParam(wxT("INDEX")).ToLong(index);
	wnd->SetClientData((void *)index);

	wnd->Show(TRUE);

	m_WParser->GetContainer()->InsertCell(new wxHtmlWidgetCell(wnd, fl));

	DEBUG(_T("VIEWBUTTON Tag Handler exit"));
	return FALSE;
}


TAG_HANDLER_END(VIEWBUTTON)
	

TAGS_MODULE_BEGIN(MyBind)

	TAGS_MODULE_ADD(CLOSEBUTTON)
	TAGS_MODULE_ADD(VIEWBUTTON)

TAGS_MODULE_END(MyBind)
*/

MyHTMLDialog::MyHTMLDialog(wxWindow *parent_frame, const wxString &html_code, const wxString &title, const wxSize &size)
: wxFrame ((wxWindow *) parent_frame, -1, title, wxDefaultPosition, size)
{
	DEBUG(_T("MyHTMLDialog::MyHTMLDialog called"));
	wxHtmlWindow *html = NULL;
	html = new wxHtmlWindow(this);
	html->SetPage(html_code);
	this->Centre();
};

void MyHTMLDialog::OnHTMLClose(wxCommandEvent &event)
{
	DEBUG(_T("MyHTMLDialog::OnHTMLClose called"));
	//this->Hide();
	//this->Show(false);
	this->Close(TRUE);
};

// frame constructor
AppFrame::AppFrame(const wxString &title, const wxPoint &pos,
const wxSize& size)
: wxFrame ((wxFrame *) NULL, -1, title, pos, size)
{
	DEBUG(_T("AppFrame::AppFrame called"));

	/* "A" is the name of the application's main icon,
	 * as set by Dev-C++
	 */
	SetIcon(wxICON(A));

	//We create the VideoSquirrelConfiguration before creating the controls so we know what settings to use ;)
 	settings = new VideoSquirrelConfiguration();
 	new_options = NULL;
 	search_box = NULL;

	// create menus
	fileMenu = new wxMenu();
	fileMenu->Append(MainApp_MenuOpenDatabase, _T("Open &Database"), _T("Open a Database"));
	fileMenu->Append(MainApp_MenuSaveDatabase, _T("Save &Database"), _T("Save the current Database"));
	fileMenu->Append(MainApp_AddItem, _T("&Add File"), _T("Add file to database"));
	fileMenu->Append(MainApp_AddFolder, _T("Add &Folder or Drive"), _T("Add a whole folder or drive to the database"));
	fileMenu->AppendSeparator();
	fileMenu->Append(MainApp_MenuGenerateHTMLListing, _T("&Generate HTML Listing\tCtrl-G"), _T("Generate a HTML listing of all the videos in database"));
	fileMenu->AppendSeparator();
	fileMenu->Append(MainApp_Quit, _T("E&xit\tCtrl-Q"), _T("Quit this program"));
	
 	itemMenu = new wxMenu();
	itemMenu->Append(MainApp_MenuSearchDatabase, _T("&Search...\tCtrl-S"), _T("Show the search dialog"));
	
 	optionsMenu = new wxMenu();
	//optionsMenu->Append(MainApp_Open, _T("&Open\tCtrl-O"), _T("Open Video Squirrel XML file"));
	optionsMenu->Append(MainApp_ShowOptions, _T("&Options\tCtrl-O"), _T("Open up the options dialog"));
	
 	helpMenu = new wxMenu();
	helpMenu->Append(MainApp_AboutHelp, _T("&Help"), _T("Show the help dialog"));
	helpMenu->AppendSeparator();
	helpMenu->Append(MainApp_About, _T("&About...\tCtrl-A"), _T("Show about dialog"));

	menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(itemMenu, _T("&Item"));
	menuBar->Append(optionsMenu, _T("&Options"));
	menuBar->Append(helpMenu, _T("&Help"));

	SetMenuBar(menuBar);

	//Create the Panels
	splitter_window = new wxSplitterWindow(this, -1);
	panel_2 = new wxPanel(splitter_window, -1);
	panel_1 = new wxPanel(splitter_window, -1);

	database_list_view = new wxListCtrl(panel_1, MainApp_DatabaseListView, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	database_list_view->InsertColumn(0, _T("Title"));

	dnd_add = new DnDFile(database_list_view);
	database_list_view->SetDropTarget(dnd_add);

  notebook_item_views = new wxNotebook(panel_2, -1, wxDefaultPosition, wxDefaultSize, 0);
  notebook_item_view_pane_edit = new wxPanel(notebook_item_views, -1);
  notebook_item_view_pane_html = new wxPanel(notebook_item_views, -1);
  html_window_item_view = new wxHtmlWindow(notebook_item_view_pane_html, -1);
  //Setup the default html page
  html_window_item_view->SetPage(_T("<html><body><font size=+2><b>Video Item Details</b></font><hr><p>Select a item to display</p></body></html>"));
  
  list_ctrl_item_edit = new wxListCtrl(notebook_item_view_pane_edit, MainApp_ListViewItemEdit, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);

	splitter_window->SplitVertically(panel_1, panel_2, 200);

	wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);

  wxBoxSizer* sizer_edit_view = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizer_html_view = new wxBoxSizer(wxHORIZONTAL);

	sizer_2->Add(database_list_view, 1, wxEXPAND, 0);
	panel_1->SetAutoLayout(true);
	panel_1->SetSizer(sizer_2);
	sizer_2->Fit(panel_1);
	sizer_2->SetSizeHints(panel_1);

	sizer_html_view->Add(html_window_item_view, 1, wxEXPAND, 0);
  notebook_item_view_pane_html->SetAutoLayout(true);
  notebook_item_view_pane_html->SetSizer(sizer_html_view);
  sizer_html_view->Fit(notebook_item_view_pane_html);
  sizer_html_view->SetSizeHints(notebook_item_view_pane_html);
	notebook_item_views->AddPage(notebook_item_view_pane_html, _T("HTML View"));
  //sizer_edit_view->Add(list_ctrl_item_edit, 1, wxEXPAND, 0);
  //notebook_item_view_pane_edit->SetAutoLayout(true);
  //notebook_item_view_pane_edit->SetSizer(sizer_edit_view);
  //sizer_edit_view->Fit(notebook_item_view_pane_edit);
  //sizer_edit_view->SetSizeHints(notebook_item_view_pane_edit);  
  //notebook_item_views->AddPage(notebook_item_view_pane_edit, _T("Edit View (non-working)"));
  sizer_3->Add(new wxNotebookSizer(notebook_item_views), 1, wxEXPAND, 0);
	panel_2->SetAutoLayout(true);
	panel_2->SetSizer(sizer_3);
	sizer_3->Fit(panel_2);
	sizer_3->SetSizeHints(panel_2);

	sizer_1->Add(splitter_window, 1, wxEXPAND, 0);
	SetAutoLayout(true);
	SetSizer(sizer_1);
	sizer_1->SetMinSize(600, 400);
	sizer_1->Fit(this);
	sizer_1->SetSizeHints(this);
	Layout();

	// Setup database list
	parseXMLFile(settings->database_filename);
	// Update the list. not needed as the list updated as items are added to the database
	// I may add this later for sorting
	RefreshVideoList();

	// Create a statusbar
	CreateStatusBar(1);
	SetStatusText(wxString(_T("Using database file: ")) + settings->database_filename);		
};

SearchFrame::SearchFrame(wxWindow *parent, const wxString &title)
: wxFrame ((wxWindow *) parent, -1, title, wxDefaultPosition, wxSize(300,200))
{
 	DEBUG(_T("SearchFrame::SearchFrame called"));

	wxPanel *panel_1 = new wxPanel(this, -1);
	text_ctrl_search_string = new wxTextCtrl(panel_1, -1, _T(""));
	text_ctrl_search_string_backup = text_ctrl_search_string;
 	//DEBUG(this->text_ctrl_search_string->GetClassInfo()->GetClassName());
 	//DEBUG(this->text_ctrl_search_string->GetClassInfo()->GetBaseClassName1());
 	//DEBUG(wxString::Format(_T("Points to %i"), text_ctrl_search_string));
 	//DEBUG(wxString::Format(_T("Points to %i"), text_ctrl_search_string_backup));
 	//DEBUG(wxString::Format(_T("Location Pointer %i"), &text_ctrl_search_string));
 	//DEBUG(wxString::Format(_T("Location Pointer %i"), &text_ctrl_search_string_backup));

	button_start_search = new wxButton(panel_1, SearchFrame_SearchButton, _T("&Search"));

	button_start_search->SetDefault();

	wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBoxSizer* sizer_6 = new wxStaticBoxSizer(new wxStaticBox(panel_1, -1, _T("Search")), wxHORIZONTAL);
	sizer_6->Add(text_ctrl_search_string, 1, wxALL, 1);
	sizer_5->Add(sizer_6, 3, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 8);
	sizer_5->Add(button_start_search, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	panel_1->SetAutoLayout(true);
	panel_1->SetSizer(sizer_5);
	sizer_5->Fit(panel_1);
	sizer_5->SetSizeHints(panel_1);
	sizer_4->Add(panel_1, 1, wxEXPAND, 0);
	SetAutoLayout(true);
	SetSizer(sizer_4);
	sizer_4->Fit(this);
	sizer_4->SetSizeHints(this);
	Layout();
}


/*****************
 * event handlers
 *****************/

void SearchFrame::OnSearchFrame_SearchButton(wxCommandEvent &event)
{
	DEBUG(_T("SearchFrame::OnSearchFrame_SearchButton called"));
	wxBusyInfo wait(_T("Please wait, searching..."));
	
 	//DEBUG(frame->search_box->text_ctrl_search_string->GetClassInfo()->GetClassName());
 	//DEBUG(frame->search_box->text_ctrl_search_string->GetClassInfo()->GetBaseClassName1());
 	//DEBUG(frame->search_box->text_ctrl_search_string_backup->GetClassInfo()->GetClassName());
 	//DEBUG(frame->search_box->text_ctrl_search_string_backup->GetClassInfo()->GetBaseClassName1());


 	//DEBUG(wxString::Format(_T("Points to %i"), frame->search_box->text_ctrl_search_string));
 	//DEBUG(wxString::Format(_T("Points to %i"), frame->search_box->text_ctrl_search_string_backup));
 	//DEBUG(wxString::Format(_T("Location Pointer %i"), &frame->search_box->text_ctrl_search_string));
 	//DEBUG(wxString::Format(_T("Location Pointer %i"), &frame->search_box->text_ctrl_search_string_backup));
	
 	wxString search_string = text_ctrl_search_string->GetLineText(0);
	wxString html_report;
	VideoItem *current_item = NULL;
	
	std::vector<long> found_items;
	DEBUG(_T("Searching"));
	for (size_t d = 0; d < frame->m_Database.size(); d++)
	{
		current_item = (VideoItem *)&frame->m_Database.at(d);

		int found_index = current_item->title.Lower().Find(search_string.Lower().c_str());
		if (found_index != -1)
		{
			//We found it :)
			DEBUG(_T("Found matching item"));
			found_items.push_back(current_item->UID);
		}
	}	
	//We are done searching, so now we generate a search results page
	DEBUG(_T("Creating HTML search report"));
	html_report = _T("<html><body bgcolor=\"#ccffff\">");
	html_report += _T("<h2><img src=\"squirrel_search.png\">Video Squirrel Search Results</h2>");
	html_report += _T("<p><ul>");
	DEBUG(_T("Creating HTML search report list"));
	if (found_items.size() > 0)
	{
		int i = 0;
		for (i = 0; i < found_items.size(); i++)
		{
			DEBUG(_T("HTML search report list adding item"));
			html_report += wxString::Format(_T("<li><a href=\"#%i\">Title: "), found_items.at(i));
			
			current_item = frame->FindVideoItemByUID(found_items.at(i));
			if (current_item != NULL)
			{
				html_report += current_item->title;
				DEBUG(_T("HTML search report list item added"));
			}
			else
			{
				html_report += _T("Unable to get Title");
			}
			html_report += _T("</a></li>");
		}
		html_report += _T("</ul></p><hr>");

		DEBUG(_T("Creating HTML search report item tables"));
		for (i = 0; i < found_items.size(); i++)
		{
			DEBUG(_T("HTML search report list adding item table"));
			html_report += wxString::Format(_T("<a name=\"%i\"><br>"), found_items.at(i));
			html_report += _T("<table width=\"100%\" border=\"1\" bgcolor=\"#FFFFCC\">");

			current_item = frame->FindVideoItemByUID(found_items.at(i));
			if (current_item != NULL)
			{
				html_report += _T("<tr><td>Title: ");
				html_report += current_item->title;
				html_report += _T("</td><td>CD: ");
				html_report += current_item->cd;
				html_report += _T("</td></tr><tr><td>Filename: ");
				html_report += current_item->filename;
				html_report += _T("</td>");				

				html_report += _T("<td>");
				html_report += _T("Filesize: ") + Format_FileSize(current_item->file_size);
				html_report += _T("<br>Duration: ") + current_item->video.GetNiceDuration();
				html_report += _T("</td>");

				html_report += _T("</tr><tr>");

				html_report += _T("<td>Video Info:<br>");				
				

				html_report += current_item->video.compressor
					+ wxString::Format(_T(" %i x %i"), current_item->video.x, current_item->video.y)
					+ _T(" ")
					+ wxString::Format(_T("%.1f fps"), current_item->video.frame_rate)
					+ _T(" ")
					+ wxString::Format(_T("%i kbit/s"), current_item->video.avg_bitrate);

				html_report += _T("</td><td>");
				if (current_item->audio.size() > 0) {
					html_report += _T("First Audio Track:<br>");
					html_report += current_item->audio[0].compressor 
						+ _T(" ")
						+ wxString::Format(_T("%i channels"), current_item->audio[0].channels)
						+ _T(" ")
						+ wxString::Format(_T("%.2f Hz"), (float)current_item->audio[0].sample_rate/1000)
						+ _T(" ")
						+ wxString::Format(_T("%i kbit/s"), current_item->audio[0].avg_bitrate);
				}
				//html_report += _T("</td><td ALIGN=\"center\" VALIGN=\"bottom\">");
				//html_report += wxString::Format(_T("<viewbutton name=\"View\" index=\"%i\">"), found_items.at(i));
				html_report += _T("</td></tr>");
				DEBUG(_T("HTML search report list item table added"));
			}
			else
			{
				html_report += _T("<tr><td>Unable to get Record!</td></tr>");
			}
			html_report += _T("</table><br>");
		}
	}
	else
	{
		html_report += _T("<li>No items found!</li></ul>");
	}
	html_report += _T("<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>");
	html_report += _T("<hr><p align=\"right\">Generated by Video Squirrel ") _T(Video_Squirrel_Version);
	html_report += wxDateTime::Now().Format(_T("on %x at %X."));
	html_report += _T("</p></body></html>");

	DEBUG(_T("Displaying HTML search report"));
	
	DEBUG(html_report);	
	
	MyHTMLDialog *search_results = new MyHTMLDialog(NULL, html_report, _T("Search Results"), wxSize(525,400));
	search_results->Show();	
	
	DEBUG(_T("SearchFrame::OnSearchFrame_SearchButton exiting"));
	
	return;
};

void AppFrame::OnOpen(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnOpen called"));
	wxFileDialog dialog (this, _T("Load Video Squirrel XML Database file"), _T(""), _T(""), _T("XML Files (*.xml)|*.xml"), wxOPEN);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString input_file;
		input_file = dialog.GetPath();
		parseXMLFile(input_file);
	}
}


void AppFrame::OnAdd(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnAdd called"));
	wxFileDialog dialog (this, _T("Locate input files"), _T(""), _T(""), _T("All Files (*.*)|*.*"), wxOPEN);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString input_file;
		input_file = dialog.GetPath();
		AddFileToDatabase(input_file);
	}
}


void AppFrame::OnAddFolder(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnAddFolder called"));
	wxDirDialog dialog(this, _T("Locate folder or drive"), settings->last_added_folder, wxOPEN);

	if (dialog.ShowModal() == wxID_OK)
	{		
		wxString input_folder = dialog.GetPath();	
		settings->last_added_folder = input_folder;

		wxString cd_label = input_folder;
#ifdef WIN32
		TCHAR volume_label[1025];
		memset(volume_label, 0, 1024);
		GetVolumeInformation(cd_label.c_str(), volume_label, 1024/sizeof(TCHAR), NULL, NULL, NULL, NULL, 0);
		if (lstrlen(volume_label) > 0)
			cd_label = volume_label;
#elif USE_BORKED_WXWINDOWS_VOLUME
		wxFileName input_filename = input_folder;
		if (input_filename.GetVolume().length() > 0)
			cd_label = input_filename.GetVolume();
#endif

		wxTextEntryDialog *cd_title_dialog = new wxTextEntryDialog(this, _T("Enter the name of the Group you want this under."), _T("Enter Group name"), cd_label, wxOK | wxCENTRE);

		if (cd_title_dialog->ShowModal() == wxID_OK) 
		{
			AddFolderToDatabase(input_folder, cd_title_dialog->GetValue());
		}
	}
}


void AppFrame::AddFolderToDatabase(wxString &folder, wxString group_under)
{
	DEBUG(_T("AppFrame::AddFolderToDatabase called"));
	wxStringList list_of_files;
	wxString current_file;
	wxArrayString file_types;
	int num_of_files = 0;

	//Setup the file types to scan for
	file_types.Add(_T("*.avi"));
	file_types.Add(_T("*.mpg"));
	file_types.Add(_T("*.mpeg"));
	file_types.Add(_T("*.m1v"));
	file_types.Add(_T("*.m2v"));
	file_types.Add(_T("*.rm"));
	file_types.Add(_T("*.rmvb"));
	file_types.Add(_T("*.mkv"));

	int m = 0;
	for (m = 0; m < menuBar->GetMenuCount(); m++)
 		menuBar->EnableTop(m, false);
	//Do a loop for each file type in the list
	for (int current_file_type = 0; current_file_type < file_types.Count(); current_file_type++)
	{
		wxDir level_0_folder(folder);
		if (level_0_folder.IsOpened())
		{
			bool good_name = level_0_folder.GetFirst(&current_file, file_types.Item(current_file_type));
			while (good_name)
			{
				wxString curent_filename = folder;
				curent_filename += current_file;
				AddFileToDatabase(curent_filename, group_under);
				//Go to next item
				good_name = level_0_folder.GetNext(&current_file);
				//Let's give the user a chance to click
				wxYield();
				num_of_files++;
			}
		}
	}
	for (m = 0; m < menuBar->GetMenuCount(); m++)
		menuBar->EnableTop(m, true);
	
	SetStatusText(wxString::Format(_T("Done, added: %i files"), num_of_files));
}


void AppFrame::AddFileToDatabase(wxString &filename, wxString group_under)
{
	DEBUG(_T("AppFrame::AddFileToDatabase called"));
	DEBUG(_T("Opening:"));
	DEBUG(filename.c_str());
	
	SetStatusText(wxString(_T("Please wait, adding: ")) + filename);
	VideoItem new_item;
	new_item.filename = filename;
	new_item.cd = group_under;
	if (filename.Right(4).Lower() == _T(".avi"))
	{
		#ifdef AVI_SUPPORT
		avi_t *avi_infomation = NULL;
		avi_infomation = AVI_open_input_file((char *)((const char *)wxConvLibc.cWX2MB(filename.c_str())), 0);
		if (avi_infomation != NULL)
		{
			//Add the parsed infomation to our Video Item
			new_item.UID = CreateUID();
			new_item.title = wxFileNameFromPath(filename).BeforeLast('.');
			new_item.filename = filename;
			//Video info
			new_item.video.frame_rate = avi_infomation->fps;
			new_item.video.duration = (long)(avi_infomation->video_frames / avi_infomation->fps);
			new_item.video.compressor = wxString(avi_infomation->compressor, wxConvUTF8);
			if (!!stricmp(avi_infomation->compressor, avi_infomation->compressor2))
				new_item.video.compressor += _T(" - ") + wxString(avi_infomation->compressor2, wxConvUTF8);
			new_item.video.x = avi_infomation->width;
			new_item.video.y = avi_infomation->height;
			//Audio info
			unsigned long total_audio_bitrate = 0;
			for (int audio_track = 0; audio_track < avi_infomation->anum; audio_track++)
			{
				audioData audio;
				
				audio.SetNiceCompression(avi_infomation->wave_format_ex[audio_track]->w_format_tag);
				audio.sample_rate = avi_infomation->wave_format_ex[audio_track]->n_samples_per_sec;
				audio.channels = avi_infomation->wave_format_ex[audio_track]->n_channels;
				audio.avg_bitrate = avi_infomation->wave_format_ex[audio_track]->n_avg_bytes_per_sec / 124;
				total_audio_bitrate += audio.avg_bitrate;

				new_item.audio.push_back(audio);
			}
			//Currently I have a dummy video bitrate calculation
			wxFile source_file(filename.c_str());
			off_t file_length = source_file.Length();
			if (file_length > 0)
			{
				new_item.file_size = file_length;
				if ((avi_infomation->fps != 0) && (avi_infomation->video_frames != 0))
				{
					new_item.video.avg_bitrate = long(file_length / new_item.video.duration - total_audio_bitrate) / 128;
				}
			}
			else
			{
				new_item.file_size = 0;
			}
			AVI_close(avi_infomation);
			
			//Add item to database list
			AddVideoItemToDatabase(new_item);			
		}
		else
		{
			SetStatusText(_T("Unable to read AVI infomation."));
			return;
		}
		#else
		SetStatusText(_T("AVI support not compiled in"));
		#endif																				//AVI_SUPPORT
	}
	else if ((filename.Right(3).Lower() == _T(".rm")) || (filename.Right(5).Lower() == _T(".rmvb")))
	{
		#ifdef REALMEDIA_SUPPORT
		//A RealMedia file :D
		RealMedia_Reader rm_reader;
		rm_reader.Read((const char *)wxConvLibc.cWX2MB(filename.c_str()));

		new_item.UID = CreateUID();
		new_item.title = wxFileNameFromPath(filename).BeforeLast('.');
		new_item.filename = filename;
		new_item.comment_text = wxString(rm_reader.content_description_block[0]->comment, wxConvUTF8);
		
		int audio_stream_no = 0;
		for (int stream_no = 0; stream_no < rm_reader.properties_block.num_streams; stream_no++)
		{
			RealMedia_Media_Properties *current_stream = rm_reader.media_properties_block[stream_no];
			if (current_stream != NULL) {
				if (!strcmpi(current_stream->stream_name, "Video Stream"))
				{
					new_item.video.frame_rate = current_stream->frame_rate;
					new_item.video.duration = current_stream->duration / 1000;

					char fourcc[10];
					memset(fourcc, 0, 9);
					memcpy(fourcc, &current_stream->video_header->fcc2, 4);
					new_item.video.compressor = wxString(fourcc, wxConvUTF8);

					new_item.video.avg_bitrate = current_stream->avg_bit_rate / 1024;
					new_item.video.x = current_stream->video_header->w;
					new_item.video.y = current_stream->video_header->h;
				}
				else if (!strcmpi(current_stream->stream_name, "Audio Stream"))
				{
					audioData audio;
					
					char fourcc[10];
					memset(fourcc, 0, 9);
					memcpy(fourcc, &current_stream->audio_header->fourcc2, 4);

					audio.compressor = wxString(fourcc, wxConvUTF8);
					audio.avg_bitrate = current_stream->avg_bit_rate / 1024;

					if (current_stream->audio_header->version1 == 4) {
						rainfo4 *audio_header = (rainfo4 *)current_stream->audio_header;
						audio.channels = audio_header->channels;
						audio.sample_rate = audio_header->sample_rate;

					} else if (current_stream->audio_header->version1 == 5) {
						rainfo5 *audio_header = (rainfo5 *)current_stream->audio_header;
						audio.channels = audio_header->channels;
						audio.sample_rate = audio_header->sample_rate;
					}

					new_item.audio.push_back(audio);
				}
			}
		}

		wxFile source_file(filename.c_str());
		off_t file_length = source_file.Length();
		if (file_length > 0)
		{
			new_item.file_size = file_length;
		}
		else
		{
			new_item.file_size = 0;
		}
		
		//We are done so now add the item to the database
		AddVideoItemToDatabase(new_item);
		
		#else
		SetStatusText(_T("RealVideo support not compiled in"));
		#endif																				//REALMEDIA_SUPPORT
	}
	else if ((filename.Right(4).Lower() == _T(".mpg")) || (filename.Right(5).Lower() == _T(".mpeg"))  || (filename.Right(4).Lower() == _T(".m2v"))  || (filename.Right(4).Lower() == _T(".m1v")))
	{
		#ifdef MPEG_SUPPORT
		//A MPEG file :D
		CMPEGInfo mpeg_reader;

		mpeg_reader.LoadRawMPEG2((const char *)wxConvLibc.cWX2MB(filename.c_str()));
                            
		new_item.UID = CreateUID();
		new_item.title = wxFileNameFromPath(filename).BeforeLast('.');
		new_item.filename = filename; 
		new_item.video.x = mpeg_reader.GetWidth();
		new_item.video.y = mpeg_reader.GetHeight();
		new_item.video.frame_rate = mpeg_reader.GetFrameRate();
		new_item.video.duration = long(mpeg_reader.GetTotalFrames() / new_item.video.frame_rate);

		new_item.video.compressor = _T("MPEG Video");

		wxFile source_file(filename.c_str());
		off_t file_length = source_file.Length();
		if (file_length > 0)
		{
			new_item.file_size = file_length;
		}
		else
		{
			new_item.file_size = 0;
		}
		
		new_item.video.avg_bitrate = file_length / new_item.video.duration / 128;

		//We are done so now add the item to the database
		AddVideoItemToDatabase(new_item);
		
		#else
		SetStatusText(_T("MPEG support not compiled in"));
		#endif																				//MPEG_SUPPORT
	}
	else if ((filename.Right(4).Lower() == _T(".mkv")) || (filename.Right(4).Lower() == _T(".mka")))
	{
		#ifdef MATROSKA_SUPPORT
		//This is where some code goes ;P  	
		using namespace MatroskaUtilsNamespace;
   	
		MatroskaInfoParser new_file(filename.c_str());
		if (new_file.ParseFile() != 0)
		{
			SetStatusText(_T("Unable to read Matroska infomation."));
			return;
		}
		
		new_item.UID = CreateUID();
		new_item.title = wxString(new_file.GetTitle().GetUTF8().c_str(), wxConvUTF8);
		if (new_item.title.length() == 0)
			new_item.title = wxFileNameFromPath(filename).BeforeLast('.');

  	for (int t = 0; t < new_file.GetNumberOfTracks(); t++)
		{
			MatroskaTrackInfo *current_track = new_file.GetTrackInfo(t);
			if (current_track != NULL)
			{
				if (current_track->GetTrackType() == track_video) {
					new_item.video.compressor = wxString(current_track->GetCodecID(), wxConvUTF8);
					MatroskaVideoTrackInfo *video_track = current_track->GetVideoInfo();
					if (video_track != NULL)
					{
 			 			new_item.video.x = video_track->video_Width;
     			 	new_item.video.y = video_track->video_Height;
     			 	new_item.video.display_x = video_track->video_DisplayWidth;
     			 	new_item.video.display_y = video_track->video_DisplayHeight;
     			 	new_item.video.frame_rate = video_track->video_FrameRate;
 			    }				
				}else if (current_track->GetTrackType() == track_audio) {
									
					MatroskaAudioTrackInfo *audio_track = current_track->GetAudioInfo();
					if (audio_track != NULL)
					{
						audioData audio;
												
						audio.compressor = wxString(current_track->GetCodecID(), wxConvUTF8);
						audio.sample_rate = audio_track->audio_SampleRate;
						audio.channels = audio_track->audio_Channels;						

						new_item.audio.push_back(audio);
					}
				}
			}
		}

		new_item.video.duration = new_file.GetGlobalTrackInfo()->GetDuration();
		new_item.file_size = new_file.file_size;
		new_item.video.avg_bitrate = (int64)new_file.file_size / 1024 / new_file.GetGlobalTrackInfo()->GetDuration() * 8;
		
		
		//We are done so now add the item to the database
		AddVideoItemToDatabase(new_item);		
		#else
		SetStatusText(_T("Matroska support not compiled in"));
		#endif																				//MATROSKA_SUPPORT
	}
	else
	{
		DEBUG(_T("Failed to open:"));
		DEBUG(filename.c_str());
		SetStatusText(_T("File type not supported"));
		return;
	}
	SetStatusText(wxString(_T("Done, added: ")) + filename);
}


void AppFrame::OnMenuSearchDatabase(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnMenuSearchDatabase called"));
	//Time to SEARCH :D
	//wxString search_text = wxGetTextFromUser(_T("Enter the text to search for"));
	SearchFrame *search_box = new SearchFrame(this, _T("Search Database"));
	search_box->Show(TRUE);
};

int AppFrame::AddVideoItemToDatabase(VideoItem *new_item)
{
	if (new_item != NULL)
		return AddVideoItemToDatabase(*new_item);

	return -1;
}

int AppFrame::AddVideoItemToDatabase(VideoItem &new_item)
{
	DEBUG(_T("AppFrame::AddVideoItemToDatabase called"));

	m_DatabaseChanged = true;

	//Ok, we add this item to the database list in memory
	m_Database.push_back(new_item);
	//Now append this item to the Database ListView Control
	long new_item_index = 0;
	new_item_index = database_list_view->InsertItem(database_list_view->GetItemCount()+1, new_item.title);
	database_list_view->SetItemData(new_item_index, (long)new_item.UID);
	database_list_view->SetColumnWidth(0, -1);
	
	//If we got this far without crashing, We're doing great :)
	return 0;
};

long AppFrame::CreateUID()
{
	DEBUG(_T("AppFrame::CreateUID called"));
	long new_uid = 0;

	//Set the seed
	srand(time(0));
	new_uid = rand();
	//DEBUG(wxString::Format(_T("The largest random number created will be %i"), RAND_MAX).c_str());
	while (FindVideoItemByUID(new_uid) != NULL)
	{
		//Drats we already have this UID, create another
		new_uid = rand();
		//If we reach the limit of the rand() function, we will never exit this loop :(
		//but if the user has added 32000 something items she/he already has too many videos :P
	}
	return new_uid;
};


void AppFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnQuit called"));
	
	// TRUE is to force the frame to close
	Close(TRUE);
}

void AppFrame::OnMenuShowOptions(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnMenuShowOptions called"));
	
	delete new_options;
	new_options = new OptionsFrame(this);
	new_options->Show();
};

void AppFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnAbout called"));
 	//this->parseXMLFile(this->settings->database_filename);
	wxString msg;
	long real_total_size = sizeof(m_Database);
	for (size_t d = 0; d < m_Database.size(); d++)
	{
		VideoItem &current_item = m_Database.at(d);
		real_total_size += current_item.GetSize();
	}
	msg = _T("<html>");
	msg += _T("<body>");
	msg += _T("<font size=\"+2\"><b>Video Squirrel ") _T(Video_Squirrel_Version);
	msg += _T("</font></b><hr>");
	msg += _T("<p>A cross-platform video file cataloger.");
	msg += _T("<br>Written by Jory Stone jcsston@toughguy.net");
	msg += _T("<br>Uses wxWindows, TinyXml, avilib, CMPEGInfo,libebml and libmatroska.</p>");
	msg += wxString().Format(_T("<p>Total Size of database in memory: %i bytes</p>"), real_total_size);
	msg += _T("<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>");
	msg += _T("</body>");
	msg += _T("</html>");

	MyHTMLDialog *about_dialog = new MyHTMLDialog(this, msg, _T("About Video Squirrel"), wxSize(350,280));
	about_dialog->Show();
}


void AppFrame::OnAboutHelp(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnAboutHelp called"));

	MyHTMLDialog *help_dialog = new MyHTMLDialog(this, _T("<html><body TEXT=\"#000000\" BGCOLOR=\"#FFFBF0\">")
		_T("<h1><img src=\"squirrel_logo.png\">Video Squirrel Help</h1>")
		_T("<hr><p>Umm, I need to write this ;)</p>")
		_T("<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>")
		_T("</body></html>"), _T("Video Squirrel Help"));
	help_dialog->Show();
}

void AppFrame::OnMenuOpenDatabase(wxCommandEvent &event)
{
	wxFileDialog dialog (this, _T("Open a datebase"), _T(""), _T(""), _T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*"), wxOPEN);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString input_file;
		input_file = dialog.GetPath();
		settings->database_filename = input_file;
		parseXMLFile(input_file);
		RefreshVideoList();
	}
}

void AppFrame::OnMenuSaveDatabase(wxCommandEvent &event)
{
	wxFileDialog dialog (this, _T("Save datebase"), _T(""), _T(""), _T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*"), wxSAVE);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString input_file;
		input_file = dialog.GetPath();
		settings->database_filename = input_file;
		SaveDatabase();
	}
}

void AppFrame::OnMenuGenerateHTMLListing(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnMenuGenerateHTMLListing called"));
	
	VideoItem *current_item = NULL;
	wxString html_listing;
	html_listing = _T("<html><body><font size=+2><b>Squirrel Video Listing</b></font><hr>");
	for (size_t d = 0; d < m_Database.size(); d++)
	{
		VideoItem &current_item = m_Database.at(d);

		html_listing += CreateHTMLPage(&current_item, true);   
	}
	html_listing += _T("</body></html>");
	
	MyHTMLDialog *listing_dialog = new MyHTMLDialog(this, html_listing, _T("Video Squirrel HTML Listing"), wxSize(475, 400));
	listing_dialog->Show();
};

wxString AppFrame::CreateHTMLPage(VideoItem *item_to_display, bool fragment)
{
	DEBUG(_T("AppFrame::CreateHTMLPage called"));
	if (item_to_display != NULL)
	{
		wxString formated_html;
		wxString formated_html_audio;
		wxString formated_html_audio_track;
		
		//Check if this is to be a page in itself or a fragment
		if (!fragment)
		{
			formated_html += _T("<html><body>");
		}		
		//Copy the Item HTML template and get ready for search and replace ;)
  	formated_html += settings->html_item_view_template;
		
		formated_html.Replace(_T("$title$"), item_to_display->title);
		formated_html.Replace(_T("$cd$"), item_to_display->cd);
		formated_html.Replace(_T("$filename$"), item_to_display->filename);
		formated_html.Replace(_T("$filesize$"), Format_FileSize(item_to_display->file_size));
		formated_html.Replace(_T("$videobitrate$"), wxString::Format(_T("%i kbit/s"), item_to_display->video.avg_bitrate));
		formated_html.Replace(_T("$duration$"), item_to_display->video.GetNiceDuration());
		formated_html.Replace(_T("$framerate$"), wxString::Format(_T("%.1f"), item_to_display->video.frame_rate));
		formated_html.Replace(_T("$videopixelsize$"), wxString::Format(_T("%i x %i"), item_to_display->video.x, item_to_display->video.y));
		formated_html.Replace(_T("$videocompression$"), item_to_display->video.compressor);

		int audio_track = 0;
		for (audio_track = 0; audio_track < item_to_display->audio.size(); audio_track++)
		{
			//Copy the Audio track template to our local wxString for formating
   		formated_html_audio_track = settings->html_item_view_template_audio;

			formated_html_audio_track.Replace(_T("$audiotrack$"), wxString::Format(_T("%i"), audio_track+1));
			formated_html_audio_track.Replace(_T("$audiocompression$"), item_to_display->audio[audio_track].compressor);
			formated_html_audio_track.Replace(_T("$audiobitrate$"), wxString::Format(_T("%i kbit/s"), item_to_display->audio[audio_track].avg_bitrate));
			formated_html_audio_track.Replace(_T("$audiochannels$"), wxString::Format(_T("%i"), item_to_display->audio[audio_track].channels));
			formated_html_audio_track.Replace(_T("$audiosamplerate$"), wxString::Format(_T("%.1f Hz"), (float)item_to_display->audio[audio_track].sample_rate/1000));
			formated_html_audio += formated_html_audio_track;
		}
		
		formated_html.Replace(_T("$audio$"), formated_html_audio);
		
		formated_html.Replace(_T("$itemmemorysize$"), wxString::Format(_T("%u"), item_to_display->GetSize()));
  		
		if (!fragment)
		{
  		formated_html += _T("</body></html>");
		}		
		return formated_html;
	}
	return _T("<b>Invaild item passed to HTML generator</b>");
};

void AppFrame::OnListViewItemSelected(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnListViewItemSelected called"));
	
	int selected_item = 0;
	for (int i = 0; i < database_list_view->GetItemCount(); i++)
	{
		if (database_list_view->GetItemState(i, wxLIST_STATE_SELECTED) != 0)
		{
			selected_item = i;
			break;
  	}
	}
	long selected_item_uid = (long)database_list_view->GetItemData(selected_item);
	VideoItem *current_item = FindVideoItemByUID(selected_item_uid);
	wxASSERT_MSG(current_item != NULL, _T("A NULL ItemData was recieved from the ListCtrl"));
	if (current_item != NULL)
	{		
		html_window_item_view->SetPage(CreateHTMLPage(current_item));
	}
}


void AppFrame::CloseFrame(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::CloseFrame called"));
	
	if (m_DatabaseChanged && !SaveDatabase())
	{
		return;
	}
	delete settings;

	this->Destroy();
}


void AppFrame::OnHTMLViewItem(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnHTMLViewItem called"));

	wxButton *the_button = (wxButton *)event.GetEventObject();
	if (the_button != NULL)
	{
		DEBUG(_T("Getting button client data"));
		long *index = NULL;
		index = (long *)the_button->GetClientData();
  	if (index != NULL)
		{
			DEBUG(_T("Displaying item"));
			wxMessageBox(wxString::Format(_T("%i ID"), *index));
		}
	}
}


void AppFrame::RefreshVideoList()
{
	DEBUG(_T("AppFrame::RefreshVideoList called"));

	long current_item_index = 0;
	VideoItem *current_item = NULL;
	for (size_t d = 0; d < m_Database.size(); d++)
	{
		//Get the current item's data
		VideoItem *current_item = (VideoItem *)&m_Database.at(d);
		if (current_item != NULL) {
			current_item_index = database_list_view->InsertItem(database_list_view->GetItemCount()+1, current_item->title); 
 			database_list_view->SetItemData(current_item_index, (long)current_item->UID);
		}
 		
	}
}


void AppFrame::parseXMLFile(wxString filename)
{
	DEBUG(_T("AppFrame::parseXMLFile called"));

	//Tell the user that we are loading this, really big databases may take a while
	wxBusyInfo wait(_T("Please wait, loading database..."));

	// Clear the current database
	m_Database.clear();
	m_DatabaseChanged = false;

	// Build an XML tree from a file;
	TiXmlDocument xmlDatabase("");
	xmlDatabase.LoadFile(filename.mb_str());
	//Check if the XML tree was built
	if (xmlDatabase.Error()) {
		wxLogError(_T("TinyXML returned error code %i, \"%S\""), xmlDatabase.ErrorId(), xmlDatabase.ErrorDesc());
		return;
	}

	TiXmlElement *root_element = xmlDatabase.RootElement();
	if (root_element == NULL) {
		wxLogError(_T("TinyXML, failed to get root element, bad xml file?"));
		return;
		}
	if (!stricmp(root_element->Value(), "VideoSquirrelDatabase"))
	{
		TiXmlNode *level_0 = root_element->IterateChildren(NULL);
		while (level_0 != NULL)
		{
			if (!stricmp(level_0->Value(), "VideoList")) {
				//Good this is our document
				TiXmlNode *level_1 = level_0->IterateChildren(NULL);
				while (level_1 != NULL)
				{					
					if (!stricmp(level_1->Value(), "VideoItem")) {				
						// Create a new VideoItem
						VideoItem newItem;

						TiXmlNode *level_2 = level_1->IterateChildren(NULL);
						while (level_2 != NULL)
						{								
							if (!stricmp(level_2->Value(), "UID")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.UID = atol(level_2_data->Value());
								}
							}else if (!stricmp(level_2->Value(), "Title")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.title = wxString(level_2_data->Value(), wxConvUTF8);
								}
							}else if (!stricmp(level_2->Value(), "CD")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.cd = wxString(level_2_data->Value(), wxConvUTF8);
								}
							}else if (!stricmp(level_2->Value(), "Filename")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.filename = wxString(level_2_data->Value(), wxConvUTF8);
								}
							}else if (!stricmp(level_2->Value(), "Comment")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.comment_text = wxString(level_2_data->Value(), wxConvUTF8);
								}
							}else if (!stricmp(level_2->Value(), "FileSize")) {
								TiXmlNode *level_2_data = level_2->FirstChild();
								if (level_2_data != NULL) {
									newItem.file_size = atol(level_2_data->Value());
								}
							}else if (!stricmp(level_2->Value(), "VideoData")) {
								TiXmlNode *level_3 = level_2->IterateChildren(NULL);
								while (level_3 != NULL)
								{
									if (!stricmp(level_3->Value(), "VideoBitrate")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.avg_bitrate = atol(level_3_data->Value());
										}
									
									} else if (!stricmp(level_3->Value(), "PixelWidth")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.x = atol(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "PixelHeight")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.y = atol(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "VideoFrameRate")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.frame_rate = atof(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "VideoDuration")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.duration = atol(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "VideoCompressor")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newItem.video.compressor = wxString(level_3_data->Value(), wxConvUTF8);
										}
									}
									level_3 = level_2->IterateChildren(level_3);
								}	// while (level_3 != NULL)

							}else if (!stricmp(level_2->Value(), "AudioTrack")) {
								audioData newAudioData;
								TiXmlNode *level_3 = level_2->IterateChildren(NULL);
								while (level_3 != NULL)
								{
									if (!stricmp(level_3->Value(), "AudioBitrate")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newAudioData.avg_bitrate = atol(level_3_data->Value());
										}
									
									} else if (!stricmp(level_3->Value(), "AudioSampleRate")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newAudioData.sample_rate = atol(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "AudioChannelCount")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newAudioData.channels = atol(level_3_data->Value());
										}

									} else if (!stricmp(level_3->Value(), "AudioCompressor")) {
										TiXmlNode *level_3_data = level_3->FirstChild();
										if (level_3_data != NULL) {
											newAudioData.compressor = wxString(level_3_data->Value(), wxConvUTF8);
										}
									}
									level_3 = level_2->IterateChildren(level_3);
								}	// while (level_3 != NULL)
								newItem.audio.push_back(newAudioData);

							}else if (!stricmp(level_2->Value(), "Option")) {

							}
							level_2 = level_1->IterateChildren(level_2);
						} //while (level_2 != NULL)
						m_Database.push_back(newItem);
					}
					level_1 = level_0->IterateChildren(level_1);
				}
			}
			level_0 = root_element->IterateChildren(level_0);
		}
	}
				
	return;
}


bool AppFrame::SaveDatabase()
{
	DEBUG(_T("AppFrame::SaveDatabase called"));

	//Save the database to file
	wxBusyInfo wait(_T("Please wait, saving..."));

	//First open the xml output file for writing
	wxFileOutputStream output_file_stream(settings->database_filename);
	if (output_file_stream.Ok())
	{		
		//It opened ok, so now we bind it to a text output stream
		wxTextOutputStream xml_output_stream(output_file_stream);
		//A wxString buffer for composing larger lines
		wxString xml_buffer;
		xml_buffer = _T("<?xml version=\"1.0\"?>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("<!--Video Squirrel ") _T(Video_Squirrel_Version) _T(" Video File Database-->\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("<!--Written on ");
		xml_buffer += wxDateTime::Now().Format(_T("%x at %X"));
		xml_buffer += _T(" running on ") + wxGetOsDescription() + _T("-->\n");
		xml_output_stream.WriteString(xml_buffer);

		xml_buffer = _T("<VideoSquirrelDatabase>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("\t<VideoList>\n");
		xml_output_stream.WriteString(xml_buffer);
		//Go through the vector, writing each item
		for (size_t d = 0; d < m_Database.size(); d++)
		{
			//Get the current item's data
			VideoItem *current_item = (VideoItem *)&m_Database.at(d);
			xml_buffer = _T("\t\t<VideoItem>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<UID>");
			xml_buffer += wxString().Format(_T("%i"), current_item->UID);
			xml_buffer += _T("</UID>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<CD>");
			xml_buffer += MakeXMLNiceString(current_item->cd);
			xml_buffer += _T("</CD>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Title>");
			xml_buffer += MakeXMLNiceString(current_item->title);
			xml_buffer += _T("</Title>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Filename>");
			xml_buffer += MakeXMLNiceString(current_item->filename);
			xml_buffer += _T("</Filename>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<FileSize>");
			xml_buffer += wxString().Format(_T("%i"), current_item->file_size);
			xml_buffer += _T("</FileSize>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<VideoData>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<VideoBitrate>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.avg_bitrate);
			xml_buffer += _T("</VideoBitrate>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<PixelWidth>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.x);
			xml_buffer += _T("</PixelWidth>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<PixelHeight>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.y);
			xml_buffer += _T("</PixelHeight>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<VideoFrameRate>");
			xml_buffer += wxString().Format(_T("%f"), current_item->video.frame_rate);
			xml_buffer += _T("</VideoFrameRate>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<VideoDuration>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.duration);
			xml_buffer += _T("</VideoDuration>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<VideoCompressor>");
			xml_buffer += MakeXMLNiceString(current_item->video.compressor);
			xml_buffer += _T("</VideoCompressor>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t</VideoData>\n");
			xml_output_stream.WriteString(xml_buffer);

			size_t audio_track_no = 0;
			for (audio_track_no = 0; audio_track_no < current_item->audio.size(); audio_track_no++)
			{
				xml_buffer = _T("\t\t\t<AudioTrack>\n");
				xml_output_stream.WriteString(xml_buffer);
				xml_buffer = _T("\t\t\t\t<AudioCompressor>");
				xml_buffer += MakeXMLNiceString(current_item->audio[audio_track_no].compressor);
				xml_buffer += _T("</AudioCompressor>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t<AudioBitrate>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no].avg_bitrate);
				xml_buffer += _T("</AudioBitrate>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t<AudioChannelCount>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no].channels);
				xml_buffer += _T("</AudioChannelCount>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t<AudioSampleRate>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no].sample_rate);
				xml_buffer += _T("</AudioSampleRate>\n");
				xml_output_stream.WriteString(xml_buffer);
				
				xml_buffer = _T("\t\t\t</AudioTrack>\n");
				xml_output_stream.WriteString(xml_buffer);
			}

			xml_buffer = _T("\t\t\t<Comment>");
			xml_buffer += MakeXMLNiceString(current_item->comment_text);
			xml_buffer += _T("</Comment>\n");
			xml_output_stream.WriteString(xml_buffer);
			
			xml_buffer = _T("\t\t</VideoItem>\n");
			xml_output_stream.WriteString(xml_buffer);
		}
		xml_buffer = _T("\t</VideoList>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("</VideoSquirrelDatabase>");
		xml_output_stream.WriteString(xml_buffer);		

		m_DatabaseChanged = false;
	}
	else
	{		
		wxMessageDialog not_saved((wxWindow *)this, _T("Unable to write database file\nContinue?"), _T("Database not saved"), wxYES_NO);
		SetStatusText(_T("Unable to write database file"));
		if (not_saved.ShowModal() == wxID_NO)
		{
			return false;
		}
	}
	return true;
};

VideoItem *AppFrame::FindVideoItemByUID(long uid)
{
	//Go through the vector, writing each item
	for (size_t d = 0; d < m_Database.size(); d++)
	{
		//Get the current item's data
		VideoItem &current_item = m_Database.at(d);
		if (current_item.UID == uid)
			return &current_item;
	}
	return NULL;
}

bool DnDFile::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	wxString dnd_file;
	unsigned int i;
	int user_choice = 0;

	for(i=0;i<filenames.GetCount();i++)
	{
		dnd_file = filenames[i];
		wxMessageDialog add_file(frame, wxString::Format(_T("Add file %s to database?"), dnd_file.c_str()), _T("Drag-n-Dropped file"), wxYES_NO|wxCANCEL);
		user_choice = add_file.ShowModal();
		if (user_choice == wxID_YES)
		{
			frame->AddFileToDatabase(dnd_file);
		}
		else if (user_choice == wxID_CANCEL)
		{
			//If the user chooses Cancel we toss the filenames
			break;
		}
	}
	return true;
}


VideoItem::VideoItem()
{
	DEBUG(_T("VideoItem::VideoItem called"));

	UID = 0;
	file_size = 0;
};

VideoItem::~VideoItem()
{
	DEBUG(_T("VideoItem::~VideoItem called"));

};

unsigned int VideoItem::GetSize()
{
	unsigned int total_size = 0;
	total_size += sizeof(*this);
	size_t audio_track = 0;
	for (audio_track = 0; audio_track < audio.size(); audio_track++)
	{
		total_size += sizeof(audioData);
		audioData &audioItem = audio.at(audio_track);
		total_size += audioItem.compressor.length();
	}
	return total_size;
};

audioData::audioData()
{
	channels = 0;
	sample_rate = 0;
	avg_bitrate = 0;
};

const char *audioData::GetWaveFormatName(WORD formatTag)
{
	switch (formatTag)
	{
		case 0x0000: return "Microsoft Unknown Wave Format";
		case 0x0001: return "Microsoft PCM Format";
		case 0x0002: return "Microsoft ADPCM Format";
		case 0x0003: return "IEEE Float";
		case 0x0004: return "Compaq Computer VSELP (codec for Windows CE 2.0 devices)";
		case 0x0005: return "IBM CVSD";
		case 0x0006: return "Microsoft ALAW (CCITT A-Law)";
		case 0x0007: return "Microsoft MULAW (CCITT u-Law)";
		case 0x0008: return "Microsoft DTS";
		case 0x0010: return "OKI ADPCM";
		case 0x0011: return "Intel DVI ADPCM (IMA ADPCM)";
		case 0x0012: return "Videologic MediaSpace ADPCM";
		case 0x0013: return "Sierra Semiconductor ADPCM";
		case 0x0014: return "Antex Electronics G.723 ADPCM";
		case 0x0015: return "DSP Solutions DIGISTD";
		case 0x0016: return "DSP Solutions DIGIFIX";
		case 0x0017: return "Dialogic OKI ADPCM";
		case 0x0018: return "MediaVision ADPCM";
		case 0x0019: return "HP CU codec";
		case 0x0020: return "Yamaha ADPCM";
		case 0x0021: return "Speech Compression SONARC";
		case 0x0022: return "DSP Group True Speech";
		case 0x0023: return "Echo Speech EchoSC1";
		case 0x0024: return "Audiofile AF36";
		case 0x0025: return "APTX";
		case 0x0026: return "AudioFile AF10";
		case 0x0027: return "Prosody 1612 codec for CTI Speech Card";
		case 0x0028: return "Merging Technologies S.A. LRC";
		case 0x0030: return "Dolby Labs AC2";
		case 0x0031: return "Microsoft GSM 6.10";
		case 0x0032: return "MSNAudio";
		case 0x0033: return "Antex Electronics ADPCME";
		case 0x0034: return "Control Resources VQLPC";
		case 0x0035: return "DSP Solutions Digireal";
		case 0x0036: return "DSP Solutions DigiADPCM";
		case 0x0037: return "Control Resources Ltd CR10";
		case 0x0038: return "Natural MicroSystems VBXADPCM";
		case 0x0039: return "Roland RDAC (Crystal Semiconductor IMA ADPCM)";
		case 0x003A: return "Echo Speech EchoSC3";
		case 0x003B: return "Rockwell ADPCM";
		case 0x003C: return "Rockwell Digit LK";
		case 0x003D: return "Xebec Multimedia Solutions";
		case 0x0040: return "Antex Electronics G.721 ADPCM";
		case 0x0041: return "Antex Electronics G.728 CELP";
		case 0x0042: return "Microsoft MSG723";
		case 0x0043: return "IBM AVC ADPCM";
		case 0x0050: return "Microsoft MPEG-1 layer 1, 2";
		case 0x0052: return "InSoft RT24 (ACM codec is an alternative codec)";
		case 0x0053: return "InSoft PAC";
		case 0x0055: return "MPEG-1 Layer 3 (MP3)";
		case 0x0059: return "Lucent G.723";
		case 0x0060: return "Cirrus Logic";
		case 0x0061: return "ESS Technology ESPCM / Duck DK4 ADPCM";
		case 0x0062: return "Voxware file-mode codec / Duck DK3 ADPCM";
		case 0x0063: return "Canopus Atrac";
		case 0x0064: return "APICOM G.726 ADPCM";
		case 0x0065: return "APICOM G.722 ADPCM";
		case 0x0066: return "Microsoft DSAT";
		case 0x0067: return "Microsoft DSAT Display";
		case 0x0069: return "Voxware Byte Aligned (bitstream-mode codec)";
		case 0x0070: return "Voxware AC8 (Lernout & Hauspie CELP 4.8 kbps)";
		case 0x0071: return "Voxware AC10 (Lernout & Hauspie CBS 8kbps)";
		case 0x0072: return "Voxware AC16 (Lernout & Hauspie CBS 12kbps)";
		case 0x0073: return "Voxware AC20 (Lernout & Hauspie CBS 16kbps)";
		case 0x0074: return "Voxware MetaVoice (file and stream oriented)";
		case 0x0075: return "Voxware MetaSound (file and stream oriented)";
		case 0x0076: return "Voxware RT29HW";
		case 0x0077: return "Voxware VR12";
		case 0x0078: return "Voxware VR18";
		case 0x0079: return "Voxware TQ40";
		case 0x0080: return "Softsound";
		case 0x0081: return "Voxware TQ60";
		case 0x0082: return "Microsoft MSRT24 (ACM codec is an alternative codec)";
		case 0x0083: return "AT&T Labs G.729A";
		case 0x0084: return "Motion Pixels MVI MV12";
		case 0x0085: return "DataFusion Systems G.726";
		case 0x0086: return "DataFusion Systems GSM610";
		case 0x0088: return "Iterated Systems ISIAudio";
		case 0x0089: return "Onlive";
		case 0x0091: return "Siemens Business Communications SBC24";
		case 0x0092: return "Sonic Foundry Dolby AC3 SPDIF";
		case 0x0093: return "MediaSonic G.723";
		case 0x0094: return "Aculab 8KBPS";
		case 0x0097: return "ZyXEL ADPCM";
		case 0x0098: return "Philips LPCBB";
		case 0x0099: return "Studer Professional Audio AG Packed";
		case 0x00A0: return "Malden Electronics PHONYTALK";
		case 0x00ff: return "AAC";
		case 0x0100: return "Rhetorex ADPCM";
		case 0x0101: return "BeCubed Software IRAT";
		case 0x0111: return "Vivo G.723";
		case 0x0112: return "Vivo Siren";
		case 0x0123: return "Digital G.723";
		case 0x0125: return "Sanyo ADPCM";
		case 0x0130: return "Sipro Lab Telecom ACELP.net";
		case 0x0131: return "Sipro Lab Telecom ACELP.4800";
		case 0x0132: return "Sipro Lab Telecom ACELP.8V3";
		case 0x0133: return "Sipro Lab Telecom ACELP.G.729";
		case 0x0134: return "Sipro Lab Telecom ACELP.G.729A";
		case 0x0135: return "Sipro Lab Telecom ACELP.KELVIN";
		case 0x0140: return "Dictaphone G.726 ADPCM";
		case 0x0150: return "Qualcomm PUREVOICE";
		case 0x0151: return "Qualcomm HALFRATE";
		case 0x0155: return "Ring Zero Systems TUBGSM";
		case 0x0160: return "Windows Media Audio V1 / DivX audio (WMA)";
		case 0x0161: return "Windows Media Audio V2 / DivX audio (WMA) / Alex AC3 Audio";
		case 0x0200: return "Creative Labs ADPCM";
		case 0x0202: return "Creative Labs FastSpeech8";
		case 0x0203: return "Creative Labs FastSpeech10";
		case 0x0210: return "UHER informatic GmbH ADPCM";
		case 0x0216: return "Ulead DV ACM";
		case 0x0220: return "Quarterdeck";
		case 0x0230: return "I-link Worldwide ILINK VC";
		case 0x0240: return "Aureal Semiconductor RAW SPORT";
		case 0x0250: return "Interactive Products HSX";
		case 0x0251: return "Interactive Products RPELP";
		case 0x0260: return "Consistent Software CS2";
		case 0x0270: return "Sony ATRAC3 (SCX, same as MiniDisk LP2)";
		case 0x0300: return "Fujitsu TOWNS SND";
		case 0x0400: return "BTV Digital (Brooktree digital audio format)";
		case 0x0401: return "Intel Music Coder (IMC)";
		case 0x0450: return "QDesign Music";
		case 0x0680: return "AT&T Labs VME VMPCM";
		case 0x0681: return "AT&T Labs TPC";
		case 0x1000: return "Olivetti GSM";
		case 0x1001: return "Olivetti ADPCM";
		case 0x1002: return "Olivetti CELP";
		case 0x1003: return "Olivetti SBC";
		case 0x1004: return "Olivetti OPR";
		case 0x1100: return "Lernout & Hauspie codec";
		case 0x1101: return "Lernout & Hauspie CELP codec";
		case 0x1102: return "Lernout & Hauspie SBC codec";
		case 0x1103: return "Lernout & Hauspie SBC codec";
		case 0x1104: return "Lernout & Hauspie SBC codec";
		case 0x1400: return "Norris Communication";
		case 0x1401: return "ISIAudio";
		case 0x1500: return "AT&T Labs Soundspace Music Compression";
		case 0x181C: return "VoxWare RT24 speech codec";
		case 0x1FC4: return "NTC ALF2CD ACM";
		case 0x2000: return "FAST Multimedia AG DVM (Dolby AC3)";
		case 0x674f: return "Ogg Vorbis (mode 1)";
		case 0x676f: return "Ogg Vorbis (mode 1+)";
		case 0x6750: return "Ogg Vorbis (mode 2)";
		case 0x6770: return "Ogg Vorbis (mode 2+)";
		case 0x6751: return "Ogg Vorbis (mode 3)";
		case 0x6771: return "Ogg Vorbis (mode 3+)";
		case 0xFFFE: return "Extensible wave format (or Ogg Vorbis)";
		case 0xFFFF: return "In Development / Unregistered";
	}

	return NULL;
};

void audioData::SetNiceCompression(int format_tag)
{
	const char *wavFormat = GetWaveFormatName(format_tag);
	if (wavFormat == NULL) {
		DEBUG(wxString::Format(_T("Unknown Audio Format Tag: %i"), format_tag));
		compressor = wxString::Format(_T("Unknown 0x0%x"), format_tag);
	} else {
		compressor = wxString(wavFormat, wxConvUTF8);
	}
};
		
videoData::videoData()
{
	x = 0;
	y = 0;
	display_x = 0;
	display_y = 0;
	frame_rate = 0;
	duration = 0;
	avg_bitrate = 0;
};

wxString videoData::GetNiceDuration()
{
	return Format_Duration(duration);
};

OptionsFrame::OptionsFrame(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, _T("Video Squirrel Options"), pos, size, wxDEFAULT_FRAME_STYLE)
{
	DEBUG(_T("OptionsFrame::OptionsFrame called"));
  // begin wxGlade: OptionsFrame::OptionsFrame
  window_2 = new wxSplitterWindow(this, -1);
  window_2_pane_2 = new wxPanel(window_2, -1);
  window_2_pane_1 = new wxPanel(window_2, -1);
  
  //Setup the options tree
  tree_ctrl_options = new wxTreeCtrl(window_2_pane_1, OptionsFrame_Tree, wxDefaultPosition, wxDefaultSize);
  wxTreeItemId root_node = tree_ctrl_options->AddRoot(_T("Video Squirrel Options"));
  wxTreeItemId html_templates = tree_ctrl_options->AppendItem(root_node, _T("HTML Templates"));
  tree_ctrl_options->AppendItem(html_templates, _T("Base Template"));
  tree_ctrl_options->AppendItem(html_templates, _T("Audio Track Template"));
  
  tree_ctrl_options->Expand(root_node);
  tree_ctrl_options->Expand(html_templates);
  
  label_option_name = new wxStaticText(window_2_pane_2, -1, _T("Name of this option"));
  current_option_object = new wxStaticText(window_2_pane_2, -1, _T(""));
  label_option_info = new wxStaticText(window_2_pane_2, -1, _T("More infomation about this option."));

  // begin wxGlade: OptionsFrame::set_properties
  window_2->SplitVertically(window_2_pane_1, window_2_pane_2, 200);
  // end wxGlade

  // begin wxGlade: OptionsFrame::do_layout
  wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* sizer_6 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
  sizer_5->Add(tree_ctrl_options, 1, wxEXPAND, 0);
  window_2_pane_1->SetAutoLayout(true);
  window_2_pane_1->SetSizer(sizer_5);
  sizer_5->Fit(window_2_pane_1);
  sizer_5->SetSizeHints(window_2_pane_1);
  sizer_6->Add(label_option_name, 0, wxALL, 2);
  sizer_6->Add((wxWindow *)current_option_object, 1, wxALL|wxEXPAND, 1);
  sizer_6->Add(label_option_info, 0, wxALL|wxEXPAND, 5);
  window_2_pane_2->SetAutoLayout(true);
  window_2_pane_2->SetSizer(sizer_6);
  sizer_6->Fit(window_2_pane_2);
  sizer_6->SetSizeHints(window_2_pane_2);
  sizer_4->Add(window_2, 1, wxEXPAND, 0);
  SetAutoLayout(true);
  SetSizer(sizer_4);
 	sizer_4->SetMinSize(300, 200);
  sizer_4->Fit(this);
  sizer_4->SetSizeHints(this);
  Layout();
  // end wxGlade
  
 	DEBUG(wxString::Format(_T("Points to %i"), tree_ctrl_options));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &tree_ctrl_options));    
}

void OptionsFrame::OnTreeSelectionChange(wxCommandEvent &event)
{
	DEBUG(_T("OptionsFrame::OnTreeSelectionChange called"));
	tree_ctrl_options = frame->new_options->tree_ctrl_options;
	//current_option_object = frame->new_options->current_option_object;

 	DEBUG(wxString::Format(_T("Points to %i"), frame->new_options->tree_ctrl_options));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &frame->new_options->tree_ctrl_options));

 	//DEBUG(tree_ctrl_options->GetClassInfo()->GetClassName());
 	//DEBUG(tree_ctrl_options->GetClassInfo()->GetBaseClassName1());

	wxTreeItemId selected_tree_item = tree_ctrl_options->GetSelection();
	if (tree_ctrl_options->GetItemText(selected_tree_item) == _T("Base Template"))
	{
		delete frame->new_options->current_option_object;
		frame->new_options->current_option_object = new wxTextCtrl(window_2_pane_2, -1, _T("Base"));
	}
 	else if (tree_ctrl_options->GetItemText(selected_tree_item) == _T("Audio Track Template"))
	{
		delete frame->new_options->current_option_object;
		frame->new_options->current_option_object = new wxTextCtrl(window_2_pane_2, -1, _T("Audio"));
	}
};

VideoSquirrelConfiguration::VideoSquirrelConfiguration()
{
	DEBUG(_T("VideoSquirrelConfiguration::VideoSquirrelConfiguration called"));
	//Setup the wxConfig object
 	pConfig = new wxConfig(_T("Squirrel Video")); 
	pConfig->SetRecordDefaults(true);
	
	//Read all the values
	last_added_folder = pConfig->Read(_T("/Last Added Folder"), _T(""));

	database_filename = pConfig->Read(_T("/Database Filename"), wxGetHomeDir() + _T("/database.xml"));
	html_item_view_template = pConfig->Read(_T("/HTML Item View Template"),
		_T("<b><font size=+1>$title$</font></b><hr>")
		_T("<p><b>CD:</b> $cd$<br>")
		_T("<b>Filename:</b> $filename$<br>")
		_T("<b>File Size:</b> $filesize$<br>")
		_T("<b>Duration:</b> $duration$<br>")
		_T("<b>Video Bitrate:</b> $videobitrate$<br>")
		_T("<b>Video Pixel Size:</b> $videopixelsize$<br>")
		_T("<b>Framerate:</b> $framerate$<br>")
		_T("<b>Video Compression:</b> $videocompression$<br>")
		_T("<table><tr><td width=20px></td>")
	 	_T("<td>$audio$</td></tr></table>")
		_T("</p><p align=right><font size=-1>Item Size in Memory: $itemmemorysize$</font></p><hr>"));

  html_item_view_template_audio = pConfig->Read(_T("/HTML Item View Audio Track Template"),
	  _T("<b>Audio Track:</b> $audiotrack$<br>")
		_T("<b>Audio Compression:</b> $audiocompression$<br>")
		_T("<b>Bitrate:</b> $audiobitrate$<br>")
		_T("<b>Channels:</b> $audiochannels$<br>")
		_T("<b>Sample Rate:</b> $audiosamplerate$<br>"));
};

VideoSquirrelConfiguration::~VideoSquirrelConfiguration()
{
	DEBUG(_T("VideoSquirrelConfiguration::~VideoSquirrelConfiguration called"));

	pConfig->Write(_T("/Last Added Folder"), last_added_folder);
	pConfig->Write(_T("/Database Filename"), database_filename);		 
	pConfig->Write(_T("/HTML Item View Template"), html_item_view_template);
  pConfig->Write(_T("/HTML Item View Audio Track Template"), html_item_view_template_audio);

	delete pConfig;
};

wxString MakeXMLNiceString(const wxString &input) {	
	wxString outString;
	size_t i = 0;

	while(i < input.length())
	{
		wxChar c = input[i];

		if (c == _T('&') 
			&& i < (input.length() - 2 )
			&& input[i+1] == _T('#')
			&& input[i+2] == _T('x'))
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			while (i < input.length()) {
				outString.append(input.c_str() + i, 1);
				++i;
				if (input[i] == _T(';'))
					break;
			}
		}	else if ( c == _T('&') ) {
			outString.append(_T("&amp;"));
			++i;
		}	else if ( c == _T('<') ) {
			outString.append(_T("&lt;"));
			++i;
		}	else if ( c == _T('>') ) {
			outString.append(_T("&gt;"));
			++i;
		}	else if ( c == _T('\"') )	{
			outString.append(_T("&quot;"));
			++i;
		}	else if ( c == _T('\'') )	{
			outString.append(_T("&apos;"));
			++i;
		} else {
			wxChar realc = c;
			outString.append(1, realc);
			++i;
		}
	}

	return outString;
};

wxString Format_FileSize(int64 file_size)
{
	if (file_size < 1024) {
		return wxString::Format(_T("%u bytes"), file_size);

	}	else if (file_size < 1024 * 1024) {
		return wxString::Format(_T("%.2f KB"), (float)file_size / 1024);

	}	else if (file_size < 1024 * 1024 * 1024) {
		return wxString::Format(_T("%.2f MB"), (float)file_size / 1024 / 1024);

	}	else {
		return wxString::Format(_T("%.2f GB"), (float)file_size / 1024 / 1024 / 1024);
	}
	return _T("N/A");
};

wxString Format_Duration(long length)
{
	//The duration should be stored in seconds
	if (length < 60)
	{
		return wxString::Format(_T("%i seconds"), length);
	}
	else //if (length < 60 * 60)
	{
		return wxString::Format(_T("%i minutes %i seconds"), int(length / (int)60), length - int(length / 60)*60);
	}
	return _T("");
};
