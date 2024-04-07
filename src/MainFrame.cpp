#include "MainFrame.hpp"
#include <wx/gbsizer.h>
#include <wx/valnum.h>
#include <vector>
#include <numeric>


void MainFrame::initIsing() {
	isingModel = new Ising();
	resetIsing();
	testModeSwitchButtons();
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(nullptr, wxID_ANY, title, pos, size) {

	

	wxFont boldFont = GetFont();
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
	wxFont normalFont = GetFont();

	wxBoxSizer* vertSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* mainHorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* controlsSizer = new wxFlexGridSizer(5, 3, 0, 0);
	wxBoxSizer* controlsVertSizer = new wxBoxSizer(wxVERTICAL);
	this->SetBackgroundColour(wxColour(0xf0f0f0));
	//wxPanel* leftPanel = new wxPanel(this);
	//wxPanel* rightPanel = new wxPanel(this);

	//wxPanel* dummySpinMap = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	//wxPanel* dummyChart = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	//dummySpinMap->SetBackgroundColour(wxColour("SLATE BLUE"));
	//dummyChart->SetBackgroundColour(wxColour("SPRING GREEN"));

	//vertSizer->Add(dummySpinMap, 1, wxEXPAND | wxALL, 10);
	//vertSizer->Add(dummyChart, 1, wxEXPAND | wxALL, 10);

	wxSize imagePanelDefaultSize = wxSize(FromDIP(256), FromDIP(256));
	wxGridBagSizer* graphicsGbSizer = new wxGridBagSizer(FromDIP(10), FromDIP(10));
	wxPanel* initSpinDistributionDummy = new wxPanel(this, wxID_ANY, wxDefaultPosition, imagePanelDefaultSize);
	wxPanel* finalSpinDistributionDummy = new wxPanel(this, wxID_ANY, wxDefaultPosition, imagePanelDefaultSize);
	wxStaticBoxSizer* initSpinDistributionBox = new wxStaticBoxSizer(wxVERTICAL, initSpinDistributionDummy, wxString("Начальное распределение"));
	wxStaticBoxSizer* finalSpinDistributionBox = new wxStaticBoxSizer(wxVERTICAL, finalSpinDistributionDummy, wxString("Конечное распределение"));
	initSpinDistributionBox->GetStaticBox()->SetFont(boldFont);
	finalSpinDistributionBox->GetStaticBox()->SetFont(boldFont);
	initSpinDistributionPanel = new BufferedBitmap(initSpinDistributionBox->GetStaticBox(), wxID_ANY, wxBitmap(imagePanelDefaultSize));
	finalSpinDistributionPanel = new BufferedBitmap(finalSpinDistributionBox->GetStaticBox(), wxID_ANY, wxBitmap(imagePanelDefaultSize));

	
	initSpinDistributionBox->Add(initSpinDistributionPanel, 1, wxSHAPED | wxALIGN_CENTER);
	finalSpinDistributionBox->Add(finalSpinDistributionPanel, 1, wxSHAPED | wxALIGN_CENTER);
	initSpinDistributionDummy->SetSizerAndFit(initSpinDistributionBox);
	finalSpinDistributionDummy->SetSizerAndFit(finalSpinDistributionBox);

	energyChart = new wxChartCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	heatCapacityChart = new wxChartCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	graphicsGbSizer->Add(initSpinDistributionDummy, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND);
	graphicsGbSizer->Add(finalSpinDistributionDummy, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND);
	graphicsGbSizer->Add(energyChart, wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND);
	graphicsGbSizer->Add(heatCapacityChart, wxGBPosition(2, 0), wxGBSpan(1, 2), wxEXPAND);
	graphicsGbSizer->AddGrowableCol(0);
	graphicsGbSizer->AddGrowableCol(1);
	graphicsGbSizer->AddGrowableRow(0);
	graphicsGbSizer->AddGrowableRow(1);
	graphicsGbSizer->AddGrowableRow(2);


	wxIntegerValidator<unsigned int> intValidator;
	intValidator.SetRange(1, 10000);
	wxIntegerValidator<unsigned long> intItersValidator;
	intItersValidator.SetMin(1);
	wxFloatingPointValidator<double> floatValidator(nullptr, wxNUM_VAL_NO_TRAILING_ZEROES);
	wxFloatingPointValidator<double> floatTempValidator(nullptr, wxNUM_VAL_NO_TRAILING_ZEROES);
	floatTempValidator.SetRange(0, 100);
	wxFloatingPointValidator<double> floatValidatorFraction(nullptr, wxNUM_VAL_NO_TRAILING_ZEROES);
	floatValidatorFraction.SetRange(0.01, 0.99);


	


	wxPanel* ctrlsDummyPanel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* ctrlDummySizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBox* ctrlStaticBox = new wxStaticBox(ctrlsDummyPanel, wxID_ANY, "");
	wxBoxSizer* ctrlBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* innerCtrlPanel = new wxPanel(ctrlStaticBox, wxID_ANY);
	sizeCtrl = new wxTextCtrl(innerCtrlPanel, wxID_ANY, "50", wxDefaultPosition, wxDefaultSize, 0, intValidator);
	jCtrl = new wxTextCtrl(innerCtrlPanel, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, 0, floatValidator);
	posFractionCtrl = new wxTextCtrl(innerCtrlPanel, wxID_ANY, "0.5", wxDefaultPosition, wxDefaultSize, 0, floatValidatorFraction);
	eqlItersCtrl = new wxTextCtrl(innerCtrlPanel, wxID_ANY, "1000", wxDefaultPosition, wxDefaultSize, 0, intItersValidator);
	avgItersCtrl = new wxTextCtrl(innerCtrlPanel, wxID_ANY, "1000", wxDefaultPosition, wxDefaultSize, 0, intItersValidator);


	controlsSizer->Add(new wxStaticText(innerCtrlPanel, wxID_ANY, "Линейный размер Р.Я. = ", wxDefaultPosition, wxSize(175,20), wxALIGN_RIGHT), 0, wxALIGN_CENTER_VERTICAL);
	controlsSizer->Add(sizeCtrl, 1, wxEXPAND | wxALL, FromDIP(5));
	controlsSizer->AddStretchSpacer(1);
	controlsSizer->Add(new wxStaticText(innerCtrlPanel, wxID_ANY, "Число МКШ до равновесия = ", wxDefaultPosition, wxSize(175, 20), wxALIGN_RIGHT), 0, wxALIGN_CENTER_VERTICAL);
	controlsSizer->Add(eqlItersCtrl, 1, wxEXPAND | wxALL, FromDIP(5));
	controlsSizer->AddStretchSpacer(1);
	controlsSizer->Add(new wxStaticText(innerCtrlPanel, wxID_ANY, "Число МКШ для усреднения = ", wxDefaultPosition, wxSize(175, 20), wxALIGN_RIGHT), 0, wxALIGN_CENTER_VERTICAL);
	controlsSizer->Add(avgItersCtrl, 1, wxEXPAND | wxALL, FromDIP(5));
	controlsSizer->AddStretchSpacer(1);
	controlsSizer->Add(new wxStaticText(innerCtrlPanel, wxID_ANY, "Доля (+) спинов = ", wxDefaultPosition, wxSize(175, 20), wxALIGN_RIGHT), 0, wxALIGN_CENTER_VERTICAL);
	controlsSizer->Add(posFractionCtrl, 1, wxEXPAND | wxALL, FromDIP(5));
	controlsSizer->AddStretchSpacer(1);
	controlsSizer->Add(new wxStaticText(innerCtrlPanel, wxID_ANY, "J = ", wxDefaultPosition, wxSize(175, 20), wxALIGN_RIGHT), 0, wxALIGN_CENTER_VERTICAL);
	controlsSizer->Add(jCtrl, 1, wxEXPAND | wxALL, FromDIP(5));
	controlsSizer->AddStretchSpacer(1);
	controlsSizer->AddGrowableCol(1);
	controlsSizer->AddGrowableCol(2);
	innerCtrlPanel->SetSizerAndFit(controlsSizer);
	ctrlBoxSizer->Add(innerCtrlPanel, 1, wxEXPAND | wxALL, FromDIP(10));
	ctrlStaticBox->SetSizerAndFit(ctrlBoxSizer);
	ctrlDummySizer->Add(ctrlStaticBox, 1, wxEXPAND);
	ctrlsDummyPanel->SetSizerAndFit(ctrlDummySizer);

	//
	//
	

	wxPanel* configStaticDummyPanel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* configDummyPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* configChangeModeStaticBoxVertSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* configChangeModeStaticBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	configChangeModeStaticBox = new wxStaticBox(configStaticDummyPanel, wxID_ANY, "Режим смены конфигурации спинов");
	configChangeModeRadio1 = new wxRadioButton(configChangeModeStaticBox, wxID_ANY, "Смена соседних спинов", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	configChangeModeRadio2 = new wxRadioButton(configChangeModeStaticBox, wxID_ANY, "Смена произвольных спинов");
	configChangeModeRadio1->Bind(wxEVT_RADIOBUTTON, &MainFrame::clickedConfigChangeModeRadio1, this);
	configChangeModeRadio2->Bind(wxEVT_RADIOBUTTON, &MainFrame::clickedConfigChangeModeRadio2, this);
	configChangeModeStaticBox->SetFont(boldFont);


	configChangeModeStaticBoxVertSizer->AddSpacer(FromDIP(15));
	configChangeModeStaticBoxSizer->Add(configChangeModeRadio1, 0, wxEXPAND | wxALL, FromDIP(5));
	configChangeModeStaticBoxSizer->Add(configChangeModeRadio2, 0, wxEXPAND | wxALL, FromDIP(5));
	configChangeModeStaticBoxVertSizer->Add(configChangeModeStaticBoxSizer);

	configChangeModeStaticBox->SetSizerAndFit(configChangeModeStaticBoxVertSizer);
	configDummyPanelSizer->Add(configChangeModeStaticBox);
	configStaticDummyPanel->SetSizerAndFit(configDummyPanelSizer);


	wxPanel* staticDummyPanel = new wxPanel(this, wxID_ANY);
	wxBoxSizer* dummyPanelSizer = new wxBoxSizer(wxVERTICAL);

	testModeStaticBox = new wxStaticBox(staticDummyPanel, wxID_ANY, "Режим эксперимента");
	testModeRadio1 = new wxRadioButton(testModeStaticBox, wxID_ANY, "Для одной температуры", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	testModeRadio2 = new wxRadioButton(testModeStaticBox, wxID_ANY, "Для интервала температуры");
	testModeRadio1->Bind(wxEVT_RADIOBUTTON, &MainFrame::clickedTestModeRadio1, this);
	testModeRadio2->Bind(wxEVT_RADIOBUTTON, &MainFrame::clickedTestModeRadio2, this);
	//wxPanel* singleTempPanel = new wxPanel(testModeStaticBox, wxID_ANY);
	//wxPanel* tempRangePanel = new wxPanel(testModeStaticBox, wxID_ANY);
	wxBoxSizer* singleTempSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer* tempRangeSizer = new wxFlexGridSizer(2, 3, 0, 0);
	wxBoxSizer* staticBoxSizer = new wxBoxSizer(wxVERTICAL);
	tRangeTicksCtrl = new wxTextCtrl(testModeStaticBox, wxID_ANY, "20", wxDefaultPosition, wxDefaultSize, 0, intValidator);
	tLowerCtrl = new wxTextCtrl(testModeStaticBox, wxID_ANY, "0.2", wxDefaultPosition, wxDefaultSize, 0, floatTempValidator);
	tUpperCtrl = new wxTextCtrl(testModeStaticBox, wxID_ANY, "1.5", wxDefaultPosition, wxDefaultSize, 0, floatTempValidator);
	tSingleCtrl = new wxTextCtrl(testModeStaticBox, wxID_ANY, "0.3", wxDefaultPosition, wxDefaultSize, 0, floatTempValidator);
	auto tStatic = new wxStaticText(testModeStaticBox, wxID_ANY, "T = ", wxDefaultPosition, wxSize(60, 20), wxALIGN_RIGHT);
	singleTempSizer->Add(tStatic, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	singleTempSizer->Add(tSingleCtrl, 2, wxEXPAND | wxALL, 5);
	singleTempSizer->AddStretchSpacer(2);
	auto t1t2Static = new wxStaticText(testModeStaticBox, wxID_ANY, "[T1, T2] = ", wxDefaultPosition, wxSize(60, 20), wxALIGN_RIGHT);
	tempRangeSizer->Add(t1t2Static,1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	tempRangeSizer->Add(tLowerCtrl,2, wxEXPAND | wxALL, 5);
	tempRangeSizer->Add(tUpperCtrl,2, wxEXPAND | wxALL, 5);
	auto tickStatic = new wxStaticText(testModeStaticBox, wxID_ANY, "Ticks = ", wxDefaultPosition, wxSize(60, 20), wxALIGN_RIGHT);
	tempRangeSizer->Add(tickStatic,1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	tempRangeSizer->Add(tRangeTicksCtrl,2, wxEXPAND | wxALL, 5);
	tempRangeSizer->AddStretchSpacer(2);
	tempRangeSizer->AddGrowableCol(1);
	tempRangeSizer->AddGrowableCol(2);
	staticBoxSizer->AddSpacer(FromDIP(15));
	staticBoxSizer->Add(testModeRadio1,0 , wxEXPAND | wxALL, 5);
	staticBoxSizer->Add(testModeRadio2,0, wxEXPAND | wxALL, 5);
	staticBoxSizer->Add(singleTempSizer, 0,wxEXPAND | wxLEFT | wxUP | wxRIGHT, 5);
	staticBoxSizer->Add(tempRangeSizer, 0, wxEXPAND | wxLEFT | wxDOWN | wxRIGHT, 5);
	testModeStaticBox->SetSizerAndFit(staticBoxSizer);
	dummyPanelSizer->Add(testModeStaticBox, 1, wxEXPAND);
	staticDummyPanel->SetSizerAndFit(dummyPanelSizer);
	testModeStaticBox->SetFont(boldFont);
	testModeRadio1->SetFont(normalFont);
	testModeRadio2->SetFont(normalFont);
	tStatic->SetFont(normalFont);
	t1t2Static->SetFont(normalFont);
	tickStatic->SetFont(normalFont);



	resetButton = new wxButton(this, wxID_ANY, "Принять/Сброс", wxDefaultPosition, wxDefaultSize);
	startButton = new wxButton(this, wxID_ANY, "Пуск", wxDefaultPosition, wxDefaultSize);
	resetButton->Bind(wxEVT_BUTTON, &MainFrame::clickedResetButton, this);
	startButton->Bind(wxEVT_BUTTON, &MainFrame::clickedStartButton, this);
	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonsSizer->Add(resetButton, 1, wxEXPAND | wxALL, FromDIP(5));
	buttonsSizer->Add(startButton, 1, wxEXPAND | wxALL, FromDIP(5));
	resetButton->SetFont(boldFont);
	startButton->SetFont(boldFont);

	tCritNumerical = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	tCritNumerical->Disable();
	wxBoxSizer* tCritSizer = new wxBoxSizer(wxHORIZONTAL);
	tCritSizer->Add(new wxStaticText(this, wxID_ANY, "Ткрит. = ", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 1, wxEXPAND | wxALL, FromDIP(5));
	tCritSizer->Add(tCritNumerical, 1, wxEXPAND | wxALL, FromDIP(5));

	controlsVertSizer->Add(ctrlsDummyPanel, 0 ,wxEXPAND);
	controlsVertSizer->AddSpacer(FromDIP(10));
	controlsVertSizer->Add(configStaticDummyPanel, 0, wxEXPAND);
	controlsVertSizer->AddSpacer(FromDIP(10));
	controlsVertSizer->Add(staticDummyPanel, 0, wxEXPAND);
	controlsVertSizer->AddSpacer(FromDIP(10));
	controlsVertSizer->Add(buttonsSizer, 0, wxEXPAND);
	controlsVertSizer->Add(tCritSizer, 0, wxEXPAND);

	mainHorSizer->Add(graphicsGbSizer, 1, wxEXPAND | wxALL, FromDIP(10));
	mainHorSizer->Add(controlsVertSizer, 0, wxALL, FromDIP(20));


	this->SetSizerAndFit(mainHorSizer);
	this->Layout();

	initIsing();

	CreateStatusBar(1);
	SetStatusText("READY", 0);
	SetSize(wxSize(1200, 800));
}

void MainFrame::testModeSwitchButtons() {
	if (bTmRadio) {
		tSingleCtrl->Enable();
		tRangeTicksCtrl->Disable();
		tLowerCtrl->Disable();
		tUpperCtrl->Disable();
		//resetButton->Enable();
	}
	else {
		tSingleCtrl->Disable();
		tRangeTicksCtrl->Enable();
		tLowerCtrl->Enable();
		tUpperCtrl->Enable();
		//resetButton->Disable();
	}
}
void MainFrame::clickedTestModeRadio1(wxCommandEvent& event) {
	bTmRadio = true;
	isingModel->bTSingle = true;
	testModeSwitchButtons();
}

void MainFrame::clickedTestModeRadio2(wxCommandEvent& event) {
	bTmRadio = false;
	isingModel->bTSingle = false;
	testModeSwitchButtons();
}

void MainFrame::clickedConfigChangeModeRadio1(wxCommandEvent& event) {
	bCcmRadio = true;
	isingModel->bSwapNeighbours = true;
}
void MainFrame::clickedConfigChangeModeRadio2(wxCommandEvent& event) {
	bCcmRadio = false;
	isingModel->bSwapNeighbours = false;
}

void MainFrame::resetIsing() {
	applyParameters();

	if (bTmRadio) {
		isingModel->Reset();
		initSpinDistributionPanel->SetBitmap(SpinDistribToBitmap(isingModel->theCell));
		initSpinDistributionPanel->Refresh();

		finalSpinDistributionPanel->SetBitmap(wxBitmap(wxSize(1,1)));
		finalSpinDistributionPanel->Refresh();
	}
	else {
		isingModel->Reset();
		initSpinDistributionPanel->SetBitmap(wxBitmap(wxSize(1, 1)));
		initSpinDistributionPanel->Refresh();

		finalSpinDistributionPanel->SetBitmap(wxBitmap(wxSize(1, 1)));
		finalSpinDistributionPanel->Refresh();
	}
}
void MainFrame::clickedResetButton(wxCommandEvent& event) {
	resetIsing();
	
}

void MainFrame::startTSingle() {
	double T;
	if (!tSingleCtrl->GetValue().ToDouble(&T)) {
		return;
	}


	isingModel->Metropolis(isingModel->tCritTheor * T);
	finalSpinDistributionPanel->SetBitmap(SpinDistribToBitmap(isingModel->theCell));
	finalSpinDistributionPanel->Refresh();
	std::vector<double> MCSPoints;
	for (int i = 0; i < isingModel->energyDuringEql.size(); i++) {
		MCSPoints.push_back(i);
	}

	heatCapacityChart->ClearCurves();
	heatCapacityChart->SetTitle(wxString(""));
	energyChart->ClearCurves();
	energyChart->AddCurve(MCSPoints, isingModel->energyDuringEql, wxColour("SLATE BLUE"));
	energyChart->SetTitle(wxString("Зависимость E от МКШ"));
	energyChart->Refresh();
	tCritNumerical->SetValue(wxString(""));

}

#include <fstream>
void MainFrame::startTRange() {

	resetIsing();
	unsigned long ticks;
	if (!tRangeTicksCtrl->GetValue().ToULong(&ticks)) {
		return;
	}

	double lowerT;
	if (!tLowerCtrl->GetValue().ToDouble(&lowerT)) {
		return;
	}

	double upperT;
	if (!tUpperCtrl->GetValue().ToDouble(&upperT)) {
		return;
	}

	double dT{ (upperT - lowerT) / (ticks + 1) };

	std::vector<double> temperaturePoints;
	std::vector<double> avgEnergyVals;	// <E>
	std::vector<double> avgEnergy2Vals;	// <E^2>
	std::vector<double> energyVariance;	// <E^2> - <E>^2
	double cScale{ isingModel->cellLinSize * isingModel->cellLinSize * isingModel->k * isingModel->k * isingModel->tCritTheor * isingModel->tCritTheor };	// Знаменатель в выражении для C(T).

	double T{};
	double avgEnergy{};
	double avgEnergy2{};

	

	for (int i = 0; i < ticks + 2; i++) {
		T = isingModel->tCritTheor * (lowerT + dT * i);
		isingModel->Metropolis(T);
		
		avgEnergy = std::accumulate(isingModel->energyVals.begin(), isingModel->energyVals.end(), 0.0) / isingModel->energyVals.size();
		avgEnergy2 = std::accumulate(isingModel->energy2Vals.begin(), isingModel->energy2Vals.end(), 0.0) / isingModel->energy2Vals.size();
		temperaturePoints.push_back(lowerT + dT * i);
		avgEnergyVals.push_back(avgEnergy);
		avgEnergy2Vals.push_back(avgEnergy2);

	}
	
	for (int i = ticks + 1; i >= 0; i--) {
		T = isingModel->tCritTheor * (lowerT + dT * i);

		isingModel->Metropolis(T);
		avgEnergy = std::accumulate(isingModel->energyVals.begin(), isingModel->energyVals.end(), 0.0) / isingModel->energyVals.size();
		avgEnergyVals[i] = (avgEnergyVals[i] + avgEnergy) / 2;
		avgEnergy2 = std::accumulate(isingModel->energy2Vals.begin(), isingModel->energy2Vals.end(), 0.0) / isingModel->energy2Vals.size();
		avgEnergy2Vals[i] = (avgEnergy2Vals[i] + avgEnergy2) / 2;
	}

	for (int i = 0; i < ticks + 2; i++) {
		//energyVariance.push_back((avgEnergy2Vals[i] - avgEnergyVals[i] * avgEnergyVals[i]) / (cScale * temperaturePoints[i] * temperaturePoints[i]));
		if (i == 0) {
			energyVariance.push_back((avgEnergyVals[i + 1] - avgEnergyVals[i]) / (dT * isingModel->cellLinSize * isingModel->cellLinSize));
		}
		else if (i == ticks + 1) {
			energyVariance.push_back((avgEnergyVals[i] - avgEnergyVals[i - 1]) / (dT * isingModel->cellLinSize * isingModel->cellLinSize));
		}
		else { 
			energyVariance.push_back((avgEnergyVals[i + 1] - avgEnergyVals[i - 1]) / (2 * dT * isingModel->cellLinSize * isingModel->cellLinSize));
		}
	
	}


	for (int i = 0; i < ticks + 2; i++) {
		avgEnergyVals[i] /= isingModel->cellLinSize * isingModel->cellLinSize;

	}

	energyChart->ClearCurves();
	energyChart->AddCurve(temperaturePoints, avgEnergyVals, wxColour("SLATE BLUE"));
	energyChart->SetTitle(wxString(L"Зависимость ε от T"));
	energyChart->Refresh();
	heatCapacityChart->ClearCurves();
	heatCapacityChart->AddCurve(temperaturePoints, energyVariance, wxColour("VIOLET RED"));
	heatCapacityChart->SetTitle(wxString("Зависимость C от T"));
	heatCapacityChart->Refresh();
	int maxElemPos = std::distance(energyVariance.begin(), std::max_element(energyVariance.begin(), energyVariance.end()));
	tCritNumerical->SetValue(std::to_string(temperaturePoints[maxElemPos]));

	std::ofstream myfile;
	myfile.open("E.txt");
	for (auto v : avgEnergyVals) {
		myfile << v << "\t\t\t" << v * v << '\n';
	}
	myfile.close();

	std::ofstream myfile2;
	myfile2.open("E2.txt");
	for (auto v : avgEnergy2Vals) {
		myfile2 << v << '\n';
	}
	myfile2.close();


}


void MainFrame::clickedStartButton(wxCommandEvent& event) {
	if (bTmRadio) {
		startTSingle();
	}
	else {
		startTRange();
	}
}


void MainFrame::applyParameters() {
	unsigned long N;
	if (!sizeCtrl->GetValue().ToULong(&N)) {
		return;
	}

	double J;
	if (!jCtrl->GetValue().ToDouble(&J)) {
		return;
	}

	double posFraction;
	if (!posFractionCtrl->GetValue().ToDouble(&posFraction)) {
		return;
	}

	unsigned long n0;
	if (!eqlItersCtrl->GetValue().ToULong(&n0)) {
		return;
	}

	unsigned long n;
	if (!avgItersCtrl->GetValue().ToULong(&n)) {
		return;
	}


	isingModel->Set(N, J, posFraction, n0, n);

}