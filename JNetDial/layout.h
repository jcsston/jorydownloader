// -*- C++ -*- generated by wxGlade 0.3.1 on Thu Feb 12 13:44:55 2004

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
#include <wx/notebook.h>
// end wxGlade


#ifndef LAYOUT_H
#define LAYOUT_H



class ClientFrame: public wxFrame {
public:
    // begin wxGlade: ClientFrame::ids
    // end wxGlade

    ClientFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: ClientFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ClientFrame::attributes
    wxStaticText* label_connection_status;
    wxButton* button_client_connect;
    wxButton* button_client_disconnect;
    wxPanel* notebook_client_pane_server;
    wxCheckBox* checkbox_enable_stayonline;
    wxStaticText* label_stayonline_start_time;
    wxTextCtrl* text_ctrl_stayonline_start_time;
    wxStaticText* label_stayonline_end_time;
    wxTextCtrl* text_ctrl_stayonline_end_time;
    wxButton* button_stayonline_validate;
    wxPanel* notebook_client_pane_stayonline;
    wxNotebook* notebook_client;
    // end wxGlade
};

class ServerFrame: public wxFrame {
public:
    // begin wxGlade: ServerFrame::ids
    // end wxGlade

    ServerFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: ServerFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ServerFrame::attributes
    wxStaticText* label_connection_status;
    wxListCtrl* list_ctrl_client_status;
    wxPanel* notebook_base_pane_status;
    wxNotebook* notebook_base;
    // end wxGlade
};


#endif // LAYOUT_H