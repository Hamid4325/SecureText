#include "App.h"
#include "MainFrame.h"
#include "wx/wx.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("SecureText Encryption/Decryption");
	mainFrame->SetClientSize(800, 620);
	mainFrame->Centre();
	mainFrame->Show(true);
	return true;
}
