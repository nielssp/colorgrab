#ifndef ZOOMPANEL_H
#define ZOOMPANEL_H

#include <wx/bitmap.h>
#include <wx/panel.h>
#include <wx/dc.h>

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

    wxBitmap getImage() const;

    void paintEvent(wxPaintEvent& evt);
    void paintNow();

    void render(wxPaintDC& dc);

    wxPoint GetPoi() const;
    void SetPoi(const wxPoint& point);
    
    void SetZoom(int zoom);
    
    void ShowPoi(bool show);
    
    int GetZoom() const;

    void Update();

    DECLARE_EVENT_TABLE()
};

#endif // ZOOMPANEL_H
