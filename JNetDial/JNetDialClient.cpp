
#include "JNetDialClient.h"

/* XPM */
static char *xpm_connect[] = {
/* width height ncols cpp */
"32 32 7 2",
  /* Colors */
  "00 c #000000",
  "01 c #808080",
  "02 c #C0C0C0",
  "03 c #FF0000",
  "04 c #0000FF",
  "05 c #00FFFF",
  "06 c #FFFFFF",
  "0606060606060606060600000000000000000000060606060606060606060606",
  "0606060606060606060002020202020202020200000606060606060606060606",
  "0606060606060606060002000000000000000002000606060606060606060606",
  "0606060606060606060002000604050605050002000606060604060606060606",
  "0606060606060606060002000605030406040002000606060404060606060606",
  "0603030303030303060002000506060506060002000606040404040404040606",
  "0603030303030303060002000000000000000002000606040404040404040606",
  "0603030606060606060002020202020202020202000006060404060604040606",
  "0603030606060606060600000000000000000000010000060604060604040606",
  "0603030606060606060002020202020202020202000100060606060604040606",
  "0603030606060606000000000000000000000000000100060606060604040606",
  "0603030606060606000202020202020202020202000100060606060604040606",
  "0603030606060606000201010101010101010101000006060606060604040606",
  "0603030606060606000000000000000000000000000606060606060604040606",
  "0603030606060606060606060606060606060606060606060606060604040606",
  "0603030606060606060606060606060606060606060606060606060604040606",
  "0603030606060606060606060606060606060606060606060606060604040606",
  "0603030606060606060606060606060606060606060606060606060604040606",
  "0603030606060606060600000000000000000000060606060606060604040606",
  "0603030606060606060002020202020202020202000606060606060604040606",
  "0603030606030606060002000000000000000002000606060606060604040606",
  "0603030606030306060002000604050605050002000606060606060604040606",
  "0603030303030303060002000605030404060002000606040404040404040606",
  "0603030303030303060002000506050606060002000606040404040404040606",
  "0606060606030306060002000000000000000002000606060606060606060606",
  "0606060606030606060002020202020202020202000006060606060606060606",
  "0606060606060606060600000000000000000000010000060606060606060606",
  "0606060606060606060002020202020202020202000100060606060606060606",
  "0606060606060606000000000000000000000000000100060606060606060606",
  "0606060606060606000202020202020202020202000100060606060606060606",
  "0606060606060606000201010101010101010101000006060606060606060606",
  "0606060606060606000000000000000000000000000606060606060606060606",
};

static char *xpm_connected1[] = {
/* width height ncols cpp */
"16 16 6 2",
  /* Colors */
  "00 c #000000",
  "01 c #008000",
  "02 c #C0C0C0",
  "03 c #808080",
  "04 c #FFFFFF",
  ".. s None c None",
  "................................",
  "..............03030303030303....",
  "............030000000000000200..",
  "............030001040101010200..",
  "....03030303030303040101010200..",
  "..0300000000000002000101010200..",
  "..0300040101010102000101010200..",
  "..0300040101010102000303030200..",
  "..0300010101010102000202020200..",
  "..030001010101010200000000000000",
  "..030003030303030200020303020300",
  "..030202020202020200030303030300",
  "..0300000000000000000000000000..",
  "0304020202020303020300..........",
  "0303030303030303030300..........",
  "..000000000000000000............",
};

static char *xpm_disconnected1[] = {
/* width height ncols cpp */
"16 16 7 2",
  /* Colors */
  "00 c #000000",
  "01 c #008000",
  "02 c #C0C0C0",
  "03 c #808080",
  "04 c #FF0000",
  "05 c #FFFFFF",
  ".. s None c None",
  "................................",
  "..............03030303030303....",
  "............030000000000000200..",
  "............030004050404040200..",
  "....03030303030303050404040200..",
  "..0300000000000002000404040200..",
  "..0300050101010102000404040200..",
  "..0300050101010102000303030200..",
  "..0300010101010102000202020200..",
  "..030001010101010200000000000000",
  "..030003030303030200020303020300",
  "..030202020202020200030303030300",
  "..0300000000000000000000000000..",
  "0305020202020303020300..........",
  "0303030303030303030300..........",
  "..000000000000000000............",
};

static char *xpm_no_server1[] = {
/* width height ncols cpp */
"16 16 6 2",
  /* Colors */
  "00 c #000000",
  "01 c #C0C0C0",
  "02 c #808080",
  "03 c #FF0000",
  "04 c #FFFFFF",
  ".. s None c None",
  "................................",
  "..............02020202020202....",
  "............020000000000000100..",
  "............020003040303030100..",
  "....02020202020202040303030100..",
  "..0200000000000001000303030100..",
  "..0200040303030301000303030100..",
  "..0200040303030301000202020100..",
  "..0200030303030301000101010100..",
  "..020003030303030100000000000000",
  "..020002020202020100010202010200",
  "..020101010101010100020202020200",
  "..0200000000000000000000000000..",
  "0204010101010202010200..........",
  "0202020202020202020200..........",
  "..000000000000000000............",
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(ClientFrame, wxFrame)
    EVT_MENU(ClientFrame_Quit,  ClientFrame::OnQuit)
    EVT_MENU(ClientFrame_About, ClientFrame::OnAbout)
    EVT_BUTTON(ClientFrame_ButtonConnect, ClientFrame::OnButtonConnect)
    EVT_BUTTON(ClientFrame_ButtonDisconnect, ClientFrame::OnButtonDisconnect)
    EVT_BUTTON(ClientFrame_ButtonStayOnlineUpdate, ClientFrame::OnButtonStayOnlineUpdate)
    EVT_SOCKET(SOCKET_ID,  ClientFrame::OnSocketEvent)
		EVT_TIMER(ClientFrame_Timer, ClientFrame::OnTimer)
END_EVENT_TABLE()

#ifdef WIN32
BEGIN_EVENT_TABLE(ClientTrayIcon, wxTaskBarIcon)
    EVT_MENU(ClientFrame_Quit,  ClientTrayIcon::OnMenuQuit)
    EVT_MENU(ClientFrame_MenuConnect, ClientTrayIcon::OnMenuConnect)
    EVT_MENU(ClientFrame_MenuDisconnect, ClientTrayIcon::OnMenuDisconnect)
END_EVENT_TABLE()
#endif

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(ClientApp)

wxConfig *g_configuration = NULL;


// 'Main program' equivalent: the program execution "starts" here
bool ClientApp::OnInit()
{
    SetAppName(wxT("Client"));
		SetVendorName(wxT("JNetDial"));
    wxConfigBase::Set(g_configuration = new wxConfig(wxT("Client"), wxT("JNetDial")));
    
    // create the main application window
    ClientFrame *frame = new ClientFrame();
    wxCHECK_MSG(frame != NULL, false, wxT("Failed to alloc new ClientFrame"));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);
		frame->Hide();
    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}


ClientFrame::ClientFrame():
    wxFrame(NULL, -1, wxT("JNetDial Client ") wxT(JNETDIAL_VERSION_STR), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxSYSTEM_MENU|wxRESIZE_BORDER)
#ifdef WIN32
		, m_TrayIcon(this)
#endif
{
		SetIcon(wxIcon(xpm_connect));
#ifdef WIN32
    m_TrayIcon.SetIcon(wxIcon(xpm_no_server1), wxT("JNetDial Client"));
#endif

    // begin wxGlade: ClientFrame::ClientFrame
    notebook_client = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    notebook_client_pane_server = new wxPanel(notebook_client, -1);
    notebook_client_pane_stayonline = new wxPanel(notebook_client, -1);
		notebook_client_pane_log = new wxPanel(notebook_client, -1);
    label_connection_status = new wxStaticText(notebook_client_pane_server, -1, wxT("Unknown: Disconnected"), wxDefaultPosition, wxDefaultSize, 0);
    button_client_connect = new wxButton(notebook_client_pane_server, ClientFrame_ButtonConnect, wxT("Connect"));
    button_client_disconnect = new wxButton(notebook_client_pane_server, ClientFrame_ButtonDisconnect, wxT("Disconnect"));

    checkbox_enable_stayonline = new wxCheckBox(notebook_client_pane_stayonline, -1, wxT("Enable StayOnline"));
    bool stayOnlineEnabled = false;
    g_configuration->Read(wxT("StayOnline: Enabled"), &stayOnlineEnabled, false);
    checkbox_enable_stayonline->SetValue(stayOnlineEnabled);
    
    label_stayonline_start_time = new wxStaticText(notebook_client_pane_stayonline, -1, wxT("Start Time:"));
    text_ctrl_stayonline_start_time = new wxTextCtrl(notebook_client_pane_stayonline, -1, wxT(""));
    text_ctrl_stayonline_start_time->SetValue(g_configuration->Read(wxT("StayOnline: Start Time"), wxT("00:00:00")));

    label_stayonline_end_time = new wxStaticText(notebook_client_pane_stayonline, -1, wxT("End Time:"));
    text_ctrl_stayonline_end_time = new wxTextCtrl(notebook_client_pane_stayonline, -1, wxT(""));
    text_ctrl_stayonline_end_time->SetValue(g_configuration->Read(wxT("StayOnline: End Time"), wxT("23:59:59")));
    
    button_stayonline_update = new wxButton(notebook_client_pane_stayonline, ClientFrame_ButtonStayOnlineUpdate, wxT("Update"));
    
		text_ctrl_client_log = new wxTextCtrl(notebook_client_pane_log, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxTE_MULTILINE|wxTE_READONLY|wxTE_AUTO_URL);
		
		wxLogTextCtrl *text_ctrl_log = new wxLogTextCtrl(text_ctrl_client_log);
		text_ctrl_log->SetTimestamp(_T("[%Y/%m/%d %H:%M:%S]"));
		wxLog::SetVerbose(true);
		wxLogChain *log_chain = new wxLogChain(text_ctrl_log);
		log_chain = new wxLogChain(new wxLogStderr(fopen("client.log", "a")));

		// create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ClientFrame_About, _T("&About...\tF1"), _T("Show about dialog"));

    menuFile->Append(ClientFrame_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    m_TrayMenu = new wxMenu;
    m_TrayMenu->Append(ClientFrame_MenuConnect, _T("&Connect"), _T(""));
    m_TrayMenu->Append(ClientFrame_MenuDisconnect, _T("Disconnect"), _T(""));
    m_TrayMenu->AppendSeparator();
    m_TrayMenu->Append(ClientFrame_Quit, _T("Close"), _T(""));

    // begin wxGlade: ClientFrame::do_layout
    wxBoxSizer* sizer_background = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_server = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_stayonline = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_stayonline_timespan = new wxStaticBoxSizer(new wxStaticBox(notebook_client_pane_stayonline, -1, wxT("Time Span")), wxHORIZONTAL);
    wxBoxSizer* sizer_stayonline_end_time = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_stayonline_start_time = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* sizer_log = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_connection_control = new wxStaticBoxSizer(new wxStaticBox(notebook_client_pane_server, -1, wxT("Connection Control")), wxHORIZONTAL);
    wxStaticBoxSizer* sizer_connection_status = new wxStaticBoxSizer(new wxStaticBox(notebook_client_pane_server, -1, wxT("Connection Status")), wxHORIZONTAL);
    sizer_connection_status->Add(label_connection_status, 0, wxALL|wxEXPAND, 1);
    sizer_server->Add(sizer_connection_status, 0, wxALL|wxEXPAND, 3);
    sizer_connection_control->Add(button_client_connect, 0, wxALL, 2);
    sizer_connection_control->Add(button_client_disconnect, 0, wxALL, 2);
    sizer_server->Add(sizer_connection_control, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    notebook_client_pane_server->SetAutoLayout(true);
    notebook_client_pane_server->SetSizer(sizer_server);
    sizer_server->Fit(notebook_client_pane_server);
    sizer_server->SetSizeHints(notebook_client_pane_server);
    notebook_client->AddPage(notebook_client_pane_server, wxT("Server"));

    sizer_stayonline->Add(checkbox_enable_stayonline, 0, wxALL|wxEXPAND, 5);
    sizer_stayonline_start_time->Add(label_stayonline_start_time, 0, 0, 0);
    sizer_stayonline_start_time->Add(text_ctrl_stayonline_start_time, 0, 0, 0);
    sizer_stayonline_timespan->Add(sizer_stayonline_start_time, 1, wxLEFT|wxRIGHT|wxEXPAND, 2);
    sizer_stayonline_end_time->Add(label_stayonline_end_time, 0, 0, 0);
    sizer_stayonline_end_time->Add(text_ctrl_stayonline_end_time, 0, 0, 0);
    sizer_stayonline_timespan->Add(sizer_stayonline_end_time, 1, wxLEFT|wxRIGHT, 2);
    sizer_stayonline->Add(sizer_stayonline_timespan, 0, wxALL|wxEXPAND, 1);
    sizer_stayonline->Add(button_stayonline_update, 0, wxALL|wxALIGN_RIGHT, 5);
    notebook_client_pane_stayonline->SetAutoLayout(true);
    notebook_client_pane_stayonline->SetSizer(sizer_stayonline);
    sizer_stayonline->Fit(notebook_client_pane_stayonline);
    sizer_stayonline->SetSizeHints(notebook_client_pane_stayonline);
    notebook_client->AddPage(notebook_client_pane_stayonline, wxT("StayOnline"));
    
    sizer_log->Add(text_ctrl_client_log, 1, wxALL|wxEXPAND, 2);
    notebook_client_pane_log->SetAutoLayout(true);
    notebook_client_pane_log->SetSizer(sizer_log);
    sizer_log->Fit(notebook_client_pane_log);
    sizer_log->SetSizeHints(notebook_client_pane_log);
		notebook_client->AddPage(notebook_client_pane_log, wxT("Log"));
		
    sizer_background->Add(new wxNotebookSizer(notebook_client), 1, wxEXPAND, 0);
    SetAutoLayout(true);
    SetSizer(sizer_background);
    sizer_background->Fit(this);
    sizer_background->SetSizeHints(this);
    Layout();
    Centre();
    
    memset(&m_lastMsg, 0, sizeof(JNetDialServerPacket));
    // Create the socket
    m_sock = new wxSocketClient();
    wxCHECK_RET(m_sock != NULL, wxT("Failed to alloc new wxSocketClient"));

    // Setup the event handler and subscribe to most events
    m_sock->SetEventHandler(*this, SOCKET_ID);
    m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
                      wxSOCKET_INPUT_FLAG |
                      wxSOCKET_LOST_FLAG);
    m_sock->Notify(TRUE);

		TryConnect();

		m_StayOnlineStartTime.ParseTime(text_ctrl_stayonline_start_time->GetValue());
		m_StayOnlineEndTime.ParseTime(text_ctrl_stayonline_end_time->GetValue());

		m_Timer.SetOwner(this, ClientFrame_Timer);
		// Check for server connection every 5 seconds
		m_Timer.Start(5 * 1000);				
}

// event handlers

void ClientFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void ClientFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the About dialog of the JNetDial Client.\n")
                _T("Version %s"),  wxT(JNETDIAL_VERSION_STR));

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void ClientFrame::OnButtonConnect(wxCommandEvent &event)
{
  JNetDialClientPacket packet;
  packet.version = JNETDIAL_VERSION;
	wxString userName = wxGetUserId();
	if (userName.length() == 0)
		userName = wxGetUserName();
	strncpy(packet.name, (const char *)userName.mb_str(wxConvUTF8), 32);
  
	packet.msg = JNETDIAL_CONNECT;
	
	m_sock->WriteMsg(&packet, sizeof(JNetDialClientPacket));
}

void ClientFrame::OnButtonDisconnect(wxCommandEvent &event)
{
  JNetDialClientPacket packet;
  packet.version = JNETDIAL_VERSION;
	wxString userName = wxGetUserId();
	if (userName.length() == 0)
		userName = wxGetUserName();
	strncpy(packet.name, (const char *)userName.mb_str(wxConvUTF8), 32);

	packet.msg = JNETDIAL_DISCONNECT;

	m_sock->WriteMsg(&packet, sizeof(JNetDialClientPacket));
}

void ClientFrame::OnButtonStayOnlineUpdate(wxCommandEvent &event)
{
  wxString startTimeStr;
  startTimeStr = text_ctrl_stayonline_start_time->GetValue();

  if (m_StayOnlineStartTime.ParseTime(startTimeStr.c_str()) == NULL) {
    // Parsing the start time failed
    wxLogMessage(wxT("Invalid date format for start time. Please use ISO 8601 format (HH:MM:SS)"));
    return;
  }
	startTimeStr = m_StayOnlineStartTime.FormatISOTime();
	text_ctrl_stayonline_start_time->SetValue(startTimeStr);
	g_configuration->Write(wxT("StayOnline: Start Time"), startTimeStr);
	  
  wxString endTimeStr;
  endTimeStr = text_ctrl_stayonline_end_time->GetValue();

  if (m_StayOnlineEndTime.ParseTime(endTimeStr.c_str()) == NULL) {
    // Parsing the end time failed
    wxLogMessage(wxT("Invalid date format for end time. Please use ISO 8601 format (HH:MM:SS)"));
    return;
  }
	endTimeStr = m_StayOnlineEndTime.FormatISOTime();
	text_ctrl_stayonline_end_time->SetValue(endTimeStr);
  g_configuration->Write(wxT("StayOnline: End Time"), endTimeStr);
  
  g_configuration->Write(wxT("StayOnline: Enabled"), checkbox_enable_stayonline->GetValue());
}

void ClientFrame::OnSocketEvent(wxSocketEvent& event)
{
  wxString s = _("OnSocketEvent: ");

  switch(event.GetSocketEvent())
  {
    case wxSOCKET_INPUT      : s.Append(_("wxSOCKET_INPUT")); break;
    case wxSOCKET_LOST       : s.Append(_("wxSOCKET_LOST")); break;
    case wxSOCKET_CONNECTION : s.Append(_("wxSOCKET_CONNECTION")); break;
    default                  : s.Append(_("Unexpected event !")); break;
  }

	wxSocketBase *sock = event.GetSocket();

	wxLogDebug(s);
	
	// Now we process the event
  switch(event.GetSocketEvent())
  {
    case wxSOCKET_INPUT:
    {
      // We disable input events, so that the test doesn't trigger
      // wxSocketEvent again.
      sock->SetNotify(wxSOCKET_LOST_FLAG);

      // Which test are we going to run?
      char buffer[1025];
      sock->ReadMsg(buffer, 1024);
			
      wxCHECK_RET(sock->LastCount() >= sizeof(JNetDialServerPacket), wxT("Server Packet too small"));
      
      JNetDialServerPacket *packet = (JNetDialServerPacket *)buffer;
      if (packet->version != JNETDIAL_VERSION) {
        wxLogError(wxT("Invalid packet version"));
        return;
      }
	
			wxString connectStat;
			connectStat = wxString(packet->name, wxConvUTF8) + wxT(": ");

      m_lastMsg = *packet;
			
      if (packet->status == JNETDIAL_ONLINE) {
        connectStat += wxT("Connected");
#ifdef WIN32
				m_TrayIcon.SetIcon(wxIcon(xpm_connected1), wxT("JNetDial Client"));
#endif
      } else if (packet->status == JNETDIAL_OFFLINE) {
        connectStat += wxT("Disconnected");
#ifdef WIN32
				m_TrayIcon.SetIcon(wxIcon(xpm_disconnected1), wxT("JNetDial Client"));
#endif
			} else if (packet->status == JNETDIAL_NO_CONNECTION) {
				connectStat += wxT("No connection");
      }

			label_connection_status->SetLabel(connectStat);
			
      // Enable input events again.
      sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
      break;
    }
	}  
}

void ClientFrame::OnTimer(wxCommandEvent &event)
{
  wxASSERT(m_sock != NULL);
  
	if (!m_sock->IsConnected())
		TryConnect();

	if (m_sock->IsConnected()) {
    // Wee, we are connected to a server
    if (checkbox_enable_stayonline->GetValue()
        && wxDateTime::Now().IsBetween(m_StayOnlineStartTime, m_StayOnlineEndTime))
    {
      // Check our online status
      if (m_lastMsg.status == JNETDIAL_ONLINE) {
        // No worry, we are already online
      } else if (m_lastMsg.status == JNETDIAL_OFFLINE) {
        OnButtonConnect(event);
			} else if (m_lastMsg.status == JNETDIAL_NO_CONNECTION) {
        // No internet connection? strange...
      }
    }
	}
}

void ClientFrame::TryConnect(wxString ip)
{
	// Create the address - defaults to localhost:0 initially
  wxIPV4address addr;
  addr.Hostname(ip);
  addr.Service(4400);

  m_sock->Connect(addr, FALSE);
  m_sock->WaitOnConnect(10);

  if (m_sock->IsConnected()) {
    wxLogDebug(_("Succeeded ! Connection established"));

		// Send the server a packet requesting the current status, this also gives the server our 'nice' name
		JNetDialClientPacket packet;
		packet.version = JNETDIAL_VERSION;
		strncpy(packet.name, (const char *)wxGetUserId().mb_str(wxConvUTF8), 32);
		packet.msg = JNETDIAL_STATUS;
		m_sock->WriteMsg(&packet, sizeof(JNetDialClientPacket));
  } else {
    m_sock->Close();

		/*if (ip == wxT("192.168.0.1")) {
			wxTextEntryDialog dialog(this, wxT("Please enter the ip of the server"), ip);

			if (dialog.ShowModal() == wxID_OK) {
				TryConnect(dialog.GetValue());
			}
		}*/

#ifdef WIN32
		m_TrayIcon.SetIcon(wxIcon(xpm_no_server1), wxT("JNetDial Client"));
#endif
    wxLogDebug(_("Failed ! Unable to connect"));
    //wxLogError(_("Can't connect to 192.168.0.1:4400"));
		label_connection_status->SetLabel(wxT("Unable to connect to server"));
  }
}

#ifdef WIN32
ClientTrayIcon::ClientTrayIcon(ClientFrame *parent)
{
  m_Parent = parent;
};

void ClientTrayIcon::OnRButtonUp(wxEvent &event)
{
  PopupMenu(m_Parent->m_TrayMenu);
}

void ClientTrayIcon::OnLButtonUp(wxEvent &event)
{
  if (m_Parent->IsShown()) {
		m_Parent->Raise();
  }
}

void ClientTrayIcon::OnLButtonDClick(wxEvent &event)
{
  if (m_Parent->IsShown()) {
		m_Parent->Iconize(TRUE);
    m_Parent->Show(FALSE);
  } else {
		m_Parent->Iconize(FALSE);
    m_Parent->Show(TRUE);
    m_Parent->Raise();
  }
}

void ClientTrayIcon::OnMenuConnect(wxCommandEvent &event)
{
	m_Parent->OnButtonConnect(event);
}

void ClientTrayIcon::OnMenuDisconnect(wxCommandEvent &event)
{
	m_Parent->OnButtonDisconnect(event);
}

void ClientTrayIcon::OnMenuQuit(wxCommandEvent &event)
{
	m_Parent->Show(TRUE);
	m_Parent->Close(TRUE);
	wxWakeUpIdle();
}
#endif
