// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef ZOOMPANEL_H
#define ZOOMPANEL_H

#include <wx/bitmap.h>
#include <wx/panel.h>
#include <wx/dc.h>

/// Displays a zoomable region of the screen.
class ZoomPanel : public wxPanel
{
private:
    bool showPoi;
    int zoom;
    wxPoint poi;
    wxPoint roiCorner;
    wxBitmap image;

    void UpdateRoi();
public:
    ZoomPanel(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL);

    /// Get the current image.
    wxBitmap getImage() const;

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

    void render(wxPaintDC& dc);

    /// Get current point of interest on screen.
    wxPoint GetPoi() const;
    /// Set the current point of interest on screen/
    void SetPoi(const wxPoint& point);
    
    /// Set the magnification level.
    void SetZoom(int zoom);
    
    /// Whether or not to show the current point of interest.
    void ShowPoi(bool show);
    
    /// Get the magnification level.
    int GetZoom() const;

    /// Refresh the image.
    void Update();

    DECLARE_EVENT_TABLE()
};

#endif // ZOOMPANEL_H
