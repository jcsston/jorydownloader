
#ifndef _J_NET_DIAL_SERVER_H_
#define _J_NET_DIAL_SERVER_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/dialup.h>
#include <wx/socket.h>
#include <wx/url.h>
#ifdef WIN32
#include <wx/taskbar.h>
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

struct JNetClient {
  wxString name;
	wxString ip;
  wxSocketBase *sock;
};

WX_DEFINE_ARRAY(JNetClient *, JNetClients);

class ServerFrame;

#ifdef WIN32
class ServerTrayIcon : public wxTaskBarIcon {
public:
  ServerTrayIcon(ServerFrame *parent);
  
  virtual void OnLButtonDClick(wxEvent &event);

protected:
  ServerFrame *m_Parent;
};
#endif

class ServerFrame: public wxFrame {
public:
    ServerFrame();

		void OnAbout(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
    
    void OnDialupConnect(wxDialUpEvent& event);

    void OnServerEvent(wxSocketEvent& event);
    void OnSocketEvent(wxSocketEvent& event);

		void OnTimer(wxCommandEvent &event);
    void Dial();
    void HangUp();
protected:
    void RemoveClient(wxSocketBase *sock);
		JNetClient *LookupClient(wxSocketBase *sock);
    void UpdateClientListView();
		void UpdateClients(const wxString &msg = wxEmptyString);
		void UpdateClient(JNetClient *client, const wxString &msg = wxEmptyString);
		void UpdateCJB(bool online);

    JNetClients m_clients;
    wxDialUpManager *m_dial;
    wxSocketServer *m_server;
    
		wxTimer m_Timer;
		long m_StartConnectTime;
		long m_ConnectTime;
		long m_ConnectedTime;

    // begin wxGlade: ServerFrame::attributes
    wxStaticText* label_connection_status;
    wxListCtrl* list_ctrl_client_status;
    wxPanel* notebook_base_pane_status;
    wxNotebook* notebook_base;
    // end wxGlade

#ifdef WIN32
    ServerTrayIcon m_TrayIcon;
		friend ServerTrayIcon;
#endif

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    ServerFrame_Quit = 123,
		ServerFrame_Timer,

    // id for sockets
    SERVER_ID,
    SOCKET_ID,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    ServerFrame_About = wxID_ABOUT
};

#endif // _J_NET_DIAL_SERVER_H_

