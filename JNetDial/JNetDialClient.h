
#ifndef _J_NET_DIAL_CLIENT_H_
#define _J_NET_DIAL_CLIENT_H_

#include <wx/wx.h>
#include <wx/confbase.h>
#include <wx/config.h>
#include <wx/datetime.h>
#include <wx/dialup.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/notebook.h>
#include <wx/socket.h>
#include <wx/string.h>
#include <wx/textdlg.h>
#include <wx/utils.h>
#ifdef WIN32
#include <wx/taskbar.h>
#endif

#include "JNetDial.h"
#include <string.h>
// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class ClientApp : public wxApp
{
public:
    virtual bool OnInit();
};

class ClientFrame;

#ifdef WIN32
class ClientTrayIcon : public wxTaskBarIcon {
public:
  ClientTrayIcon(ClientFrame *parent);
  
	virtual void OnRButtonUp(wxEvent &event);
	virtual void OnLButtonUp(wxEvent &event);
  virtual void OnLButtonDClick(wxEvent &event);	  
  
  void OnMenuConnect(wxCommandEvent &event);
  void OnMenuDisconnect(wxCommandEvent &event);
	void OnMenuQuit(wxCommandEvent &event);

protected:
  ClientFrame *m_Parent;

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};
#endif

class ClientFrame: public wxFrame {
public:
    ClientFrame();

		void OnAbout(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
    
    void OnButtonConnect(wxCommandEvent &event);
    void OnButtonDisconnect(wxCommandEvent &event);
    void OnButtonStayOnlineUpdate(wxCommandEvent &event);
    
    void OnSocketEvent(wxSocketEvent& event);
    
		void OnTimer(wxCommandEvent &event);
protected:
		void TryConnect(wxString ip = wxT("192.168.0.1"));

    JNetDialServerPacket m_lastMsg;
    
    wxSocketClient *m_sock;
    wxMenu *m_TrayMenu;
		wxTimer m_Timer;
		
		wxDateTime m_StayOnlineStartTime;
		wxDateTime m_StayOnlineEndTime;

    wxStaticText* label_connection_status;
		wxTextCtrl* text_ctrl_client_log;
    wxButton* button_client_connect;
    wxButton* button_client_disconnect;
    wxPanel* notebook_client_pane_server;
		wxPanel* notebook_client_pane_log;
    wxCheckBox* checkbox_enable_stayonline;
    wxStaticText* label_stayonline_start_time;
    wxTextCtrl* text_ctrl_stayonline_start_time;
    wxStaticText* label_stayonline_end_time;
    wxTextCtrl* text_ctrl_stayonline_end_time;
    wxButton* button_stayonline_update;
    wxPanel* notebook_client_pane_stayonline;
    wxNotebook* notebook_client;

#ifdef WIN32
		ClientTrayIcon m_TrayIcon;
    friend ClientTrayIcon;
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
    ClientFrame_Quit = 123,
    ClientFrame_MenuConnect,
    ClientFrame_MenuDisconnect,
    ClientFrame_ButtonConnect,
    ClientFrame_ButtonDisconnect,
    ClientFrame_ButtonStayOnlineUpdate,
		ClientFrame_Timer,

    // id for sockets
    SOCKET_ID,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    ClientFrame_About = wxID_ABOUT
};

#endif // _J_NET_DIAL_CLIENT_H_

