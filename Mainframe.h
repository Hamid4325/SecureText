#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <vector>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    // Event Handlers
    void OnSelectFile(wxCommandEvent& event);
    void OnSelectOperation(wxCommandEvent& event);
    void OnGetKey(wxCommandEvent& event);
    void OnShowConversion(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    // Event handler for file operation
    void PerformFileOperation(const wxString& inputFile, const wxString& outputFile, const wxString& key, bool encrypt);



    // Member Variables    
    wxTextCtrl* m_keyCtrl;
    wxStaticText* m_statusLabel;
    wxStaticText* staticText5;  // Static text to show file save message
    wxString m_inputFile;
    wxString m_outputFile;
    wxString m_key;
    bool m_isEncryption;
    wxGauge* m_gauge;  // Progress gauge to show conversion status

    // Declare the Event Table
    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H
