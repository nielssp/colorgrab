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
        tool->SetB(pos.x / (double) GetClientSize().x);
        tool->SetA(1.0 - pos.y / (double) GetClientSize().y);
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
                double b = x / 100.0;
                double a = 1.0 - y / 100.0;
                wxColour color = tool->ToRgb(hue, a ,b);
                image.SetRGB(x, y, color.Red(), color.Green(), color.Blue());
            }
        }
        wxSize size = dc.GetSize();
        image.Rescale(size.x, size.y, wxIMAGE_QUALITY_NEAREST);
        dc.DrawBitmap(wxBitmap(image), 0, 0);

        int a = (int)((1.0 - tool->GetA()) * (size.y - 1));
        int b = (int)(tool->GetB() * (size.x - 1));
        dc.SetPen(wxPen(*wxWHITE, 3));
        dc.DrawLine(0, a, size.x, a);
        dc.DrawLine(b, 0, b, size.y);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, a, size.x, a);
        dc.DrawLine(b, 0, b, size.y);
    }
};

struct CylColorSpace
{
    wxString name;
    wxColour (*toRgb)(double hue, double a, double b);
    IColorModel *model;
};

CylindricalSelectorTool::CylindricalSelectorTool(MainFrame* main) : ToolWindow(main, wxID_ANY, _("Cylindrical Selector Tool"))
{
    model = new HSLModel;
    toRgb = &hslToRgb;

    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(boxSizer);
        
    wxPanel *mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTAB_TRAVERSAL);
    boxSizer->Add(mainPanel, 1, wxEXPAND, 5);
    
    wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 0, 0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(1);
    
    wxStaticText *text = new wxStaticText(mainPanel, wxID_ANY, wxT(""));
    sizer->Add(text, 1, wxALL | wxEXPAND, 5);
    
    spaceChoice = new wxChoice(mainPanel, wxID_ANY);
    sizer->Add(spaceChoice, 1, wxALL | wxEXPAND, 5);
    
    hueSlider = new HueSlider(this, mainPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(hueSlider, 1, wxALL | wxEXPAND, 5);
    
    crossSection = new CrossSection(this, mainPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_THEME);
    sizer->Add(crossSection, 1, wxALL | wxEXPAND, 5);
    
    mainPanel->SetSizerAndFit(sizer);
    SetSizerAndFit(boxSizer);
    SetSize(wxSize(300, 250));
    
    Bind(wxEVT_CHOICE, &CylindricalSelectorTool::OnSelectColorSpace, this);
    
    AddColorSpace("Hue, Saturation, Lightness", &hslToRgb, model);
    AddColorSpace("Hue, Saturation, Value", &hsvToRgb, new HSVModel);
    spaceChoice->SetSelection(0);
}

CylindricalSelectorTool::~CylindricalSelectorTool()
{
    
}

void CylindricalSelectorTool::AddColorSpace(const wxString& name, wxColour (*toRgb)(double hue, double a, double b), IColorModel *model)
{
    CylColorSpace* space = new CylColorSpace;
    space->name = name;
    space->toRgb = toRgb;
    space->model = model;
    colorSpaces.push_back(space);
    spaceChoice->Append(name);
}

void CylindricalSelectorTool::SetColorSpace(CylColorSpace* colorSpace)
{
    toRgb = colorSpace->toRgb;
    model = colorSpace->model;
    model->setColor(main->GetColor());
    this->hue = model->getValue(0) / 360.0;
    this->a = model->getValue(1) / 100.0;
    this->b = model->getValue(2) / 100.0;
    crossSection->Refresh(false);
}

std::string CylindricalSelectorTool::GetName()
{
    return "CylindricalSelectorTool";
}

void CylindricalSelectorTool::Store(wxConfigBase* config)
{
    ToolWindow::Store(config);
    
    config->Write("ColorSpace", spaceChoice->GetSelection());
}

void CylindricalSelectorTool::Restore(wxConfigBase* config)
{
    ToolWindow::Restore(config);
    
    spaceChoice->SetSelection(config->ReadLong("ColorSpace", 0));
    size_t selection = spaceChoice->GetSelection();
    if (selection >= 0 && selection < colorSpaces.size())
        SetColorSpace(colorSpaces[selection]);
}

void CylindricalSelectorTool::UpdateColor(const wxColour& color)
{
    if (model->getColor() == color)
        return;
    model->setColor(color);
    this->hue = model->getValue(0) / 360.0;
    this->a = model->getValue(1) / 100.0;
    this->b = model->getValue(2) / 100.0;
    hueSlider->Refresh(false);
    crossSection->Refresh(false);
}

wxColour CylindricalSelectorTool::ToRgb(double hue, double a, double b)
{
    return toRgb(hue, a, b);
}

double CylindricalSelectorTool::GetHue() {
    return hue;
}

void CylindricalSelectorTool::SetHue(double hue) {
    this->hue = std::min(std::max(hue, 0.0), 1.0);
    hueSlider->Refresh(false);
    crossSection->Refresh(false);
    model->setValue(0, round(hue * 360.0));
    main->SetColor(model->getColor());
}

double CylindricalSelectorTool::GetB()
{
    return b;
}

void CylindricalSelectorTool::SetB(double b)
{
    this->b = std::min(std::max(b, 0.0), 1.0);
    model->setValue(2, round(b * 100.0));
    main->SetColor(model->getColor());
}

double CylindricalSelectorTool::GetA()
{
    return a;
}

void CylindricalSelectorTool::SetA(double a)
{
    this->a = std::min(std::max(a, 0.0), 1.0);
    model->setValue(1, round(a * 100.0));
    main->SetColor(model->getColor());
}

void CylindricalSelectorTool::OnPushColor(wxMouseEvent& event)
{
    main->PushColor(model->getColor());
}

void CylindricalSelectorTool::OnSelectColorSpace(wxCommandEvent& event)
{
    size_t selection = spaceChoice->GetSelection();
    if (selection >= 0 && selection < colorSpaces.size())
        SetColorSpace(colorSpaces[selection]);
}
