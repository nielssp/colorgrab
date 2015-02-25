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
private:
    CylindricalSelectorTool* tool;
public:
    HueSlider(CylindricalSelectorTool* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL)
        : wxPanel(parent, id, pos, size, style)
    {
        tool = parent;
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
            double hue = 1.0 - y / (double) size.y;
            dc.SetPen(wxPen(wxColour(
                hueToRgb(0, 1, hue + 1.0 / 3.0),
                hueToRgb(0, 1, hue),
                hueToRgb(0, 1, hue - 1.0 / 3.0)
            )));
            dc.DrawLine(0, y, size.x, y);
        }
        int hue = (int)(tool->GetHue() * size.y);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, hue, size.x, hue);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawLine(0, hue - 1, size.x, hue - 1);
        dc.DrawLine(0, hue + 1, size.x, hue + 1);
        
    }
};

class CrossSection : public wxPanel
{
private:
    CylindricalSelectorTool* tool;
public:
    CrossSection(CylindricalSelectorTool* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL)
        : wxPanel(parent, id, pos, size, style)
    {
        tool = parent;
        SetCursor(*wxCROSS_CURSOR);
        SetMinSize(wxSize(100, 100));
        Bind(wxEVT_PAINT, &CrossSection::OnPaint, this);
        Bind(wxEVT_SIZE, &CrossSection::OnResize, this);
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
        wxImage image(100, 100);
        double hue = tool->GetHue();
        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                double lig = x / 100.0;
                double sat = 1.0 - y / 100.0;
                wxColour color = hslToRgb(hue, sat ,lig);
                image.SetRGB(x, y, color.Red(), color.Green(), color.Blue());
            }
        }
        wxSize size = dc.GetSize();
        image.Rescale(size.x, size.y);
        dc.DrawBitmap(wxBitmap(image), 0, 0);
        int sat = (int)(tool->GetSaturation() * size.y);
        int lig = (int)(tool->GetLightness() * size.x);
        dc.SetPen(wxPen(*wxWHITE, 3));
        dc.DrawLine(0, sat, size.x, sat);
        dc.DrawLine(lig, 0, lig, size.y);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, sat, size.x, sat);
        dc.DrawLine(lig, 0, lig, size.y);
    }
};

CylindricalSelectorTool::CylindricalSelectorTool(MainFrame* main) : ToolWindow(main, wxID_ANY, _("Cylindrical Selector Tool"), wxDefaultPosition, wxSize(480, 300))
{
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    boxSizer->Add(mainPanel, 1, wxEXPAND, 5);
    
    wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(1);
    
    wxStaticText *text = new wxStaticText(mainPanel, wxID_ANY, wxT(""));
    sizer->Add(text, 1, wxALL | wxEXPAND, 5);
    
    wxChoice *choice = new wxChoice(mainPanel, wxID_ANY);
    sizer->Add(choice, 1, wxALL | wxEXPAND, 5);
    
    hueSlider = new HueSlider(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(hueSlider, 1, wxALL | wxEXPAND, 5);
    
    crossSection = new CrossSection(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(crossSection, 1, wxALL | wxEXPAND, 5);
    
    mainPanel->SetSizerAndFit(sizer);
    SetSizerAndFit(boxSizer);
}

std::string CylindricalSelectorTool::GetName()
{
    return "CylindricalSelectorTool";
}

double CylindricalSelectorTool::GetHue() {
    return hue;
}

void CylindricalSelectorTool::SetHue(double hue) {
    this->hue = hue;
}

double CylindricalSelectorTool::GetLightness()
{
    return lightness;
}

void CylindricalSelectorTool::SetLightness(double lightness)
{
    this->lightness = lightness;
}

double CylindricalSelectorTool::GetSaturation()
{
    return saturation;
}

void CylindricalSelectorTool::SetSaturation(double saturation)
{
    this->saturation = saturation;
}
