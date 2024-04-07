#include "MainApp.hpp"
#include "MainFrame.hpp"

bool MainApp::OnInit() {
	MainFrame* mainFrame = new MainFrame("Ising Model", wxDefaultPosition, wxDefaultSize);
	mainFrame->Show(true);
	return true;
}

wxIMPLEMENT_APP(MainApp);
