#include "wxChartCtrl.hpp"
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <tuple>
#include <algorithm>

wxChartCtrl::wxChartCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size) : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->Bind(wxEVT_PAINT, &wxChartCtrl::OnPaint, this);

	title = "";
	xLowValue = xHighValue = yLowValue = yHighValue = xValueSpan = yValueSpan = 0;
}

void wxChartCtrl::OnPaint(wxPaintEvent& evt) {
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();

	//wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	wxGraphicsRenderer* renderer = wxGraphicsRenderer::GetDirect2DRenderer();
	wxGraphicsContext* gc = renderer->CreateContext(dc);
	//
	//yHighValue = 1.40;
	//yLowValue = -0.60;
	//xLowValue = -1.45;
	//xHighValue = 13.86;
	//xValueSpan = xHighValue - xLowValue;
	//yValueSpan = yHighValue - yLowValue;
	//

	if (gc) {
		wxSize size = this->GetSize();

		wxFont titleFont = wxFont(wxNORMAL_FONT->GetPointSize() * 2.0, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

		gc->SetFont(titleFont, *wxBLACK);

		double tw, th;

		gc->GetTextExtent(title, &tw, &th);

		const double titleTopBottomMinimymMargin = this->FromDIP(10);

		wxRect2DDouble fullArea(0, 0, static_cast<double>(GetSize().GetWidth()), static_cast<double>(GetSize().GetHeight()));

		const double marginX = fullArea.GetSize().GetWidth() / 8.0;
		const double marginTop = std::max(fullArea.GetSize().GetHeight() / 8.0, titleTopBottomMinimymMargin * 2.0 + th);
		const double marginBottom = fullArea.GetSize().GetHeight() / 8.0;
		double labelsToChartAreaMargin = this->FromDIP(10);

		wxRect2DDouble chartArea = fullArea;
		chartArea.Inset(marginX, marginTop, marginX, marginBottom);
		gc->DrawText(title, (fullArea.GetSize().GetWidth() - tw) / 2.0, (marginTop - th) / 2.0);

		gc->SetBrush(*wxWHITE_BRUSH);
		gc->DrawRectangle(chartArea.m_x, chartArea.m_y, chartArea.m_width, chartArea.m_height);

		wxAffineMatrix2D normalizedToChartArea;
		normalizedToChartArea.Translate(chartArea.GetLeft(), chartArea.GetTop());
		normalizedToChartArea.Scale(chartArea.m_width, chartArea.m_height);

		wxAffineMatrix2D normalizedToValue;
		normalizedToValue.Translate(xLowValue, yHighValue);
		normalizedToValue.Scale(1, -1);
		normalizedToValue.Scale(xValueSpan, yValueSpan);

		const int LinesCount = 11;

		gc->SetPen(wxPen(wxColour(128, 128, 128), 1, wxPENSTYLE_LONG_DASH));
		wxFont tickLabelFont = wxFont(wxNORMAL_FONT->GetPointSize() * 1.2, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		gc->SetFont(tickLabelFont, *wxBLACK);

		for (int i = 0; i < LinesCount; i++) {
			double normalizedLine = static_cast<double>(i) / (LinesCount - 1);

			auto yLineStartPoint = normalizedToChartArea.TransformPoint({ 0, normalizedLine });
			auto yLineEndPoint = normalizedToChartArea.TransformPoint({ 1, normalizedLine });

			wxPoint2DDouble linePointsY[] = { yLineStartPoint, yLineEndPoint };

			gc->StrokeLines(2, linePointsY);

			auto xLineStartPoint = normalizedToChartArea.TransformPoint({ normalizedLine, 0 });
			auto xLineEndPoint = normalizedToChartArea.TransformPoint({ normalizedLine, 1 });

			wxPoint2DDouble linePointsX[] = { xLineStartPoint, xLineEndPoint };

			gc->StrokeLines(2, linePointsX);

			double value = normalizedToValue.TransformPoint({ 0, normalizedLine }).m_y;
			auto text = wxString::Format("%.2f", value);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() - labelsToChartAreaMargin);

			double tw, th;
			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, chartArea.GetLeft() - labelsToChartAreaMargin - tw, yLineStartPoint.m_y - th / 2.0);

			value = normalizedToValue.TransformPoint({ normalizedLine, 0 }).m_x;
			text = wxString::Format("%.2f", value);
			text = wxControl::Ellipsize(text, dc, wxELLIPSIZE_MIDDLE, chartArea.GetLeft() - labelsToChartAreaMargin);

			gc->GetTextExtent(text, &tw, &th);
			gc->DrawText(text, xLineStartPoint.m_x - tw / 2.0, chartArea.GetBottom() + labelsToChartAreaMargin);
		}

		wxPoint2DDouble* transformedPoints = nullptr;
		std::vector<wxPoint2DDouble> test;
		wxAffineMatrix2D valueToNormalized = normalizedToValue;
		valueToNormalized.Invert();
		wxAffineMatrix2D valueToChartArea = normalizedToChartArea;
		valueToChartArea.Concat(valueToNormalized);


		for (auto& curve : curves) {
			gc->SetPen(curve.curveColor);
			transformedPoints = new wxPoint2DDouble[curve.points.size()];

			for (int i = 0; i < curve.points.size(); i++) {
				transformedPoints[i] = valueToChartArea.TransformPoint(curve.points[i]);
				test.push_back(transformedPoints[i]);
			}

			gc->StrokeLines(curve.points.size(), transformedPoints);
			delete[] transformedPoints;
		}

		delete gc;
	}
}

void wxChartCtrl::SetTitle(wxString title) {
	this->title = title;
}

void wxChartCtrl::AddCurve(std::vector<double>& x, std::vector<double>& y, wxColour curveColor = wxColour("BLACK")) {
	curves.push_back(Curve(x, y, curveColor));
	FitPoints();
	
}

void wxChartCtrl::ClearCurves() {
	curves.clear();
}

void wxChartCtrl::FitPoints() {
	if (curves.empty()) return;
	std::vector<double> yAllLowest;
	std::vector<double> yAllHighest;
	std::vector<double> xAllLowest;
	std::vector<double> xAllHighest;
	for (auto& curve : curves) {
		auto yLowest = std::min_element(curve.points.begin(), curve.points.end(), [](const wxPoint2DDouble a, const wxPoint2DDouble b) {
			return a.m_y < b.m_y;
			});

		auto yHighest = std::max_element(curve.points.begin(), curve.points.end(), [](const wxPoint2DDouble a, const wxPoint2DDouble b) {
			return a.m_y < b.m_y;
			});
		yAllLowest.push_back(yLowest->m_y);
		yAllHighest.push_back(yHighest->m_y);

		auto xLowest = std::min_element(curve.points.begin(), curve.points.end(), [](const wxPoint2DDouble a, const wxPoint2DDouble b) {
			return a.m_x < b.m_x;
			});

		auto xHighest = std::max_element(curve.points.begin(), curve.points.end(), [](const wxPoint2DDouble a, const wxPoint2DDouble b) {
			return a.m_x < b.m_x;
			});
		xAllLowest.push_back(xLowest->m_x);
		xAllHighest.push_back(xHighest->m_x);
	}

	yLowValue = *std::min_element(yAllLowest.begin(), yAllLowest.end());
	yHighValue = *std::max_element(yAllHighest.begin(), yAllHighest.end());
	yValueSpan = yHighValue - yLowValue;

	xLowValue = *std::min_element(xAllLowest.begin(), xAllLowest.end());
	xHighValue = *std::max_element(xAllHighest.begin(), xAllHighest.end());
	xValueSpan = xHighValue - xLowValue;
}


Curve::Curve(std::vector<double> x, std::vector<double> y, wxColour curveColor = wxColour("BLACK")) {

	for (auto [itx, ity] = std::tuple{ x.begin(), y.begin() }; itx != x.end() && ity != y.end(); ++itx, ++ity) {
		points.push_back({ *itx, *ity });
	}
	this->curveColor = curveColor;
}

void Curve::SetPoints(std::vector<double> x, std::vector<double> y) {
	points.clear();
	for (auto [itx, ity] = std::tuple{ x.begin(), y.begin() }; itx != x.end() && ity != y.end(); ++itx, ++ity) {
		points.push_back({ *itx, *ity });
	}
}

void Curve::SetColor(wxColour curveColor) {
	this->curveColor = curveColor;
}


