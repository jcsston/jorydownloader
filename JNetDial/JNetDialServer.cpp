
#include "JNetDialServer.h"
#include "JNetDial.h"

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

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(ServerFrame, wxFrame)
    EVT_MENU(ServerFrame_Quit,  ServerFrame::OnQuit)
    EVT_MENU(ServerFrame_About, ServerFrame::OnAbout)
    EVT_DIALUP_CONNECTED(ServerFrame::OnDialupConnect)
    EVT_DIALUP_DISCONNECTED(ServerFrame::OnDialupConnect)
    EVT_SOCKET(SERVER_ID,  ServerFrame::OnServerEvent)
    EVT_SOCKET(SOCKET_ID,  ServerFrame::OnSocketEvent)
		EVT_TIMER(ServerFrame_Timer, ServerFrame::OnTimer)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	FILE *test = fopen("C:\\test.log", "w");
	fprintf(test, "test output");
	fclose(test);
	wxLogStderr *log_file = new wxLogStderr(fopen("server.log", "a"));
	log_file->SetTimestamp(_T("[%Y/%m/%d %H:%M:%S]"));
	wxLog::SetVerbose(true);
	wxLog::SetActiveTarget(log_file);

	// create the main application window
  ServerFrame *frame = new ServerFrame();
  wxCHECK_MSG(frame != NULL, FALSE, wxT("Failed to alloc new ServerFrame"));
  
  // and show it (the frames, unlike simple controls, are not shown when
  // created initially)
  frame->Show(TRUE);
	frame->Hide();

  // success: wxApp::OnRun() will be called which will enter the main message
  // loop and the application will run. If we returned FALSE here, the
  // application would exit immediately.
  return TRUE;
}


ServerFrame::ServerFrame():
    wxFrame(NULL, -1, wxT("JNetDial Server"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxMINIMIZE_BOX|wxMAXIMIZE_BOX|wxSYSTEM_MENU|wxRESIZE_BORDER)
#ifdef WIN32
		, m_TrayIcon(this)
#endif
{    
		SetIcon(wxIcon(xpm_connect));

#ifdef WIN32
    m_TrayIcon.SetIcon(wxIcon(xpm_connect), wxT("JNetDial Server"));
#endif
    // begin wxGlade: ServerFrame::ServerFrame
    notebook_base = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, 0);
    notebook_base_pane_status = new wxPanel(notebook_base, -1);
    label_connection_status = new wxStaticText(notebook_base_pane_status, -1, wxT("No connection: Disconnected"), wxDefaultPosition, wxDefaultSize, 0);
    list_ctrl_client_status = new wxListCtrl(notebook_base_pane_status, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);
		list_ctrl_client_status->InsertColumn(0, _T("Name"), wxLIST_FORMAT_LEFT);
		list_ctrl_client_status->InsertColumn(1, _T("IP"), wxLIST_FORMAT_LEFT);

		// create a menu bar
    wxMenu *menuFile = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ServerFrame_About, _T("&About...\tF1"), _T("Show about dialog"));

    menuFile->Append(ServerFrame_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, _T("&File"));
    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

    // begin wxGlade: ServerFrame::do_layout
    wxBoxSizer* sizer_background = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_status = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_client_status = new wxStaticBoxSizer(new wxStaticBox(notebook_base_pane_status, -1, wxT("Client Status")), wxHORIZONTAL);
    wxStaticBoxSizer* sizer_connection_status = new wxStaticBoxSizer(new wxStaticBox(notebook_base_pane_status, -1, wxT("Connection Status")), wxHORIZONTAL);
    sizer_connection_status->Add(label_connection_status, 0, wxALL|wxEXPAND, 1);
    sizer_status->Add(sizer_connection_status, 0, wxALL|wxEXPAND, 3);
    sizer_client_status->Add(list_ctrl_client_status, 1, wxEXPAND, 0);
    sizer_status->Add(sizer_client_status, 1, wxALL|wxEXPAND, 3);
    notebook_base_pane_status->SetAutoLayout(true);
    notebook_base_pane_status->SetSizer(sizer_status);
    sizer_status->Fit(notebook_base_pane_status);
    sizer_status->SetSizeHints(notebook_base_pane_status);
    notebook_base->AddPage(notebook_base_pane_status, wxT("Status"));
    sizer_background->Add(new wxNotebookSizer(notebook_base), 1, wxEXPAND, 0);
    SetAutoLayout(true);
    SetSizer(sizer_background);
    sizer_background->Fit(this);
    sizer_background->SetSizeHints(this);
    Layout();
    Centre();
    // end wxGlade

    // Init dial up manager
    m_dial = wxDialUpManager::Create();

    if ((m_dial == NULL) || !m_dial->IsOk())
    {
        wxLogError(wxT("Unable to create wxDialUpManager."));
        return;
    }    
		m_dial->EnableAutoCheckOnlineStatus();

    // Create the address - defaults to localhost:0 initially
    wxIPV4address addr;
    addr.Service(4400);

    // Create the socket
    m_server = new wxSocketServer(addr);
    wxCHECK_RET(m_server != NULL, wxT("Failed to alloc new wxSocketServer"));
    
    // We use Ok() here to see if the server is really listening
    if (! m_server->Ok())
    {
      wxLogError(_("Could not listen at the specified port!"));
      return;
    }
    else
    {
      wxLogDebug(_("Server listening."));
    }

    // Setup the event handler and subscribe to connection events
    m_server->SetEventHandler(*this, SERVER_ID);
    m_server->SetNotify(wxSOCKET_CONNECTION_FLAG);
    m_server->Notify(TRUE);
    
    m_clients.Clear();
  
    wxString connectionStr;
    size_t ispCount;
    wxArrayString ispNames;

    ispCount = m_dial->GetISPNames(ispNames);
    if (ispCount > 0) {
      connectionStr = ispNames[0] + wxT(": ");
      connectionStr += m_dial->IsOnline() ? wxT("Connected") : wxT("Disconnected");
    } else {
      connectionStr = wxT("None Found: ");
      connectionStr += m_dial->IsOnline() ? wxT("Connected") : wxT("Disconnected");
    }		
		label_connection_status->SetLabel(connectionStr);

		m_Timer.SetOwner(this, ServerFrame_Timer);
		// Update clients every 10 seconds
		m_Timer.Start(10 * 1000);

		m_StartConnectTime = 0;
		m_ConnectTime = 0;
}

// event handlers

void ServerFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void ServerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the About dialog of the minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}

void ServerFrame::OnDialupConnect(wxDialUpEvent& event)
{
    wxString connectionStr;
    size_t ispCount;
    wxArrayString ispNames;

    ispCount = m_dial->GetISPNames(ispNames);
    if (ispCount > 0) {
      connectionStr = ispNames[0] + wxT(": ");
      connectionStr += m_dial->IsOnline() ? wxT("Connected") : wxT("Disconnected");
    } else {
      connectionStr = wxT("None Found: ");
      connectionStr += m_dial->IsOnline() ? wxT("Connected") : wxT("Disconnected");
    }		
		label_connection_status->SetLabel(connectionStr);

		UpdateClients();

    //wxLogDebug(msg);
}

void ServerFrame::Dial()
{
	wxASSERT(m_dial != NULL);
	
	if (!m_dial->IsDialing() && !m_dial->IsOnline()) {
		m_dial->Dial(wxEmptyString, wxEmptyString, wxEmptyString, false);
		
		if (m_dial->IsOnline()) {
			m_StartConnectTime = wxGetLocalTime();
			UpdateCJB(true);
		} else {
			UpdateClients(wxT("Failed to connect."));
		}
	}
}

void ServerFrame::HangUp()
{
  wxASSERT(m_dial != NULL);
	if (m_dial->IsOnline())
		m_dial->HangUp();
}

void ServerFrame::OnServerEvent(wxSocketEvent& event)
{
  wxString s = _("OnServerEvent: ");
  wxSocketBase *sock;

  switch(event.GetSocketEvent())
  {
    case wxSOCKET_CONNECTION : s.Append(_("wxSOCKET_CONNECTION")); break;
    default                  : s.Append(_("Unexpected event !")); break;
  }

  wxLogDebug(s);

  // Accept new connection if there is one in the pending
  // connections queue, else exit. We use Accept(FALSE) for
  // non-blocking accept (although if we got here, there
  // should ALWAYS be a pending connection).

  wxASSERT(m_server != NULL);
  sock = m_server->Accept(FALSE);

  if (sock) {
    wxLogDebug(_("New client connection accepted"));
		JNetClient *newClient = new JNetClient;
		newClient->sock = sock;
		wxIPV4address addr;
		sock->GetPeer(addr);
		newClient->ip = addr.Hostname();

		m_clients.Add(newClient);
  }
  else
  {
    wxLogError(_("Couldn't accept a new connection"));
    return;
  }

  sock->SetEventHandler(*this, SOCKET_ID);
  sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
  sock->Notify(TRUE);
}

void ServerFrame::OnSocketEvent(wxSocketEvent& event)
{
  wxString s = _("OnSocketEvent: ");
  wxSocketBase *sock = event.GetSocket();
  wxASSERT(sock != NULL);
  
  // First, print a message
  switch(event.GetSocketEvent())
  {
    case wxSOCKET_INPUT : s.Append(_("wxSOCKET_INPUT")); break;
    case wxSOCKET_LOST  : s.Append(_("wxSOCKET_LOST")); break;
    default             : s.Append(_("Unexpected event !")); break;
  }

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
      wxCHECK_RET(sock->LastCount() >= sizeof(JNetDialClientPacket), wxT("Client Packet too small"));

      JNetDialClientPacket *packet = (JNetDialClientPacket *)buffer;
      if (packet->version != JNETDIAL_VERSION) {
        wxLogError(wxT("Invalid packet version"));
        return;
      }

			if (packet->msg == JNETDIAL_STATUS) {
				JNetClient *client = LookupClient(sock);
				if (client != NULL) {
					client->name = wxString(packet->name, wxConvUTF8);
					UpdateClient(client);
					UpdateClientListView();
				} else {
					wxLogError(wxT("Invalid client socket"));
				}
      } else if (packet->msg == JNETDIAL_CONNECT) {
        Dial();
      } else if (packet->msg == JNETDIAL_DISCONNECT) {
        HangUp();
      }


      // Enable input events again.
      sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
      break;
    }
    case wxSOCKET_LOST:
    {
      RemoveClient(sock);

			UpdateClientListView();
      // Destroy() should be used instead of delete wherever possible,
      // due to the fact that wxSocket uses 'delayed events' (see the
      // documentation for wxPostEvent) and we don't want an event to
      // arrive to the event handler (the frame, here) after the socket
      // has been deleted. Also, we might be doing some other thing with
      // the socket at the same time; for example, we might be in the
      // middle of a test or something. Destroy() takes care of all
      // this for us.

      wxLogDebug(_("Deleting socket."));
      sock->Destroy();
      break;
    }
    default: ;
  }
}

void ServerFrame::OnTimer(wxCommandEvent &event)
{
	OnDialupConnect((wxDialUpEvent &)*((int *)NULL));

	m_ConnectTime = wxGetLocalTime() - m_StartConnectTime;
	
	if (m_ConnectTime > 3 * 59 * 60)
		// We are about to hit the 4 hour limit
		UpdateCJB(false);

	UpdateClients();
}

void ServerFrame::RemoveClient(wxSocketBase *sock)
{
  for (size_t c = 0; c < m_clients.GetCount(); c++) {
    JNetClient *item = (JNetClient *)m_clients.Item(c);
    if (item->sock == sock)
      delete item;
    m_clients.RemoveAt(c);
  }
}

JNetClient *ServerFrame::LookupClient(wxSocketBase *sock)
{
  for (size_t c = 0; c < m_clients.GetCount(); c++) {
    JNetClient *item = (JNetClient *)m_clients.Item(c);
    if (item->sock == sock)
      return item;
  }
	return NULL;
}

void ServerFrame::UpdateClientListView()
{
	size_t c;
	list_ctrl_client_status->DeleteAllItems();
	for (c = 0; c < m_clients.GetCount(); c++) {
		JNetClient *clientItem = (JNetClient *)m_clients[c];
		wxListItem item;
		item.SetColumn(0);
		item.SetMask(wxLIST_MASK_TEXT|wxLIST_MASK_DATA);
		item.SetText(clientItem->name);		
		item.SetData((long)c);
		int itemIndex = list_ctrl_client_status->InsertItem(item);
		// We can reuse this, just update the text, column and index id
		item.SetId(itemIndex);
		item.SetColumn(1);
		item.SetText(clientItem->ip);		
		list_ctrl_client_status->SetItem(item);
		// We can still reuse this, but now we just update the text and column
		/*
		item.SetColumn(2);
		item.SetMask(item.GetMask()|wxLIST_MASK_FORMAT);
		item.SetAlign(wxLIST_FORMAT_RIGHT);
		item.SetText(wxString::Format(_T("%.1f KB"), (float)(currentAttachment->SourceDataLength+1) / 1024));		
		totalSize += (float)(currentAttachment->SourceDataLength+1) / 1024;
		list_ctrl_client_status->SetItem(item);
		*/
	}	
}

void ServerFrame::UpdateClients(const wxString &msg)
{
	// Send out our status to the clients
	for (size_t c = 0; c < m_clients.GetCount(); c++) {
    JNetClient *item = (JNetClient *)m_clients.Item(c);
		UpdateClient(item, msg);
	}
}

void ServerFrame::UpdateClient(JNetClient *client, const wxString &msg)
{
	// We update each client at a time
	JNetDialServerPacket packet;
  packet.version = JNETDIAL_VERSION;
	strncpy(packet.server_name, (const char *)wxGetHostName().mb_str(wxConvUTF8), 32);
	
	wxArrayString ispNames;
	if (m_dial->GetISPNames(ispNames) > 0) {
		strncpy(packet.name, (const char *)ispNames[0].mb_str(wxConvUTF8), 32);
		if (m_dial->IsOnline()) {
			packet.status = JNETDIAL_ONLINE;
		} else {
			packet.status = JNETDIAL_OFFLINE;
		}
	} else {
		packet.status = JNETDIAL_NO_CONNECTION;
	}
	strncpy(packet.status_msg, (const char *)msg.mb_str(wxConvUTF8), 127);


	client->sock->WriteMsg(&packet, sizeof(JNetDialServerPacket));
}

void ServerFrame::UpdateCJB(bool online)
{
	if (online) {
		wxURL cjbUpdate(wxT("http://www.cjb.net/cgi-bin/dynip.cgi?username=webjory&password=play"));
		wxInputStream *cjbUpdateInStream = cjbUpdate.GetInputStream();
		cjbUpdateInStream->GetC();			
	} else {
		wxURL cjbUpdate(wxT("http://www.cjb.net/cgi-bin/dynip.cgi?username=webjory&password=play&url=http://webjory.tripod.com/offline.htm"));
		wxInputStream *cjbUpdateInStream = cjbUpdate.GetInputStream();
		cjbUpdateInStream->GetC();	


	}
}

#ifdef WIN32
ServerTrayIcon::ServerTrayIcon(ServerFrame *parent)
{
  m_Parent = parent;
};

void ServerTrayIcon::OnLButtonDClick(wxEvent &event)
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
#endif
