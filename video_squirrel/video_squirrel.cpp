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
//#define MATROSKA_SUPPORT

#include "video_squirrel.h"
//#include "exe/squirrel_logo.xpm"

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


/*TAG_HANDLER_BEGIN(CLOSEBUTTON, "CLOSEBUTTON")

TAG_HANDLER_PROC(tag)
{
	DEBUG(_T("CLOSEBUTTON Tag Handler start"));
	wxButton *wnd;
	int fl = 0;

	wnd = new wxButton(m_WParser->GetWindow(), HTMLTag_CloseButton, tag.GetParam(wxT("NAME")), wxPoint(0,0));
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
	
TAGS_MODULE_END(MyBind)*/


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
	this->Hide();
	this->Show(false);
	//this->Close(TRUE);
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
	//SetIcon(wxICON(A));

	//We create the VideoSquirrelConfiguration before creating the controls so we know what settings to use ;)
 	settings = new VideoSquirrelConfiguration();
 	new_options = NULL;
 	search_box = NULL;

	// create menus
	fileMenu = new wxMenu();
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

	//list_tree = new wxTreeCtrl(panel_1, MainApp_TreeCtrl);
	//tree_root = list_tree->AddRoot(_T("Video Database"));
	//list_tree->Expand(tree_root);

	database_list_view = new wxListCtrl(panel_1, MainApp_DatabaseListView, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	database_list_view->InsertColumn(0, _T("Title"));

  //item_list = new wxListBox(panel_1, MainApp_ItemList);
  //item_list->Hide();

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
  sizer_edit_view->Add(list_ctrl_item_edit, 1, wxEXPAND, 0);
  notebook_item_view_pane_edit->SetAutoLayout(true);
  notebook_item_view_pane_edit->SetSizer(sizer_edit_view);
  sizer_edit_view->Fit(notebook_item_view_pane_edit);
  sizer_edit_view->SetSizeHints(notebook_item_view_pane_edit);
  notebook_item_views->AddPage(notebook_item_view_pane_html, _T("HTML View"));
  notebook_item_views->AddPage(notebook_item_view_pane_edit, _T("Edit View (non-working)"));
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
	the_database = new VideoItemList(wxKEY_INTEGER);
	// Open up our database file
	parseXMLFile(settings->database_filename);
	// Update the list
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
 	DEBUG(this->text_ctrl_search_string->GetClassInfo()->GetClassName());
 	DEBUG(this->text_ctrl_search_string->GetClassInfo()->GetBaseClassName1());
 	DEBUG(wxString::Format(_T("Points to %i"), text_ctrl_search_string));
 	DEBUG(wxString::Format(_T("Points to %i"), text_ctrl_search_string_backup));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &text_ctrl_search_string));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &text_ctrl_search_string_backup));

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
	
 	DEBUG(frame->search_box->text_ctrl_search_string->GetClassInfo()->GetClassName());
 	DEBUG(frame->search_box->text_ctrl_search_string->GetClassInfo()->GetBaseClassName1());
 	DEBUG(frame->search_box->text_ctrl_search_string_backup->GetClassInfo()->GetClassName());
 	DEBUG(frame->search_box->text_ctrl_search_string_backup->GetClassInfo()->GetBaseClassName1());


 	DEBUG(wxString::Format(_T("Points to %i"), frame->search_box->text_ctrl_search_string));
 	DEBUG(wxString::Format(_T("Points to %i"), frame->search_box->text_ctrl_search_string_backup));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &frame->search_box->text_ctrl_search_string));
 	DEBUG(wxString::Format(_T("Location Pointer %i"), &frame->search_box->text_ctrl_search_string_backup));
	
 	wxString search_string = text_ctrl_search_string->GetLineText(0);
	wxString html_report;
	VideoItem *current_item = NULL;

	wxVideoItemListNode *node = frame->the_database->GetFirst();
	wxArrayLong found_items;
	DEBUG(_T("Searching"));
	while (node)
	{
		current_item = (VideoItem *)node->GetData();

		int found_index = current_item->title.Lower().Find(search_string.Lower().c_str());
		if (found_index != -1)
		{
			//We found it :)
			DEBUG(_T("Found matching item"));
			found_items.Add(current_item->UID);
		}
		node = node->GetNext();
	}	
	//We are done searching, so now we generate a search results page
	DEBUG(_T("Creating HTML search report"));
	html_report = _T("<html><body bgcolor=\"#ccffff\">");
	html_report += _T("<h2><img src=\"squirrel_search.png\">Video Squirrel Search Results</h2>");
	html_report += _T("<p><ul>");
	DEBUG(_T("Creating HTML search report list"));
	if (found_items.GetCount() > 0)
	{
		for (int i = 0; i <= found_items.GetCount(); i++)
		{
			DEBUG(_T("HTML search report list adding item"));
			html_report += wxString::Format(_T("<li><a href=\"%i\">Title: "), found_items.Index(i));

			wxVideoItemListNode *node = frame->the_database->Find(found_items.Index(i));
			current_item = (VideoItem *)node->GetData();
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
		for (int i = 0; i <= found_items.GetCount(); i++)
		{
			DEBUG(_T("HTML search report list adding item table"));
			html_report += wxString::Format(_T("<a name=\"%i\"><br>"), found_items.Index(i));
			html_report += _T("<table width=\"100%\" border=\"1\" bgcolor=\"#FFFFCC\">");

			wxVideoItemListNode *node = frame->the_database->Find(found_items.Index(i));
			current_item = (VideoItem *)node->GetData();
			if (current_item != NULL)
			{
				html_report += _T("<tr><td>Title: ");
				html_report += current_item->title;
				html_report += _T("</td><td>CD: ");
				//on CD named
				html_report += _T("</td><td>Filename: ");
				html_report += current_item->filename;
				html_report += _T("</td></tr><tr><td> Video Info:");
				//compression, framesize, bitrate
				html_report += _T("</td><td>First Audio Track:");
				//compression, bitrate
				html_report += _T("</td><td ALIGN=\"center\" VALIGN=\"bottom\">");
				html_report += wxString::Format(_T("<viewbutton name=\"View\" index=\"%i\">"), found_items.Index(i));
				html_report += _T("</td></tr>");
				DEBUG(_T("HTML search report list item table added"));
			}
			else
			{
				html_report += _T("<tr><td>Unable to get Record!</td></tr>");
			}
			html_report += _T("</table>");
		}
	}
	else
	{
		html_report += _T("<li>No items found!</li></ul>");
	}
	html_report += _T("<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>");
	html_report += _T("<hr><p align=\"right\">Generated by Video Squirrel " Video_Squirrel_Version);
	html_report += wxDateTime::Now().Format(_T("on %x at %X."));
	html_report += _T("</p></body></html>");

	DEBUG(_T("Displaying HTML search report"));
	DEBUG(html_report);	
	MyHTMLDialog *search_results = new MyHTMLDialog(NULL, html_report, _T("Search Results"), wxSize(425,375));
	search_results->Show();
	return;
	DEBUG(_T("SearchFrame::OnSearchFrame_SearchButton exiting"));
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


void AppFrame::OnAdd(wxCommandEvent &WXUNUSED(event))
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


void AppFrame::OnAddFolder(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnAddFolder called"));
	wxDirDialog dialog(this, _T("Locate folder or drive"), _T(""), wxOPEN);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString input_folder;
		input_folder = dialog.GetPath();
		//wxTextEntryDialog *cd_title_dialog = new wxTextEntryDialog(NULL, _T("Enter the name of the Group you want this under."), _T("Enter Group name"), input_folder, wxOK | wxCENTRE);
		//cd_title_dialog->ShowModal();
		//wxMessageBox(_T("test"));
		AddFolderToDatabase(input_folder); //, cd_title_dialog->GetValue());
	}
}


void AppFrame::AddFolderToDatabase(wxString &folder, wxString group_under)
{
	DEBUG(_T("AppFrame::AddFolderToDatabase called"));
	wxStringList list_of_files;
	wxString *current_file = new wxString;
	wxArrayString file_types;
	int num_of_files = 0;

	//Setup the file types to scan for
	file_types.Alloc(3);														//< This should improve the speed a tiny bit
	file_types.Add(_T("*.avi"));
	file_types.Add(_T("*.rm"));
	file_types.Add(_T("*.mkv"));

	for (int m = 0; m < menuBar->GetMenuCount(); m++)
 		menuBar->EnableTop(m, false);
	//Do a loop for each file type in the list
	for (int current_file_type = 0; current_file_type < file_types.Count(); current_file_type++)
	{
		wxDir level_0_folder(folder);
		if (level_0_folder.IsOpened())
		{
			bool good_name = level_0_folder.GetFirst(current_file, file_types.Item(current_file_type));
			while (good_name)
			{
				wxString curent_filename = folder;
				curent_filename += *current_file;
				AddFileToDatabase(curent_filename, group_under);
				//Go to next item
				good_name = level_0_folder.GetNext(current_file);
				//Let's give the user a chance to click
				wxYield();
				num_of_files++;
			}
		}
	}
	for (int m = 0; m < menuBar->GetMenuCount(); m++)
		menuBar->EnableTop(m, true);
	
	SetStatusText(wxString::Format(_T("Done, added: %i files"), num_of_files));
}


void AppFrame::AddFileToDatabase(wxString &filename, wxString group_under)
{
	DEBUG(_T("AppFrame::AddFileToDatabase called"));
	DEBUG(_T("Opening:"));
	DEBUG(filename.c_str());
	
	SetStatusText(wxString(_T("Please wait, adding: ")) + filename);
	VideoItem *new_item = new VideoItem();
	if (filename.Right(4).Lower() == _T(".avi"))
	{
		#ifdef AVI_SUPPORT
		avi_t *avi_infomation = NULL;
		avi_infomation = AVI_open_input_file((char *)filename.c_str(), 0);
		if (avi_infomation != NULL)
		{
			//Add the parsed infomation to our Video Item
			new_item->UID = CreateUID();
			new_item->title = wxFileNameFromPath(filename).BeforeLast('.');
			new_item->filename = filename;
			//Video info
			new_item->video.frame_rate = avi_infomation->fps;
			new_item->video.duration = (long)(avi_infomation->video_frames / avi_infomation->fps);
			new_item->video.color_depth = avi_infomation->bitmap_info_header->bi_bit_count;
			new_item->video.compressor = wxString(avi_infomation->compressor, wxConvUTF8);
			new_item->video.compressor2 = wxString(avi_infomation->compressor2, wxConvUTF8);
			new_item->video.x = avi_infomation->width;
			new_item->video.y = avi_infomation->height;
			//Audio info
			unsigned long total_audio_bitrate = 0;
			for (int audio_track = 0; audio_track < avi_infomation->anum; audio_track++)
			{
				new_item->audio[audio_track] = new audioData();
				new_item->audio[audio_track]->SetNiceCompression(avi_infomation->wave_format_ex[audio_track]->w_format_tag);
				new_item->audio[audio_track]->sample_rate = avi_infomation->wave_format_ex[audio_track]->n_samples_per_sec;
				new_item->audio[audio_track]->channels = avi_infomation->wave_format_ex[audio_track]->n_channels;
				new_item->audio[audio_track]->avg_bitrate = avi_infomation->wave_format_ex[audio_track]->n_avg_bytes_per_sec / 124;
				total_audio_bitrate += new_item->audio[audio_track]->avg_bitrate;
				new_item->audio[audio_track]->bit_depth = avi_infomation->wave_format_ex[audio_track]->w_bits_per_sample;
			}
			//Currently I have a dummy video bitrate calculation
			wxFile source_file(filename.c_str());
			off_t file_length = source_file.Length();
			if (file_length > 0)
			{
				new_item->file_size = file_length;
				if ((avi_infomation->fps != 0) && (avi_infomation->video_frames != 0))
				{
					new_item->video.avg_bitrate = long(file_length / new_item->video.duration - total_audio_bitrate) / 128;
				}
			}
			else
			{
				new_item->file_size = 0;
			}
			AVI_close(avi_infomation);
			
			//Add item to database list
			AddVideoItemToDatabase(new_item);			
		}
		else
		{
			SetStatusText(_T("Unable to read AVI infomation."));
			delete new_item;
			new_item = NULL;
		}
		#else
		SetStatusText(_T("AVI support not compiled in"));
		#endif																				//AVI_SUPPORT
	}
	else if ((filename.Right(3).Lower() == _T(".rm")) || (filename.Right(4).Lower() == _T(".rvb")))
	{
		#ifdef REALMEDIA_SUPPORT
		//A RealMedia file :D
		RealMedia_Reader rm_reader;
		rm_reader.Read(filename.mb_str());

		new_item->UID = CreateUID();
		new_item->title = wxFileNameFromPath(filename).BeforeLast('.');
		new_item->filename = filename;
		new_item->comment_text = wxString(rm_reader.content_description_block[0]->comment, wxConvUTF8);
		//Since RealMedia files don't have a framerate
		new_item->video.frame_rate = 30;
		new_item->video.duration = rm_reader.properties_block[0]->duration / 1000;

		new_item->video.compressor = wxString(rm_reader.media_properties_block[0]->mime_type, wxConvUTF8);
		new_item->video.avg_bitrate = rm_reader.properties_block[0]->avg_bit_rate / 1000;

		int stream_no = 1;
		while (rm_reader.media_properties_block[stream_no] != NULL)
		{
			new_item->audio[stream_no] = new audioData();
			new_item->audio[stream_no]->avg_bitrate = rm_reader.media_properties_block[stream_no]->avg_bit_rate / 1000;
			stream_no++;
		}

		wxFile source_file(filename.c_str());
		off_t file_length = source_file.Length();
		if (file_length > 0)
		{
			new_item->file_size = file_length;
		}
		else
		{
			new_item->file_size = 0;
		}
		
		//We are done so now add the item to the database
		AddVideoItemToDatabase(new_item);
		
		#else
		SetStatusText(_T("RealVideo support not compiled in"));
		#endif																				//REALMEDIA_SUPPORT
	}
	else if ((filename.Right(4).Lower() == _T(".mpg")) || (filename.Right(5).Lower() == _T(".mpeg")))
	{
		#ifdef MPEG_SUPPORT
		//A MPEG file :D
		CMPEGInfo mpeg_reader;

		mpeg_reader.LoadRawMPEG2(filename.mb_str());
                            
		new_item->UID = CreateUID();
		new_item->title = wxFileNameFromPath(filename).BeforeLast('.');
		new_item->filename = filename; 
		new_item->video.x = mpeg_reader.GetWidth();
		new_item->video.y = mpeg_reader.GetHeight();
		new_item->video.frame_rate = mpeg_reader.GetFrameRate();
		new_item->video.duration = long(mpeg_reader.GetTotalFrames() / new_item->video.frame_rate);

		new_item->video.compressor = _T("MPEG Video");
		//new_item->video.avg_bitrate = rm_reader.properties_block[0]->avg_bit_rate / 1000;

		/*int stream_no = 1;
		while (rm_reader.media_properties_block[stream_no] != NULL)
		{
			new_item->audio[stream_no] = new audioData();
			new_item->audio[stream_no]->avg_bitrate = rm_reader.media_properties_block[stream_no]->avg_bit_rate / 1000;
			stream_no++;
		}*/

		wxFile source_file(filename.c_str());
		off_t file_length = source_file.Length();
		if (file_length > 0)
		{
			new_item->file_size = file_length;
		}
		else
		{
			new_item->file_size = 0;
		}
		
		//We are done so now add the item to the database
		AddVideoItemToDatabase(new_item);
		
		#else
		SetStatusText(_T("MPEG support not compiled in"));
		#endif																				//MPEG_SUPPORT
	}
	else if ((filename.Right(3).Lower() == _T(".mkv")) || (filename.Right(4).Lower() == _T(".mka")))
	{
		#ifdef MATROSKA_SUPPORT
		//This is where some code goes ;P

		#else
		SetStatusText(_T("Matroska support not compiled in"));
		#endif																				//MATROSKA_SUPPORT
	}
	else
	{
		DEBUG(_T("Failed to open:"));
		DEBUG(filename.c_str());
		SetStatusText(_T("File type not supported"));
		delete new_item;
		new_item = NULL;
	}
	SetStatusText(wxString(_T("Done, added: ")) + filename);
}


void AppFrame::OnMenuSearchDatabase(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnMenuSearchDatabase called"));
	//Time to SEARCH :D
	search_box = new SearchFrame(this, _T("Search Database"));
	search_box->Show(TRUE);
};

int AppFrame::AddVideoItemToDatabase(VideoItem *new_item)
{
	DEBUG(_T("AppFrame::AddVideoItemToDatabase called"));

	//Like always we first check if some idot tried to add a NULL item
	if (new_item != NULL)
	{
		//Ok, we add this item to the database list in memory
		the_database->Append(new_item->UID, new_item);
		//Now append this item to the Database ListView Control
	 	long new_item_index = 0;
	  new_item_index = database_list_view->InsertItem(database_list_view->GetItemCount()+1, new_item->title);
		database_list_view->SetItemData(new_item_index, (long)new_item);
		database_list_view->SetColumnWidth(0, -1);
	}
	else
	{
		//Ok somebody did try to add a NULL item
		//We return -1 to signal an error
		return -1;
	}
	
	//If we got this far without crashing, We're doing great :)
	return 0;
};

long AppFrame::CreateUID()
{
	DEBUG(_T("AppFrame::CreateUID called"));
	long new_uid = 0;

	//Set the seed
	srand(time(0));
	DEBUG(wxString::Format(_T("The largest random number created will be %i"), RAND_MAX).c_str());
	while (the_database->Find(new_uid) != NULL)
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
 	
	wxString msg;
	unsigned long real_total_size = sizeof(the_database);
	VideoItem *current_item = NULL;
	wxVideoItemListNode *node = the_database->GetFirst();
	while (node)
	{
		current_item = (VideoItem *)node->GetData();
		real_total_size += current_item->GetSize();
		node = node->GetNext();
	}
	msg = _T("<html>");
	msg += _T("<body>");
	msg += _T("<font size=\"+2\"><b>Video Squirrel " Video_Squirrel_Version);
	msg += _T("</font></b><hr>");
	msg += _T("<p>A cross-platform video file cataloger.");
	msg += _T("<br>Written by Jory Stone jcsston@toughguy.net");
	msg += _T("<br>Uses wxWindows, libxml2, avilib, ,libebml and libmatroska.</p>");
	msg += wxString().Format(_T("<p>Total Size of database in memory: %u bytes</p>"), real_total_size);
	msg += _T("<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>");
	msg += _T("</body>");
	msg += _T("</html>");

	MyHTMLDialog *about_dialog = new MyHTMLDialog(this, msg, _T("About Video Squirrel"), wxSize(350,280));
	about_dialog->Show();
}


void AppFrame::OnAboutHelp(wxCommandEvent &WXUNUSED(event))
{
	DEBUG(_T("AppFrame::OnAboutHelp called"));

	MyHTMLDialog *help_dialog = new MyHTMLDialog(this, _T("<html><body TEXT=\"#000000\" BGCOLOR=\"#FFFBF0\">"
		"<h1><img src=\"squirrel_logo.png\">Video Squirrel Help</h1>"
		"<hr><p>Umm, I need to write this ;)</p>"
		"<p align=\"right\"><CLOSEBUTTON name=\"Close\"></p>"
		"</body></html>"), _T("Video Squirrel Help"));
	help_dialog->Show();
}

void AppFrame::OnMenuGenerateHTMLListing(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::OnMenuGenerateHTMLListing called"));
	
	VideoItem *current_item = NULL;
	wxVideoItemListNode *node = the_database->GetFirst();
	wxString html_listing;
	html_listing = _T("<html><body><font size=+2><b>Squirrel Video Listing</b></font><hr>");
	while (node)
	{
		current_item = (VideoItem *)node->GetData();

		html_listing += CreateHTMLPage(current_item, true);
   		
		node = node->GetNext();
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
		
		formated_html.Replace(_T("%title%"), item_to_display->title);
		formated_html.Replace(_T("%cd%"), item_to_display->cd);
		formated_html.Replace(_T("%filename%"), item_to_display->filename);
		formated_html.Replace(_T("%filesize%"), wxString::Format(_T("%i KB"), item_to_display->file_size / 1024));
		formated_html.Replace(_T("%videobitrate%"), wxString::Format(_T("%i kbit/s"), item_to_display->video.avg_bitrate));
		formated_html.Replace(_T("%duration%"), item_to_display->video.GetNiceDuration());
		formated_html.Replace(_T("%framerate%"), wxString::Format(_T("%.1f"), item_to_display->video.frame_rate));
		formated_html.Replace(_T("%videopixelsize%"), wxString::Format(_T("%i x %i"), item_to_display->video.x, item_to_display->video.y));
		formated_html.Replace(_T("%videocompression%"), item_to_display->video.compressor);
		formated_html.Replace(_T("%videocompression2%"), item_to_display->video.compressor2);
		formated_html.Replace(_T("%videocolordepth%"), wxString::Format(_T("%i"), item_to_display->video.color_depth));

		int audio_track = 0;
		while (item_to_display->audio[audio_track] != NULL)
		{
			//Copy the Audio track template to our local wxString for formating
   		formated_html_audio_track = settings->html_item_view_template_audio;

			formated_html_audio_track.Replace(_T("%audiotrack%"), wxString::Format(_T("%i"), audio_track+1));
			formated_html_audio_track.Replace(_T("%audiocompression%"), item_to_display->audio[audio_track]->compression);
			formated_html_audio_track.Replace(_T("%audiobitrate%"), wxString::Format(_T("%i kbit/s"), item_to_display->audio[audio_track]->avg_bitrate));
			formated_html_audio_track.Replace(_T("%audiochannels%"), wxString::Format(_T("%i"), item_to_display->audio[audio_track]->channels));
			formated_html_audio_track.Replace(_T("%audiosamplerate%"), wxString::Format(_T("%.1f Hz"), item_to_display->audio[audio_track]->sample_rate/1000));
			//formated_html_audio_track.Replace(_T("%audiobitdepth%"), wxString::Format(_T("%i"), item_to_display->audio[audio_track]->bit_depth));
			audio_track++;
			formated_html_audio += formated_html_audio_track;
		}
		
		formated_html.Replace(_T("%audio%"), formated_html_audio);
		
		formated_html.Replace(_T("%itemmemorysize%"), wxString::Format(_T("%u"), item_to_display->GetSize()));
  		
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
	VideoItem *selected_item_data = (VideoItem *)database_list_view->GetItemData(selected_item);
	wxASSERT_MSG(selected_item_data != NULL, _T("A NULL ItemData was recieved from the ListCtrl"));
	if (selected_item_data != NULL)
	{		
		html_window_item_view->SetPage(CreateHTMLPage(selected_item_data));
	}
}


void AppFrame::CloseFrame(wxCommandEvent &event)
{
	DEBUG(_T("AppFrame::CloseFrame called"));
	if (!SaveDatabase())
	{
		return;
	}
	delete settings;
	delete the_database;
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

	VideoItem *current_item = NULL;
	wxVideoItemListNode *node = the_database->GetFirst();
	long current_item_index = 0;
	while (node)
	{
		current_item = (VideoItem *)node->GetData();
 		current_item_index = database_list_view->InsertItem(database_list_view->GetItemCount()+1, current_item->title); 
 		database_list_view->SetItemData(current_item_index, (long)current_item);
 		
		node = node->GetNext();
	}
}


void AppFrame::parseXMLFile(wxString filename)
{
	DEBUG(_T("AppFrame::parseXMLFile called"));

	//Tell the user that we are loading this, really big databases may take a while
	wxBusyInfo wait(_T("Please wait, loading database..."));

	xmlDocPtr doc;
	xmlNsPtr ns;
	xmlNodePtr level_0, level_1, level_2, level_3;

	// Build an XML tree from a the file;
	//DEBUG(_T("Trying to open Doc"));
	doc = xmlParseFile(filename.mb_str());
	//DEBUG(_T("Doc opened"));
	//Check if the XML tree was built
	if (doc == NULL)
		return;

	//DEBUG(_T("Checking doc type"));
	level_0 = xmlDocGetRootElement(doc);
	// Now check if the
	if (level_0 == NULL)
	{
		DEBUG(_T("empty xml document"));
		xmlFreeDoc(doc);
		return;
	}
	//DEBUG(_T("Checking root node"));
	// Check the document is of the right kind
	if (xmlStrcmp(level_0->name, (const xmlChar *) "VideoSquirrelDatabase"))
	{
		DEBUG(_T("document of the wrong type, root node"));
		DEBUG(wxString((char *)level_0->name, wxConvUTF8));
		xmlFreeDoc(doc);
		return;
	}

	// Allocate the structure to be returned.

	// Now, walk the tree.
	// First level we expect is just VideoList
	//DEBUG(_T("Walking tree"));
	level_0 = level_0->xmlChildrenNode;
	while (level_0 && xmlIsBlankNode(level_0))
	{
		//DEBUG(_T("Passing Blank Node"));
		level_0 = level_0->next;
	}
	if (level_0 == 0)
		return;

	//DEBUG(_T("Checking for VideoList node"));
	if ((xmlStrcmp(level_0->name, (const xmlChar *) "VideoList")))
	{
		DEBUG(_T("VideoList node not found,"));
		DEBUG(wxString((char *)level_0->name, wxConvUTF8));
		xmlFreeDoc(doc);
		return;
	}
	//DEBUG(_T("Starting second level walk"));
	// Second level is a list of Videos
	level_0 = level_0->xmlChildrenNode;
	while (level_0 != NULL)
	{
		if ((!xmlStrcmp(level_0->name, (const xmlChar *) "VideoItem")))
		{
			//DEBUG(_T("Found VideoItem Node"));
			//Create a item in memory to store the data from the xml file
			VideoItem *loaded_item = new VideoItem();

			level_1 = level_0->xmlChildrenNode;
			while ((level_1 != NULL))
			{
				//DEBUG(_T("Walking VideoItem Node tree"));

				if ((!xmlStrcmp(level_1->name, (const xmlChar *) "UID")))
				{
					//DEBUG(_T("UID node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->UID = atol((char *)node_text);
					}
				}				
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "CD")))
				{
					//DEBUG(_T("CD node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->cd = wxString((char *)node_text, wxConvUTF8);
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "Filename")))
				{
					//DEBUG(_T("Filename node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->filename = wxString((char *)node_text, wxConvUTF8);
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "FileSize")))
				{
					//DEBUG(_T("Filename node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->file_size = atol((char *)node_text);
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "Title")))
				{
					//DEBUG(_T("Title node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->title = wxString((char *)node_text, wxConvUTF8);
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "Comment")))
				{
					//DEBUG(_T("Comment node found"));
					unsigned char *node_text = xmlNodeListGetString(doc, level_1->xmlChildrenNode, 1);
					if (node_text != NULL)
					{
						loaded_item->comment_text = wxString((char *)node_text, wxConvUTF8);
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "VideoData")))
				{
					level_2 = level_1->xmlChildrenNode;
					while ((level_2 != NULL))
					{
						//DEBUG(_T("Walking VideoItem->VideoData Node tree"));
						if ((!xmlStrcmp(level_2->name, (const xmlChar *) "PixelWidth")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.x = atol((char *)node_text);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "PixelHeight")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.y = atol((char *)node_text);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "Bitrate")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.avg_bitrate = atol((char *)node_text);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "ColorDepth")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.color_depth = atol((char *)node_text);
							}

						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "VideoFrameRate")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.frame_rate = atof((char *)node_text);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "VideoDuration")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.duration = atol((char *)node_text);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "VideoCompressor")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.compressor = wxString((char *)node_text, wxConvUTF8);
							}
						}
						else if ((!xmlStrcmp(level_2->name, (const xmlChar *) "VideoCompressor2")))
						{
							unsigned char *node_text = xmlNodeListGetString(doc, level_2->xmlChildrenNode, 1);
							if (node_text != NULL)
							{
								loaded_item->video.compressor2 = wxString((char *)node_text, wxConvUTF8);
							}
						}

						level_2 = level_2->next;
					}
				}
				else if ((!xmlStrcmp(level_1->name, (const xmlChar *) "AudioTrackList")))
				{
					level_2 = level_1->xmlChildrenNode;
					int audio_track_no = 0;

					while ((level_2 != NULL))
					{
						if ((!xmlStrcmp(level_2->name, (const xmlChar *) "AudioTrack")))
						{
							level_3 = level_2->xmlChildrenNode;
							loaded_item->audio[audio_track_no] = new audioData();

							while ((level_3 != NULL))
							{
								//DEBUG(_T("Walking VideoItem->AudioData Node tree"));
								if ((!xmlStrcmp(level_3->name, (const xmlChar *) "AudioCompression")))
								{
									unsigned char *node_text = xmlNodeListGetString(doc, level_3->xmlChildrenNode, 1);
									if (node_text != NULL)
									{
										loaded_item->audio[audio_track_no]->compression = wxString((char *)node_text, wxConvUTF8);
									}
								}
								else if ((!xmlStrcmp(level_3->name, (const xmlChar *) "AudioBitrate")))
								{
									unsigned char *node_text = xmlNodeListGetString(doc, level_3->xmlChildrenNode, 1);
									if (node_text != NULL)
									{
										loaded_item->audio[audio_track_no]->avg_bitrate = atol((char *)node_text);
									}
								}
								else if ((!xmlStrcmp(level_3->name, (const xmlChar *) "AudioChannelCount")))
								{
									unsigned char *node_text = xmlNodeListGetString(doc, level_3->xmlChildrenNode, 1);
									if (node_text != NULL)
									{
										loaded_item->audio[audio_track_no]->channels = atol((char *)node_text);
									}
								}
								else if ((!xmlStrcmp(level_3->name, (const xmlChar *) "AudioSampleRate")))
								{
									unsigned char *node_text = xmlNodeListGetString(doc, level_3->xmlChildrenNode, 1);
									if (node_text != NULL)
									{
										loaded_item->audio[audio_track_no]->sample_rate = atof((char *)node_text);
									}
								}
								else if ((!xmlStrcmp(level_3->name, (const xmlChar *) "AudioBitDepth")))
								{
									unsigned char *node_text = xmlNodeListGetString(doc, level_3->xmlChildrenNode, 1);
									if (node_text != NULL)
									{
										loaded_item->audio[audio_track_no]->bit_depth = atoi((char *)node_text);
									}
								}
								// Go to the next node	in the AudioData node
								level_3 = level_3->next;
							}
							// We've added a audio data, so increase the count
							audio_track_no++;
						}
						// Go to the next node in the AudioDataList
						level_2 = level_2->next;
					}
				}

				level_1 = level_1->next;
			}
			//Add the item to the database
			AddVideoItemToDatabase(loaded_item);
		}
		level_0 = level_0->next;
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
		wxTextOutputStream xml_output_stream(output_file_stream, wxEOL_DOS);
		//A wxString buffer for composing larger lines
		wxString xml_buffer;
		xml_buffer = _T("<?xml version=\"1.0\"?>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("<!--Video Squirrel " Video_Squirrel_Version " Video File Database-->\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("<!--Written on ");
		xml_buffer += wxDateTime::Now().Format(_T("%x at %X"));
		xml_buffer += _T("running on ") + wxGetOsDescription() + _T("-->\n");
		xml_output_stream.WriteString(xml_buffer);

		xml_buffer = _T("<VideoSquirrelDatabase>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("\t<VideoList>\n");
		xml_output_stream.WriteString(xml_buffer);
		//Go through the list, writing each item
		VideoItem *current_item = NULL;
		wxVideoItemListNode *node = the_database->GetFirst();
		while (node)
		{
			//Get the current item's data
			current_item = (VideoItem *)node->GetData();
			xml_buffer = _T("\t\t<VideoItem>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<UID>");
			xml_buffer += wxString().Format(_T("%i"), current_item->UID);
			xml_buffer += _T("</UID>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<CD>");
			xml_buffer += current_item->cd;
			xml_buffer += _T("</CD>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Title>");
			xml_buffer += current_item->title;
			xml_buffer += _T("</Title>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Filename>");
			xml_buffer += current_item->filename;
			xml_buffer += _T("</Filename>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Filesize>");
			xml_buffer += wxString().Format(_T("%i"), current_item->file_size);
			xml_buffer += _T("</Filesize>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<VideoData>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<Bitrate>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.avg_bitrate);
			xml_buffer += _T("</Bitrate>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<PixelWidth>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.x);
			xml_buffer += _T("</PixelWidth>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<PixelHeight>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.y);
			xml_buffer += _T("</PixelHeight>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<ColorDepth>");
			xml_buffer += wxString().Format(_T("%i"), current_item->video.color_depth);
			xml_buffer += _T("</ColorDepth>\n");
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
			xml_buffer += current_item->video.compressor;
			xml_buffer += _T("</VideoCompressor>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t\t<VideoCompressor2>");
			xml_buffer += current_item->video.compressor2;
			xml_buffer += _T("</VideoCompressor2>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t</VideoData>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<AudioTrackList>\n");
			xml_output_stream.WriteString(xml_buffer);
			int audio_track_no = 0;
			while (current_item->audio[audio_track_no] != NULL)
			{
				xml_buffer = _T("\t\t\t\t<AudioTrack>\n");
				xml_output_stream.WriteString(xml_buffer);
				xml_buffer = _T("\t\t\t\t\t<AudioCompression>");
				xml_buffer += current_item->audio[audio_track_no]->compression;
				xml_buffer += _T("</AudioCompression>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t\t<AudioBitrate>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no]->avg_bitrate);
				xml_buffer += _T("</AudioBitrate>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t\t<AudioChannelCount>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no]->channels);
				xml_buffer += _T("</AudioChannelCount>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t\t<AudioSampleRate>");
				xml_buffer += wxString().Format(_T("%f"), current_item->audio[audio_track_no]->sample_rate);
				xml_buffer += _T("</AudioSampleRate>\n");
				xml_output_stream.WriteString(xml_buffer);

				xml_buffer = _T("\t\t\t\t\t<AudioBitDepth>");
				xml_buffer += wxString().Format(_T("%i"), current_item->audio[audio_track_no]->bit_depth);
				xml_buffer += _T("</AudioBitDepth>\n");
				xml_buffer += _T("\t\t\t\t</AudioTrack>\n");
				xml_output_stream.WriteString(xml_buffer);

				audio_track_no++;
			}
			xml_buffer = _T("\t\t\t</AudioTrackList>\n");
			xml_output_stream.WriteString(xml_buffer);

			xml_buffer = _T("\t\t\t<Comment>");
			xml_buffer += current_item->comment_text;
			xml_buffer += _T("</Comment>\n");
			xml_output_stream.WriteString(xml_buffer);
			
			xml_buffer = _T("\t\t</VideoItem>\n");
			xml_output_stream.WriteString(xml_buffer);
			//Go to the next item
			node = node->GetNext();
		}
		xml_buffer = _T("\t</VideoList>\n");
		xml_output_stream.WriteString(xml_buffer);
		xml_buffer = _T("</VideoSquirrelDatabase>");
		xml_output_stream.WriteString(xml_buffer);
	}
	else
	{
		wxMessageDialog not_saved((wxWindow *)this, _T("Unable to write database file\nExit with database unsaved?"), _T("Database not saved"), wxYES_NO);
		SetStatusText(_T("Unable to write database file"));
		if (not_saved.ShowModal() == wxID_NO)
		{
			return false;
		}
	}
	return true;
};

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
	next_item = NULL;
	UID = 0;
	file_size = 0;
	for (int i = 0; i < 255; i++)
	{
		audio[i] = NULL;
	}
};

VideoItem::~VideoItem()
{
	//
};

unsigned int VideoItem::GetSize()
{
	unsigned int total_size = 0;
	total_size += sizeof(*this);
	int audio_track = 0;
	while (audio[audio_track] != NULL)
	{
		total_size += sizeof(*audio[audio_track]);
		audio_track++;
	}
	return total_size;
};

audioData::audioData()
{
	channels = 0;
	bit_depth = 0;
	sample_rate = 0;
	avg_bitrate = 0;
};

void audioData::SetNiceCompression(int format_tag)
{
	switch (format_tag)
	{
		case 1:
			compression = _T("PCM");
			break;
    case 2:
    	compression = _T("MS ADPCM");
    	break;
    case 34:
    	compression = _T("True Speech");
    	break;
    case 65:
    	compression = _T("IBM mu-law");
    	break;
    case 66:
      compression = _T("IBM a-law");
      break;
    case 67:
    	compression = _T("IBM AVC ADPCM");	
    	break;
		case 85:
			compression = _T("MP3");
			break;
    case 353:
    	compression = _T("WMA");		
    	break;
		default:
			DEBUG(wxString::Format(_T("Unknown Audio Format Tag: %i"), format_tag));
			compression = wxString::Format(_T("Unknown 0x0%x"), format_tag);
			break;
	};
};
		
videoData::videoData()
{
	x = 0;
	y = 0;
	display_x = 0;
	display_y = 0;
	frame_rate = 0;
	duration = 0;
	color_depth = 0;
	avg_bitrate = 0;
};

wxString videoData::GetNiceDuration()
{
	//The duration should be stored in seconds
	if (duration < 60)
	{
		return wxString::Format(_T("%i seconds"), duration);
	}
	else //if (duration < 3600)
	{
		return wxString::Format(_T("%i minutes %i seconds"), int((int)duration / (int)60), duration - int(duration / 60)*60);
	}
	return _T("");
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

 	DEBUG(tree_ctrl_options->GetClassInfo()->GetClassName());
 	DEBUG(tree_ctrl_options->GetClassInfo()->GetBaseClassName1());

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
	database_filename = pConfig->Read(_T("/DatabaseFilename"), wxGetHomeDir() + _T("/database.xml"));
	html_item_view_template = pConfig->Read(_T("/HTML Item View Template"),
		_T("<b><font size=+1>%title%</font></b><hr>"
		"<p><b>CD:</b> %cd%<br>"
		"<b>Filename:</b> %filename%<br>"
		"<b>File Size:</b> %filesize%<br>"
		"<b>Duration:</b> %duration%<br>"
		"<b>Video Bitrate:</b> %videobitrate%<br>"
		"<b>Video Pixel Size:</b> %videopixelsize%<br>"
		"<b>Framerate:</b> %framerate%<br>"
		"<b>Video Compression:</b> %videocompression%<br>"
		"<b>Video Compression2:</b> %videocompression2%<br>"
		"<b>Video Color Depth:</b> %videocolordepth%<br>"
		"<table><tr><td width=20px></td>"
	 	"<td>%audio%</td></tr></table>"
		"</p><p align=right><font size=-1>Item Size in Memory: %itemmemorysize%</font></p><hr>"));

  html_item_view_template_audio = pConfig->Read(_T("/HTML Item View Audio Track Template"),
	  _T("<b>Audio Track:</b> %audiotrack%<br>"
		"<b>Audio Compression:</b> %audiocompression%<br>"
		"<b>Bitrate:</b> %audiobitrate%<br>"
		"<b>Channels:</b> %audiochannels%<br>"
		"<b>Sample Rate:</b> %audiosamplerate%<br>"));
		//Bit-Depth doesn't apply to most compression codecs (Ogg, mp3, ACC?, AC3? so we won't display it)
	 	//"<b>Bit-Depth:</b> %audiobitdepth%<br>");	
};

VideoSquirrelConfiguration::~VideoSquirrelConfiguration()
{
	DEBUG(_T("VideoSquirrelConfiguration::~VideoSquirrelConfiguration called"));
	delete pConfig;
};
 	
