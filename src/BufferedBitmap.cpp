#include "BufferedBitmap.hpp"
#include "wx\rawbmp.h"

BufferedBitmap::BufferedBitmap(wxWindow* parent, wxWindowID id, const wxBitmap& b, const wxPoint& pos, const wxSize& size, long style)
    : wxScrolled<wxWindow>(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE | wxVSCROLL | wxHSCROLL | style)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows
    this->Bind(wxEVT_PAINT, &BufferedBitmap::OnPaint, this);
    this->SetBitmap(b);
    this->offset = wxPoint(0, 0);
}

void BufferedBitmap::OnPaint(wxPaintEvent& evt)
{

    this->Refresh();
    wxAutoBufferedPaintDC dc(this);


    dc.Clear();

    DoPrepareDC(dc);

    //wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    wxGraphicsRenderer* renderer = wxGraphicsRenderer::GetDirect2DRenderer();
    wxGraphicsContext* gc = renderer->CreateContext(dc);

    wxGraphicsBitmap preparedBitmap = gc->CreateBitmap(bitmap);
  
  
    
    


    //gc->SetAntialiasMode(wxANTIALIAS_NONE);
    gc->SetInterpolationQuality(wxINTERPOLATION_NONE);



    if (gc)
    {
        // scaling consistent with wxStaticBitmap
        const wxSize drawSize = ToDIP(GetVirtualSize());

        const wxSize bmpSize = GetScaledBitmapSize();

        double w = bmpSize.GetWidth();
        double h = bmpSize.GetHeight();

        double x = (drawSize.GetWidth() - w) / 2;
        double y = (drawSize.GetHeight() - h) / 2;

        // For offset bug

        //wxSize bmpOffset = wxSize(gc->FromDIP(w / (testBmp.GetWidth() * 2.0)), gc->FromDIP(h / (testBmp.GetHeight() * 2.0)));

        //////////////////////////////////////////

        gc->Scale(1, -1);
        gc->Translate(0, -h);
        gc->Translate(offset.x - (w * zoomScale / 2 - w / 2), -offset.y - (h * zoomScale / 2 - h / 2));
        gc->Scale(zoomScale, zoomScale);
 
        gc->DrawBitmap(preparedBitmap, 0, 0, w, h);

        delete gc;
    }


}

void BufferedBitmap::resetToOrigin() {
    zoomScale = 1;
    offset = { 0,0 };
}

void BufferedBitmap::SetBitmap(const wxBitmap& bitmap)
{
    this->bitmap = bitmap;

    SetScrollRate(FromDIP(10), FromDIP(10));
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));
    resetToOrigin();

    this->Refresh();
}

const wxBitmap& BufferedBitmap::GetBitmap() const
{
    return bitmap;
}

double BufferedBitmap::GetZoomMultiplier() const
{
    return pow(ZOOM_FACTOR, zoomLevel);
}

double BufferedBitmap::GetZoomPercentage() const
{
    return GetZoomMultiplier() * 100;
}

void BufferedBitmap::ZoomIn()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

    zoomLevel++;

    CenterAfterZoom(centerPos, centerPos * ZOOM_FACTOR);
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));

    this->Refresh();
}

void BufferedBitmap::ZoomOut()
{
    auto centerPos = CalcUnscrolledPosition(wxPoint(GetClientSize().GetWidth() / 2, GetClientSize().GetHeight() / 2));

    zoomLevel--;

    CenterAfterZoom(centerPos, centerPos * (1.0 / ZOOM_FACTOR));
    SetVirtualSize(FromDIP(GetScaledBitmapSize()));

    this->Refresh();
}

wxSize BufferedBitmap::GetScaledBitmapSize() const
{
    //const wxSize bmpSize = bitmap.GetSize();
    const wxSize bmpSize = GetSize();
    const double zoom = GetZoomMultiplier();
    return wxSize(bmpSize.GetWidth() * zoom, bmpSize.GetHeight() * zoom);
}

void BufferedBitmap::CenterAfterZoom(wxPoint previousCenter, wxPoint currentCenter)
{
    wxPoint pixelsPerUnit;
    GetScrollPixelsPerUnit(&pixelsPerUnit.x, &pixelsPerUnit.y);

    auto delta = currentCenter - previousCenter;

    auto destX = GetViewStart().x + delta.x / pixelsPerUnit.x;
    auto destY = GetViewStart().y + delta.y / pixelsPerUnit.y;

    Scroll(destX, destY);
}


wxBEGIN_EVENT_TABLE(BufferedBitmap, wxScrolled<wxWindow>)
EVT_MOTION(BufferedBitmap::OnMouseMove)
EVT_LEFT_DOWN(BufferedBitmap::OnLMouseDown)
EVT_LEFT_UP(BufferedBitmap::OnLMouseUp)
EVT_LEAVE_WINDOW(BufferedBitmap::OnWindowLeave)
EVT_MOUSEWHEEL(BufferedBitmap::OnMouseZoom)
EVT_RIGHT_DCLICK(BufferedBitmap::OnRMouseDClick)
wxEND_EVENT_TABLE()


void BufferedBitmap::OnMouseMove(wxMouseEvent& evt) {
    if (!bLMouseDown) return;
    auto curPt = evt.GetPosition();
    rawOffset.x += (curPt.x - prevPt.x);
    rawOffset.y += (curPt.y - prevPt.y);
    offset.x += curPt.x - prevPt.x;
    offset.y += curPt.y - prevPt.y;
    prevPt = curPt;
    Refresh();
}

void BufferedBitmap::OnLMouseDown(wxMouseEvent& evt) {
    bLMouseDown = true;
    prevPt = evt.GetPosition();
}

void BufferedBitmap::OnLMouseUp(wxMouseEvent& evt) {
    bLMouseDown = false;
}

void BufferedBitmap::OnRMouseDClick(wxMouseEvent& evt) {
    resetToOrigin();
    Refresh();
}

void BufferedBitmap::OnWindowLeave(wxMouseEvent& evt) {
    bLMouseDown = false;
}

void BufferedBitmap::OnMouseZoom(wxMouseEvent& evt) {
    int delta = evt.GetWheelRotation();
    wxPoint mousePos = evt.GetPosition();
    wxSize bmpSize = GetSize();
    // Setting origin to the center of the screen and inverting y axis.
    mousePos.y = bmpSize.y - mousePos.y - bmpSize.y / 2;
    mousePos.x = mousePos.x - bmpSize.x / 2;
    // Shift needed to zoom to the cursor position.
    wxSize shiftToCursor(0, 0);



    wxDouble zoomPrev = zoomScale;
    if (delta > 0) {
        zoomScale *= 1.25;

        shiftToCursor.x = -(mousePos.x * zoomScale / zoomPrev - mousePos.x);
        shiftToCursor.y = (mousePos.y * zoomScale / zoomPrev - mousePos.y);

        offset.x = (offset.x) * zoomScale / zoomPrev + shiftToCursor.x;
        offset.y = (offset.y) * zoomScale / zoomPrev + shiftToCursor.y;
    }
    else {

        shiftToCursor.x = -(mousePos.x * zoomScale / zoomPrev - mousePos.x);
        shiftToCursor.y = (mousePos.y * zoomScale / zoomPrev - mousePos.y);

        zoomScale = (zoomScale / 1.25) > 1.0 ? (zoomScale / 1.25) : 1.0;
        offset.x = (offset.x) * zoomScale / zoomPrev + shiftToCursor.x;
        offset.y = (offset.y) * zoomScale / zoomPrev + shiftToCursor.y;
    }
    Refresh();
}

void BufferedBitmap::SetCircleAreaSize(double rad) {
    filterAreaRad = rad;
}

void BufferedBitmap::ShowFilterArea(bool enable) {
    drawFilterArea = enable;
}



wxBitmap GetBitmap(std::vector<std::vector<double>> matrix) {


    wxBitmap matBitmap(matrix[0].size(), matrix.size(), 24);
    wxNativePixelData rawBmp(matBitmap);
    wxNativePixelData::Iterator p(rawBmp);

    matrix = GetNormalizedMatrix(matrix);

    for (int i = 0; i < matrix.size(); i++) {
        wxNativePixelData::Iterator rowStart = p;
        for (int j = 0; j < matrix[0].size(); j++, p++) {

            p.Red() = int(matrix[i][j] * 255);
            p.Green() = int(matrix[i][j] * 255);
            p.Blue() = int(matrix[i][j] * 255);
        }
        p = rowStart;
        p.OffsetY(rawBmp, 1);
    }

    return matBitmap;
}


std::vector<std::vector<double>> GetNormalizedMatrix(std::vector<std::vector<double>> matrix) {
    double maxVal = GetMax(matrix);
    double minVal = GetMin(matrix);

    double normConst = abs(maxVal) > abs(minVal) ? abs(maxVal) : abs(minVal);
    if (normConst == 0) return matrix;
    for (auto& row : matrix) {
        for (auto& colElem : row) {
            colElem /= normConst;
        }
    }
    return matrix;
}

double GetMax(std::vector<std::vector<double>>& matrix) {
    std::vector<double> maxEachRow;
    for (auto& row : matrix) {
        maxEachRow.push_back(*max_element(row.begin(), row.end()));
    }

    return *max_element(maxEachRow.begin(), maxEachRow.end());
}

double GetMin(std::vector<std::vector<double>>& matrix) {
    std::vector<double> minEachRow;
    for (auto& row : matrix) {
        minEachRow.push_back(*min_element(row.begin(), row.end()));
    }

    return *min_element(minEachRow.begin(), minEachRow.end());
}

wxBitmap SpinDistribToBitmap(std::vector < std::vector<int>> &spinDistrib) {

    wxBitmap matBitmap(spinDistrib[0].size(), spinDistrib.size(), 24);
    wxNativePixelData rawBmp(matBitmap);
    wxNativePixelData::Iterator p(rawBmp);

    constexpr unsigned char upwardSpinR = 178;
    constexpr unsigned char upwardSpinG = 34;
    constexpr unsigned char upwardSpinB = 34;

    constexpr unsigned char downwardSpinR = 30;
    constexpr unsigned char downwardSpinG = 144;
    constexpr unsigned char downwardSpinB = 255;

    for (int i = 0; i < spinDistrib.size(); i++) {
        wxNativePixelData::Iterator rowStart = p;
        for (int j = 0; j < spinDistrib[0].size(); j++, p++) {

            if (spinDistrib[i][j] == 1) {
                p.Red() = upwardSpinR;
                p.Green() = upwardSpinG;
                p.Blue() = upwardSpinB;
            }
            else {
                p.Red() = downwardSpinR;
                p.Green() = downwardSpinG;
                p.Blue() = downwardSpinB;
            }
        }
        p = rowStart;
        p.OffsetY(rawBmp, 1);
    }

    return matBitmap;
}