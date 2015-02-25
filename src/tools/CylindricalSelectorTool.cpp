// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#include "CylindricalSelectorTool.h"

#include "MainFrame.h"

#include <wx/panel.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <wx/dcmemory.h>
#include <algorithm>

class HueSlider : public wxPanel
{
private:
    CylindricalSelectorTool* tool;
    bool dragging;
public:
    HueSlider(CylindricalSelectorTool* tool,
              wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL)
        : wxPanel(parent, id, pos, size, style)
        , dragging(false)
    {
        this->tool = tool;
        SetCursor(*wxCROSS_CURSOR);
        SetMinSize(wxSize(30, 100));
        Bind(wxEVT_PAINT, &HueSlider::OnPaint, this);
        Bind(wxEVT_SIZE, &HueSlider::OnResize, this);
        
        Bind(wxEVT_LEFT_DOWN, &HueSlider::OnSetHue, this);
        Bind(wxEVT_MOTION, &HueSlider::OnSetHue, this);
        Bind(wxEVT_LEFT_UP, &HueSlider::OnSetHue, this);
        
        Bind(wxEVT_RIGHT_DOWN, &HueSlider::OnPushColor, this);
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
    
    void OnSetHue(wxMouseEvent& event)
    {
        if (event.GetEventType() == wxEVT_LEFT_DOWN) {
            dragging = true;
            CaptureMouse();
        }
        else if (event.GetEventType() == wxEVT_LEFT_UP) {
            dragging = false;
            ReleaseMouse();
        }
        else if (event.GetEventType() == wxEVT_MOTION && !dragging) {
            return;
        }
        wxPoint pos = ScreenToClient(wxGetMousePosition());
        tool->SetHue(1.0 - pos.y / (double) GetClientSize().y);
    }
    
    void OnPushColor(wxMouseEvent& event)
    {
        tool->OnPushColor(event);
    }
    
    void Render(wxPaintDC& dc)
    {
        wxImage image(1, 360);
        for (int y = 0; y < 360; y++) {
            double hue = 1.0 - y / 360.0;
            wxColour color = wxColour(
                hueToRgb(0, 1, hue + 1.0 / 3.0),
                hueToRgb(0, 1, hue),
                hueToRgb(0, 1, hue - 1.0 / 3.0)
            );
            image.SetRGB(0, y, color.Red(), color.Green(), color.Blue());
        }
        wxSize size = dc.GetSize();
        image.Rescale(size.x, size.y, wxIMAGE_QUALITY_NEAREST);
        dc.DrawBitmap(wxBitmap(image), 0, 0);
        
        int hue = round((1.0 - tool->GetHue()) * (size.y - 1));
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
    bool dragging;
public:
    CrossSection(CylindricalSelectorTool* tool,
              wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL)
        : wxPanel(parent, id, pos, size, style)
        , dragging(false)
    {
        this->tool = tool;
        SetCursor(*wxCROSS_CURSOR);
        SetMinSize(wxSize(101, 101));
        Bind(wxEVT_PAINT, &CrossSection::OnPaint, this);
        Bind(wxEVT_SIZE, &CrossSection::OnResize, this);
        
        Bind(wxEVT_LEFT_DOWN, &CrossSection::OnSetPosition, this);
        Bind(wxEVT_MOTION, &CrossSection::OnSetPosition, this);
        Bind(wxEVT_LEFT_UP, &CrossSection::OnSetPosition, this);
        
        Bind(wxEVT_RIGHT_DOWN, &CrossSection::OnPushColor, this);
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
    
    void OnSetPosition(wxMouseEvent& event)
    {
        if (event.GetEventType() == wxEVT_LEFT_DOWN) {
            dragging = true;
            CaptureMouse();
        }
        else if (event.GetEventType() == wxEVT_LEFT_UP) {
            dragging = false;
            ReleaseMouse();
        }
        else if (event.GetEventType() == wxEVT_MOTION && !dragging) {
            return;
        }
        wxPoint pos = ScreenToClient(wxGetMousePosition());
        tool->SetLightness(pos.x / (double) GetClientSize().x);
        tool->SetSaturation(1.0 - pos.y / (double) GetClientSize().y);
        Refresh(false);
    }
    
    void OnPushColor(wxMouseEvent& event)
    {
        tool->OnPushColor(event);
    }
    
    void Render(wxPaintDC& dc)
    {
        wxImage image(101, 101);
        double hue = tool->GetHue();
        for (int x = 0; x <= 100; x++) {
            for (int y = 0; y <= 100; y++) {
                double lig = x / 100.0;
                double sat = 1.0 - y / 100.0;
                wxColour color = hslToRgb(hue, sat ,lig);
                image.SetRGB(x, y, color.Red(), color.Green(), color.Blue());
            }
        }
        wxSize size = dc.GetSize();
        image.Rescale(size.x, size.y, wxIMAGE_QUALITY_NEAREST);
        dc.DrawBitmap(wxBitmap(image), 0, 0);

        int sat = (int)((1.0 - tool->GetSaturation()) * (size.y - 1));
        int lig = (int)(tool->GetLightness() * (size.x - 1));
        dc.SetPen(wxPen(*wxWHITE, 3));
        dc.DrawLine(0, sat, size.x, sat);
        dc.DrawLine(lig, 0, lig, size.y);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, sat, size.x, sat);
        dc.DrawLine(lig, 0, lig, size.y);
    }
};

CylindricalSelectorTool::CylindricalSelectorTool(MainFrame* main) : ToolWindow(main, wxID_ANY, _("Cylindrical Selector Tool"), wxDefaultPosition, wxSize(300, 250))
{
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    boxSizer->Add(mainPanel, 1, wxEXPAND, 5);
    
    wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 2, 0, 0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(0);
    
//    wxStaticText *text = new wxStaticText(mainPanel, wxID_ANY, wxT(""));
//    sizer->Add(text, 1, wxALL | wxEXPAND, 5);
//    
//    wxChoice *choice = new wxChoice(mainPanel, wxID_ANY);
//    sizer->Add(choice, 1, wxALL | wxEXPAND, 5);
    
    hueSlider = new HueSlider(this, mainPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(hueSlider, 1, wxALL | wxEXPAND, 5);
    
    crossSection = new CrossSection(this, mainPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(crossSection, 1, wxALL | wxEXPAND, 5);
    
    mainPanel->SetSizerAndFit(sizer);
    SetSizerAndFit(boxSizer);
}

std::string CylindricalSelectorTool::GetName()
{
    return "CylindricalSelectorTool";
}

void CylindricalSelectorTool::UpdateColor(const wxColour& color)
{
    if (model.getColor() == color)
        return;
    model.setColor(color);
    this->hue = model.getValue(0) / 360.0;
    this->saturation = model.getValue(1) / 100.0;
    this->lightness = model.getValue(2) / 100.0;
    hueSlider->Refresh(false);
    crossSection->Refresh(false);
}

double CylindricalSelectorTool::GetHue() {
    return hue;
}

void CylindricalSelectorTool::SetHue(double hue) {
    this->hue = std::min(std::max(hue, 0.0), 1.0);
    hueSlider->Refresh(false);
    crossSection->Refresh(false);
    model.setValue(0, round(hue * 360.0));
    main->SetColor(model.getColor());
}

double CylindricalSelectorTool::GetLightness()
{
    return lightness;
}

void CylindricalSelectorTool::SetLightness(double lightness)
{
    this->lightness = std::min(std::max(lightness, 0.0), 1.0);
    model.setValue(2, round(lightness * 100.0));
    main->SetColor(model.getColor());
}

double CylindricalSelectorTool::GetSaturation()
{
    return saturation;
}

void CylindricalSelectorTool::SetSaturation(double saturation)
{
    this->saturation = std::min(std::max(saturation, 0.0), 1.0);
    model.setValue(1, round(saturation * 100.0));
    main->SetColor(model.getColor());
}

void CylindricalSelectorTool::OnPushColor(wxMouseEvent& event)
{
    main->PushColor(model.getColor());
}
