#pragma once

#include "wx\wx.h"
#include "wx\graphics.h"
#include "wx\dcbuffer.h"
#include <vector>


class BufferedBitmap : public wxScrolled<wxWindow>
{
public:
    BufferedBitmap(wxWindow* parent, wxWindowID id, const wxBitmap& b, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
    void OnPaint(wxPaintEvent& evt);
    void SetBitmap(const wxBitmap& bitmap);
    void SetRawBitmap(std::vector<std::vector<double>>);
    const wxBitmap& GetBitmap() const;
    double GetZoomMultiplier() const;
    double GetZoomPercentage() const;
    void ZoomIn();
    void ZoomOut();
    void SetCircleAreaSize(double rad);
    void ShowFilterArea(bool enable = true);
private:
    wxBitmap bitmap;
    wxBitmap pixelDrawingBitmap;
    const double ZOOM_FACTOR = 1.1;
    int zoomLevel = 0;
    bool bRawBitmap = false; // Bitmap for pixel drawing.

    bool drawFilterArea = 0;
    double filterAreaRad;
    double filterAreaWHRatio;

    wxSize GetScaledBitmapSize() const;
    void CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter);
private:
    void OnMouseMove(wxMouseEvent& evt);
    void OnLMouseDown(wxMouseEvent& evt);
    void OnLMouseUp(wxMouseEvent& evt);
    void OnWindowLeave(wxMouseEvent& evt);
    void OnMouseZoom(wxMouseEvent& evt);
    void OnRMouseDClick(wxMouseEvent& evt);
    bool bLMouseDown = false;
    wxPoint prevPt;
    wxPoint offset{0,0};
    wxPoint rawOffset;
    wxDouble zoomScale = 1;
    void resetToOrigin();

    wxDECLARE_EVENT_TABLE();
};


wxBitmap GetBitmap(std::vector<std::vector<double>> matrix);

wxBitmap SpinDistribToBitmap(std::vector < std::vector<int>> &spinDistrib);

std::vector<std::vector<double>> GetNormalizedMatrix(std::vector<std::vector<double>> matrix);

double GetMax(std::vector<std::vector<double>>& matrix);

double GetMin(std::vector<std::vector<double>>& matrix);