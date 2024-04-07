#pragma once
#include <wx/wx.h>
#include <array>

class Curve {
public:
	std::vector<wxPoint2DDouble> points;
	wxColour curveColor;

	Curve(std::vector<double> x, std::vector<double> y, wxColour curveColor);
	void SetColor(wxColour curveColor);
	void SetPoints(std::vector<double> x, std::vector<double> y);
};


class wxChartCtrl : public wxWindow
{
public:
	wxChartCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size);
	void SetTitle(wxString title);
	void FitPoints();
	void AddCurve(std::vector<double>& x, std::vector<double>& y, wxColour curveColor);
	void ClearCurves();
private:
	void OnPaint(wxPaintEvent& evt);
	std::vector<Curve> curves;
	wxString title;
	double yLowValue;
	double yHighValue;
	double yValueSpan;

	double xLowValue;
	double xHighValue;
	double xValueSpan;

};

