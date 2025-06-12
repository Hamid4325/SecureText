#include "MainFrame.h"
#include <wx/timer.h>
#include <wx/filedlg.h>
#include <fstream>
#include <cctype>
#include <wx/bitmap.h>
#include <wx/image.h>

using namespace std;

// Enum for Event IDs
enum
{
    ID_SELECT_FILE = 1,
    ID_SAVE_FILE,
    ID_ENCRYPTION_CHECKBOX,
    ID_DECRYPTION_CHECKBOX,
    ID_SET_KEY,
    ID_START_CONVERSION
};

wxString process = "Encrypted";

// Event Table
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)

EVT_BUTTON(ID_SELECT_FILE, MainFrame::OnSelectFile)
EVT_RADIOBUTTON(ID_ENCRYPTION_CHECKBOX, MainFrame::OnSelectOperation)
EVT_RADIOBUTTON(ID_DECRYPTION_CHECKBOX, MainFrame::OnSelectOperation)
EVT_BUTTON(ID_SET_KEY, MainFrame::OnGetKey)
EVT_BUTTON(ID_START_CONVERSION, MainFrame::OnShowConversion)
EVT_BUTTON(ID_SAVE_FILE, MainFrame::OnSaveFile)

wxEND_EVENT_TABLE()


MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title), m_isEncryption(true)
{

    wxPanel* panel = new wxPanel(this);
    SetBackgroundColour(wxColour(250, 235, 215));
    panel->SetWindowStyleFlag(wxBORDER_DOUBLE);

    wxColour textColor("#000000"); // Define the single color variable
    int verticalSpacing = 60;

    // Load and display logo
    wxImage::AddHandler(new wxPNGHandler());
    wxImage logoImage("SecureText_Logo3.png", wxBITMAP_TYPE_PNG);
    logoImage = logoImage.Scale(200, 200, wxIMAGE_QUALITY_HIGH);
    wxBitmap logoBitmap(logoImage);
    wxStaticBitmap* logo = new wxStaticBitmap(panel, wxID_ANY, logoBitmap, wxPoint(300, 12), wxSize(200, 140));

    // Text Control for Key Input
    m_keyCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(40, 300 + verticalSpacing), wxSize(200, -1), wxTE_PASSWORD);

    // Status Label
    m_statusLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(310, 555 + verticalSpacing));
    m_statusLabel->SetForegroundColour(textColor); // Apply text color

    // If you've added verticalSpacing, adjust the label's position accordingly
    m_statusLabel->Move(wxPoint(280, 585)); // Modify the Y value to move it upwards

    // Progress Gauge
    m_gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(290, 550), wxSize(200, 25));

    // Buttons and Radio Buttons
    wxRadioButton* radioEncryption = new wxRadioButton(panel, ID_ENCRYPTION_CHECKBOX, "Encryption", wxPoint(480, 185 + verticalSpacing), wxDefaultSize, wxRB_GROUP);
    wxRadioButton* radioDecryption = new wxRadioButton(panel, ID_DECRYPTION_CHECKBOX, "Decryption", wxPoint(620, 185 + verticalSpacing));
    radioEncryption->SetForegroundColour(textColor);
    radioDecryption->SetForegroundColour(textColor);

    wxButton* button1 = new wxButton(panel, ID_SELECT_FILE, "Select File", wxPoint(80, 170 + verticalSpacing), wxSize(100, 45));
    wxButton* button2 = new wxButton(panel, ID_SAVE_FILE, "Save File", wxPoint(510, 310 + verticalSpacing), wxSize(100, 45));
    wxButton* button3 = new wxButton(panel, ID_SET_KEY, "Set Key", wxPoint(80, 350 + verticalSpacing), wxSize(100, 45));
    wxButton* button4 = new wxButton(panel, ID_START_CONVERSION, "Start Conversion", wxPoint(320, 490), wxSize(140, 45));
    button4->SetBackgroundColour(wxColour("#7ccfa7"));

    // Static Text Elements
    wxStaticText* staticText2 = new wxStaticText(panel, wxID_ANY, "Please Select a File (.txt)", wxPoint(35, 120 + verticalSpacing));
    wxFont font2(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    staticText2->SetFont(font2);
    staticText2->SetForegroundColour(textColor);

    wxStaticText* staticText3 = new wxStaticText(panel, wxID_ANY, "Please Select One Of The Following Process To Perform", wxPoint(400, 120 + verticalSpacing));
    wxFont font3(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    staticText3->SetFont(font3);
    staticText3->SetForegroundColour(textColor);

    wxStaticText* staticText4 = new wxStaticText(panel, wxID_ANY, "Please Enter The Key (UpperCase)", wxPoint(35, 260 + verticalSpacing));
    wxFont font4(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    staticText4->SetFont(font4);
    staticText4->SetForegroundColour(textColor);

    staticText5 = new wxStaticText(panel, wxID_ANY, "Please Select a Location To Save The " + process + " File", wxPoint(400, 260 + verticalSpacing));
    wxFont font5(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    staticText5->SetFont(font5);
    staticText5->SetForegroundColour(textColor);
}


void MainFrame::OnSelectFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open File", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK)
    {
        m_inputFile = openFileDialog.GetPath();
        m_statusLabel->SetLabel("File Selected: " + m_inputFile);
    }
}

void MainFrame::OnSelectOperation(wxCommandEvent& event)
{

    if (event.GetId() == ID_ENCRYPTION_CHECKBOX)
    {
        m_isEncryption = true;
        process = "Encrypted";
        m_statusLabel->SetLabel("Encryption Process is Selected Successfully");
    }

    else if (event.GetId() == ID_DECRYPTION_CHECKBOX)
    {
        m_isEncryption = false;
        process = "Decrypted";
        m_statusLabel->SetLabel("Decryption Process is Selected Successfully");
    }

    wxString newText = "Please Select a Location To Save The " + process + " File";
    staticText5->SetLabel(newText);
    m_gauge->SetValue(0);
}


void MainFrame::OnGetKey(wxCommandEvent& event)
{
    wxString keyInput = m_keyCtrl->GetValue();

    if (keyInput.IsEmpty())
    {
        m_statusLabel->SetLabel("Error: Key cannot be empty!");
        return;
    }

    // Remove spaces from the key
    wxString sanitizedKey;

    for (size_t i = 0; i < keyInput.Length(); i++)
    {
        if (!isspace(keyInput[i]))
        {
            sanitizedKey += keyInput[i];
        }
    }

    if (sanitizedKey.IsEmpty())
    {
        m_statusLabel->SetLabel("Error: Key cannot consist solely of spaces!");
        return;
    }

    m_key = sanitizedKey.Upper();  // Ensure the key is uppercase
    m_statusLabel->SetLabel("Key received: " + keyInput.Upper());
}


void MainFrame::OnShowConversion(wxCommandEvent& event)
{
    if (m_inputFile.IsEmpty() || m_key.IsEmpty())
    {
        m_statusLabel->SetLabel("Error: Please select a file and provide a key.");
        return;
    }

    if (m_outputFile.IsEmpty())
    {
        m_statusLabel->SetLabel("Error: Please select a save location.");
        return;
    }

    PerformFileOperation(m_inputFile, m_outputFile, m_key, m_isEncryption);
}

void MainFrame::OnSaveFile(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Save File", "", "", "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK)
    {
        m_outputFile = saveFileDialog.GetPath();
        m_statusLabel->SetLabel("File ready to save: " + m_outputFile);
    }
}

void MainFrame::PerformFileOperation(const wxString& inputFile, const wxString& outputFile, const wxString& key, bool encrypt)
{
    ifstream input(inputFile.ToStdString(), ios::binary);

    if (!input.is_open())
    {
        m_statusLabel->SetLabel("Error: Could not open input file.");
        return;
    }

    ofstream output(outputFile.ToStdString(), ios::binary);

    if (!output.is_open())
    {
        m_statusLabel->SetLabel("Error: Could not create output file.");
        return;
    }

    wxString extendedKey = key.Upper();
    size_t keyLength = extendedKey.Length();

    if (keyLength == 0)
    {
        m_statusLabel->SetLabel("Error: Key cannot be empty!");
        return;
    }

    // Read the entire file
    input.seekg(0, ios::end);
    size_t fileSize = input.tellg();
    if (fileSize == 0) {
        m_statusLabel->SetLabel("Error: The input file is empty.");
        input.close();
        return;
    }
    input.seekg(0, ios::beg);

    string fileContent((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    if (!encrypt)
    {
        reverse(fileContent.begin(), fileContent.end());
    }

    // Calculate file chunks for more controlled progress
    size_t chunkSize = max(fileSize / 20, static_cast<size_t>(1)); 

    m_gauge->SetRange(100);  // Set range to percentage
    m_gauge->SetValue(0);

    string processedText;
    size_t keyIndex = 0;
    size_t processedSize = 0;

    for (char ch : fileContent)
    {
        unsigned char uchar = static_cast<unsigned char>(ch);

        if (uchar != '\n')
        {
            int keyChar = extendedKey[keyIndex % keyLength] - 'A';

            if (encrypt)
            {
                uchar = static_cast<unsigned char>((uchar + keyChar) % 256);
            }
            else
            {
                uchar = static_cast<unsigned char>((uchar - keyChar + 256) % 256);
            }

            keyIndex++;
        }

        processedText += static_cast<char>(uchar);

        // Update progress more deliberately
        processedSize++;

        if (processedSize % chunkSize == 0)
        {
            int progress = static_cast<int>((static_cast<double>(processedSize) / fileSize) * 100);
            m_gauge->SetValue(progress);

            // Create visual pause between updates
            wxMilliSleep(50);  // Brief pause to make progress visible
            wxYield();
        }
    }

    if (encrypt)
    {
        reverse(processedText.begin(), processedText.end());
    }

    output.write(processedText.c_str(), processedText.size());
    output.close();

    // Ensure gauge reaches 100%
    m_gauge->SetValue(100);

    // Short delay before showing completion message
    wxMilliSleep(200);
    m_statusLabel->SetLabel("File conversion complete!");
}