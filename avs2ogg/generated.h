// -*- C++ -*- generated by wxGlade 0.2.3 on Sat Oct 04 00:29:25 2003

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
// end wxGlade


#ifndef GENERATED_H
#define GENERATED_H


class Avs2OggDialog: public wxDialog {
public:
    // begin wxGlade: Avs2OggDialog::ids
    // end wxGlade

    Avs2OggDialog(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: Avs2OggDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: Avs2OggDialog::attributes
    wxStaticText* label_title;
    wxListBox* list_box_input_files;
    wxButton* button_add;
    wxRadioBox* radio_box_output_file_type;
    wxCheckBox* checkbox_normalize;
    wxComboBox* combo_box_vorbis_q;
    wxStaticText* label_1;
    wxTextCtrl* text_ctrl_output_filename;
    wxButton* button_start;
    wxButton* button_close;
    // end wxGlade
};


#endif // GENERATED_H
