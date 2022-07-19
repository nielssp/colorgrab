// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#include "ZoomPanel.h"
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <wx/dcmemory.h>
#include <wx/display.h>

ZoomPanel::ZoomPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    showPoi = false;
    zoom = 4;
    SetCursor(*wxCROSS_CURSOR);
//    Bind(wxEVT_SIZE, &ZoomPanel::OnResize, this);
}


BEGIN_EVENT_TABLE(ZoomPanel, wxPanel)

    EVT_PAINT(ZoomPanel::paintEvent)
    EVT_SIZE(ZoomPanel::OnResize)

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

wxBitmap ZoomPanel::getImage() const
{
    return image;
}


void ZoomPanel::OnResize(wxSizeEvent& event)
{
    UpdateRoi();
    Update();
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

void ZoomPanel::ShowPoi(bool show)
{
    showPoi = show;
    Refresh();
}

void ZoomPanel::UpdateRoi()
{
    int idx = wxDisplay::GetFromPoint(poi);
    if (idx >= 0) {
        wxDisplay display(idx);
        wxRect geometry = display.GetGeometry();
        wxSize panelSize = GetSize();
        wxSize zoomArea(panelSize.x / zoom, panelSize.y / zoom);
        int relativeX = poi.x - geometry.x;
        int relativeY = poi.y - geometry.y;
        roiCorner.x = geometry.x;
        roiCorner.y = geometry.y;
        if (relativeX > zoomArea.x / 2) {
            if (relativeX > geometry.width - zoomArea.x / 2)
                roiCorner.x = geometry.x + geometry.width - zoomArea.x;
            else
                roiCorner.x = geometry.x + relativeX - zoomArea.x / 2;
        }
        if (relativeY > zoomArea.y / 2) {
            if (relativeY > geometry.height - zoomArea.y / 2)
                roiCorner.y = geometry.y + geometry.height - zoomArea.y;
            else
                roiCorner.y = geometry.y + relativeY - zoomArea.y / 2;
        }
    }
}

void ZoomPanel::Update()
{
    wxSize panelSize = GetSize();
    wxSize zoomArea(panelSize.x / zoom, panelSize.y / zoom);
    int x = roiCorner.x, y = roiCorner.y;
    wxBitmap bitmap(panelSize.x, panelSize.y);
    wxScreenDC dc;
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.StretchBlit(0, 0, panelSize.x, panelSize.y, &dc, x, y, zoomArea.x, zoomArea.y);
    memDC.SelectObject(wxNullBitmap);
#ifdef __WXGTK3__
    wxSize screenSize = dc.GetSize();
    dc.Blit(0, 0, screenSize.x, screenSize.y, &dc, 0, 0, wxXOR);
#endif
    image = bitmap;
    Refresh();
}
