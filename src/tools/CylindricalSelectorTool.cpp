// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#include "CylindricalSelectorTool.h"

#include "MainFrame.h"
#include "colormodels.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <wx/dcmemory.h>

class HueSlider : public wxPanel
{
public:
    HueSlider(wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL)
        : wxPanel(parent, id, pos, size, style)
    {
        SetCursor(*wxCROSS_CURSOR);
        SetMinSize(wxSize(20, 100));
        Bind(wxEVT_PAINT, &HueSlider::OnPaint, this);
        Bind(wxEVT_SIZE, &HueSlider::OnResize, this);
    }
    
    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        Render(dc);
    }
    
    void OnResize(wxSizeEvent& event)
    {
        Refresh(false);
    }
    
    void Render(wxPaintDC& dc)
    {
        wxSize size = dc.GetSize();
        for (int y = 0; y < size.y; y++) {
            double hue = y / (double) size.y;
            dc.SetPen(wxPen(wxColour(
                hueToRgb(0, 1, hue + 1.0 / 3.0),
                hueToRgb(0, 1, hue),
                hueToRgb(0, 1, hue - 1.0 / 3.0)
            )));
            dc.DrawLine(0, y, size.x, y);
        }
    }
};

CylindricalSelectorTool::CylindricalSelectorTool(MainFrame* main) : ToolWindow(main, wxID_ANY, _("Cylindrical Selector Tool"), wxDefaultPosition, wxSize(480, 300))
{
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    boxSizer->Add(mainPanel, 1, wxEXPAND, 5);
    
    wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 5, 5);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(1);
    
    wxStaticText *text = new wxStaticText(mainPanel, wxID_ANY, wxT(""));
    sizer->Add(text);
    
    wxChoice *choice = new wxChoice(mainPanel, wxID_ANY);
    sizer->Add(choice, wxSizerFlags().Expand());
    
    HueSlider *hueSlider = new HueSlider(mainPanel, wxID_ANY);
    sizer->Add(hueSlider, wxSizerFlags().Expand());
    
    text = new wxStaticText(mainPanel, wxID_ANY, wxT("sat/val"));
    sizer->Add(text);
    
    mainPanel->SetSizerAndFit(sizer);
    SetSizerAndFit(boxSizer);
}

std::string CylindricalSelectorTool::GetName()
{
    return "CylindricalSelectorTool";
}
