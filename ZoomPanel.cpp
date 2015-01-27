#include "ZoomPanel.h"
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <wx/dcmemory.h>

ZoomPanel::ZoomPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    showPoi = false;
    zoom = 4;
    SetCursor(*wxCROSS_CURSOR);
}


BEGIN_EVENT_TABLE(ZoomPanel, wxPanel)

    EVT_PAINT(ZoomPanel::paintEvent)

END_EVENT_TABLE()

void ZoomPanel::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void ZoomPanel::paintNow()
{
    wxPaintDC dc(this);
    render(dc);
}

void ZoomPanel::render(wxPaintDC& dc)
{
	 if (image.IsOk())
    {
		  dc.DrawBitmap(image, 0, 0, false);
        if (showPoi)
        {
            wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
            if (gc)
            {
                int relX = poi.x - roiCorner.x;
                int relY = poi.y - roiCorner.y;
                gc->SetPen(*wxWHITE_PEN);
                gc->SetBrush(wxNullBrush);
                wxGraphicsPath path = gc->CreatePath();
                path.AddCircle(relX * (double)zoom + zoom / 2, relY * (double)zoom + zoom / 2, 5);
                gc->StrokePath(path);
                path = gc->CreatePath();
                path.AddCircle(relX * (double)zoom + zoom / 2, relY * (double)zoom + zoom / 2, 4);
                gc->SetPen(*wxBLACK_PEN);
                gc->StrokePath(path);
                delete gc;
            }
        }
    }
}

wxPoint ZoomPanel::GetPoi() const
{
    return poi;
}


void ZoomPanel::SetPoi(const wxPoint& point)
{
    poi = point;
    ShowPoi(true);
    UpdateRoi();
    Update();
}

int ZoomPanel::GetZoom() const
{
    return zoom;
}

void ZoomPanel::SetZoom(int zoom) 
{
    this->zoom = zoom;
    UpdateRoi();
    Update();
}

void ZoomPanel::ShowPoi(bool show) {
    showPoi = show;
    Refresh();
}

void ZoomPanel::UpdateRoi() {
    wxSize screenSize = wxGetDisplaySize();
    wxSize zoomArea(128 / zoom, 128 / zoom);
    roiCorner.x = 0;
    roiCorner.y = 0;
    if (poi.x > zoomArea.x / 2) {
        if (poi.x > screenSize.x - zoomArea.x / 2)
            roiCorner.x = screenSize.x - zoomArea.x;
        else
            roiCorner.x = poi.x - zoomArea.x / 2;
    }
    if (poi.y > zoomArea.y / 2) {
        if (poi.y > screenSize.y - zoomArea.y / 2)
            roiCorner.y = screenSize.y - zoomArea.y;
        else
            roiCorner.y = poi.y - zoomArea.y / 2;
    }
}

void ZoomPanel::Update()
{
    wxSize zoomArea(128 / zoom, 128 / zoom);
    int x = roiCorner.x, y = roiCorner.y;
    wxBitmap bitmap(128, 128);
    wxScreenDC dc;
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.StretchBlit(0, 0, 128, 128, &dc, x, y, zoomArea.x, zoomArea.y);
    memDC.SelectObject(wxNullBitmap);
    image = bitmap;
    Refresh();
}