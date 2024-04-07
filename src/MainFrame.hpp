#pragma once
#include <wx/wx.h>
#include <wx/radiobox.h>
#include "BufferedBitmap.hpp"
#include "wxChartCtrl.hpp"
#include "Ising.hpp"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	wxButton* resetButton;			//	������ ���������\��������.	
	wxButton* startButton;			//	���� ���������.
	wxTextCtrl* sizeCtrl;			//	�������� ������ ��������� ������.
	wxTextCtrl* jCtrl;				//  �������� J.
	wxTextCtrl* posFractionCtrl;	//	���� ������������� ������.
	wxTextCtrl* eqlItersCtrl;		//	���-�� �������� ��� ����������.
	wxTextCtrl* avgItersCtrl;		//	���-�� �������� ��� ����������.
	wxTextCtrl* tRangeTicksCtrl;	//	���-�� ��������� ��������� �����������. 
	wxTextCtrl* tLowerCtrl;			//	������ ������� ��������� ����������� (� ��. Tc(����������� �����������)).
	wxTextCtrl* tUpperCtrl;			//	������� ������� ��������� ����������� (� ��. Tc).
	wxTextCtrl* tSingleCtrl;		//	����������� (� ��. Tc) � ������ ���������� �������.
	wxTextCtrl* tCritNumerical;		//	��������� �������� Tc, ���������� �� ������� E(T) (� ������ ��������� �����������).
	wxRadioButton* testModeRadio1;	//	����� ������������: ��� ������ �������� ����������� ��� ��� ��������� ����������.
	wxRadioButton* testModeRadio2;	
	wxRadioButton* configChangeModeRadio1;	// ����� ����� ������������ ������: �������� ��������(����� �������� ������) ��� ����� ���� ������������ ������.
	wxRadioButton* configChangeModeRadio2;
	

	wxStaticBox* testModeStaticBox;	// ����� ��� ����������� ������ ������������.
	wxStaticBox* configChangeModeStaticBox;	// ����� ��� ����������� ������ ����� ������������.

	BufferedBitmap* initSpinDistributionPanel;
	BufferedBitmap* finalSpinDistributionPanel;
	wxChartCtrl* energyChart;
	wxChartCtrl* heatCapacityChart;
	//std::vector<wxBitmap> 

	void initIsing();
	void applyParameters();
	void resetIsing();
	void metropolisForTempRange();
	void testModeSwitchButtons();
	void startTSingle();
	void startTRange();
	void clickedTestModeRadio1(wxCommandEvent& event);
	void clickedTestModeRadio2(wxCommandEvent& event);
	void clickedConfigChangeModeRadio1(wxCommandEvent& event);
	void clickedConfigChangeModeRadio2(wxCommandEvent& event);

	void clickedResetButton(wxCommandEvent& event);
	void clickedStartButton(wxCommandEvent& event);

	bool bTmRadio{ true };		// testModeRadio1 ���� true, ����� testModeRadio2.
	bool bCcmRadio{ true };		// configChangeModeRadio1 ���� true, ����� configChangeModeRadio2.
	
	Ising* isingModel{ nullptr };

};

