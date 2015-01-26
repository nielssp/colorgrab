#include "MainFrame.h"

#include "RGBModel.h"
#include "HSLModel.h"

#include "HtmlHexOutput.h"

#include <wx/aboutdlg.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#include <wx/dcmemory.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/dnd.h>
#include <wx/dataobj.h>
#include <wx/colordlg.h>
#include <wx/colourdata.h>

struct ZoomMenuFunctor
{
    ZoomPanel* panel;
    int zoom;
    ZoomMenuFunctor(ZoomPanel* p, int z) : panel(p), zoom(z) { }
    void operator()(wxCommandEvent& event)
    {
        panel->SetZoom(zoom);
    }
};

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent), capturing(false)
{
    
    RestorePosition();
    
    colorOutputs.push_back(new HtmlHexOutput());
    colorOutput = colorOutputs[0];
	for (unsigned int i = 0; i < colorOutputs.size(); i++)
	{
        wxMenuItem* menuItem = new wxMenuItem(m_colorOutputMenu, wxID_ANY, colorOutputs[i]->getName(), wxT(""), wxITEM_RADIO);
        m_colorOutputMenu->Append(menuItem);
        m_colorOutputMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorOutput, this, menuItem->GetId());
	}
    
	colorModels.push_back(new RGBModel());
	colorModels.push_back(new HSLModel());
    colorModel = colorModels[0];
	for (unsigned int i = 0; i < colorModels.size(); i++)
	{
        wxMenuItem* menuItem = new wxMenuItem(m_colorModelMenu, i, colorModels[i]->getName(), wxT(""), wxITEM_RADIO);
        m_colorModelMenu->Append(menuItem);
        m_colorModelMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorModel, this, menuItem->GetId());
	}
    UpdateColorModel();
    
    for (unsigned int i = 1; i <= 64; i *= 2)
    {
        wxMenuItem* menuItem = new wxMenuItem(m_zoomMenu, wxID_ANY, wxString::Format("%d times", i), wxT(""), wxITEM_RADIO);
        m_zoomMenu->Append(menuItem);
        m_zoomMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, ZoomMenuFunctor(m_zoomPanel, i), menuItem->GetId());
    }
    
    SetColor(wxColour(
        config.ReadLong("Main/Color/R", 0),
        config.ReadLong("Main/Color/G", 0),
        config.ReadLong("Main/Color/B", 0)
    ));
}

MainFrame::~MainFrame()
{
    wxPoint pos = GetPosition();
    config.Write("Main/Position/X", pos.x);
    config.Write("Main/Position/Y", pos.y);
    wxColour col = GetColor();
    config.Write("Main/Color/R", col.Red());
    config.Write("Main/Color/G", col.Green());
    config.Write("Main/Color/B", col.Blue());
}

void MainFrame::RestorePosition()
{
    int x = config.ReadLong("Main/Position/X", GetPosition().x);
    int y = config.ReadLong("Main/Position/Y", GetPosition().y);
    wxSize screenSize = wxGetDisplaySize();
    wxSize windowSize = GetSize();
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > (screenSize.x - windowSize.x)) x = screenSize.x - windowSize.x;
    if (y > (screenSize.y - windowSize.y)) y = screenSize.y - windowSize.y;
    SetPosition(wxPoint(x, y));
}

void MainFrame::SetColorModel(IColorModel* colorModel)
{
    this->colorModel = colorModel;
    UpdateColorModel();
}

void MainFrame::SetColorOutput(IColorOutput* colorOutput)
{
    this->colorOutput = colorOutput;
}

void update_label_and_ctrl(int i, IColorModel* colorModel, wxStaticText* label, wxTextCtrl* ctrl)
{
    if (colorModel->getNumComponents() > i) {
        label->Show(true);
        ctrl->Show(true);
        std::string labelStr = colorModel->getLabel(i);
        label->SetLabel(labelStr.substr(0, 1).append(":"));
        label->SetToolTip(labelStr);
        ctrl->SetToolTip(labelStr);
    }
    else {
        label->Show(false);
        ctrl->Show(false);
    }
}

void MainFrame::UpdateColorModel()
{
    update_label_and_ctrl(0, colorModel, m_firstLabel, m_firstCtrl);
    update_label_and_ctrl(1, colorModel, m_secondLabel, m_secondCtrl);
    update_label_and_ctrl(2, colorModel, m_thirdLabel, m_thirdCtrl);
    update_label_and_ctrl(3, colorModel, m_fourthLabel, m_fourthCtrl);
    SetColor(GetColor());
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    wxUnusedVar(event);
    Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetName("ColorGrab");
    info.SetVersion("0.1-dev");
    info.SetWebSite("http://nielssp.dk");
    info.SetCopyright(_("(C) 2015 Niels Sonnich Poulsen"));
    info.SetLicence(_("MIT License"));
    info.SetDescription(_("Free color picker tool."));
    ::wxAboutBox(info);
}

wxBitmap GetScreenShot()
{
    wxSize screenSize = wxGetDisplaySize();
    wxBitmap bitmap(screenSize.x, screenSize.y);
    wxScreenDC dc;
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.Blit(0, 0, screenSize.x, screenSize.y, &dc, 0, 0);
    memDC.SelectObject(wxNullBitmap);
    return bitmap;
}

void MainFrame::UpdateZoomArea()
{
    m_zoomPanel->SetPoi(wxGetMousePosition());
}

void MainFrame::SetColorFromMouse()
{
    int x, y;
    wxGetMousePosition(&x, &y);
    SetColorFromPixel(x, y);
}

void MainFrame::SetColorFromPixel(wxCoord x, wxCoord y)
{
    wxColor color;
    wxScreenDC dc;
    dc.GetPixel(x, y, &color);
    SetColor(color);
}

wxColor MainFrame::GetColor() const
{
    return m_colorButton->GetBackgroundColour();
}

void MainFrame::SetColor(const wxColor& color, bool updateInputs, bool updateOutput)
{
    m_colourPicker->SetColour(color);
    m_colorButton->SetBackgroundColour(color);
	m_colorButton->Refresh();
    colorModel->setColor(color);
    if (updateInputs)
    {
        m_firstCtrl->ChangeValue(wxString::Format("%d", colorModel->getValue(0)));
        m_secondCtrl->ChangeValue(wxString::Format("%d", colorModel->getValue(1)));
        m_thirdCtrl->ChangeValue(wxString::Format("%d", colorModel->getValue(2)));
        m_fourthCtrl->ChangeValue(wxString::Format("%d", colorModel->getValue(3)));
    }
    colorOutput->setColor(color);
    if (updateOutput)
        m_formatText->ChangeValue(colorOutput->getOutput());
}

long getColorValue(wxTextCtrl* ctrl)
{
    long val = 0;
    ctrl->GetValue().ToLong(&val);
    return val;
}

void MainFrame::OnColorChange(wxCommandEvent& event)
{
    colorModel->setValue(0, getColorValue(m_firstCtrl));
    colorModel->setValue(1, getColorValue(m_secondCtrl));
    colorModel->setValue(2, getColorValue(m_thirdCtrl));
    colorModel->setValue(3, getColorValue(m_fourthCtrl));
    SetColor(colorModel->getColor(), false);
}
void MainFrame::OnColorPick(wxColourPickerEvent& event)
{
    SetColor(m_colourPicker->GetColour());
}

void MainFrame::OnCaptureStart(wxMouseEvent& event)
{
    SetColorFromMouse();
    UpdateZoomArea();
    CaptureMouse();
    SetCursor(*wxCROSS_CURSOR);
    capturing = true;
}
void MainFrame::OnCaptureEnd(wxMouseEvent& event)
{
    if (capturing)
    {
        SetColorFromMouse();
        UpdateZoomArea();
        ReleaseMouse();
        capturing = false;
        SetCursor(wxNullCursor);
    }
}
void MainFrame::OnCaptureMove(wxMouseEvent& event)
{
    if (capturing) {
        SetColorFromMouse();
        UpdateZoomArea();
    }
}

void MainFrame::OnSystemColorPicker(wxCommandEvent& event)
{
    wxColourDialog dialog(this);
    dialog.GetColourData().SetColour(GetColor());
    dialog.ShowModal();
    SetColor(dialog.GetColourData().GetColour());
}
void MainFrame::OnFormatChoose(wxMenuEvent& event)
{
}
void MainFrame::OnFormatClick(wxCommandEvent& event)
{
}
void MainFrame::OnCaptureZoom(wxMouseEvent& event)
{
    if (capturing)
        OnZoomPanelZoom(event);
}

void MainFrame::OnZoomSelect(wxCommandEvent& event)
{
    m_zoomPanel->SetZoom(2);
}
void MainFrame::OnZoomPanelDown(wxMouseEvent& event)
{
    m_zoomPanel->ShowPoi(false);
    SetColorFromMouse();
    capturing = true;
}
void MainFrame::OnZoomPanelMove(wxMouseEvent& event)
{
    if (capturing)
        SetColorFromMouse();
}
void MainFrame::OnZoomPanelUp(wxMouseEvent& event)
{
    SetColorFromMouse();
    capturing = false;
}
void MainFrame::OnZoomPanelZoom(wxMouseEvent& event)
{
    int zoom = m_zoomPanel->GetZoom();
    if (event.GetWheelRotation() > 0 && zoom < 64)
        zoom *= 2;
    else if (event.GetWheelRotation() < 0 && zoom > 1)
        zoom /= 2;
    m_zoomPanel->SetZoom(zoom);
}

void MainFrame::OnSelectColorModel(wxCommandEvent& event)
{
    SetColorModel(colorModels[event.GetId()]);
}

void MainFrame::OnSelectColorOutput(wxCommandEvent& event)
{
    SetColorOutput(colorOutputs[event.GetId()]);
}
void MainFrame::OnColorOutputChange(wxCommandEvent& event)
{
    colorOutput->setOutput(m_formatText->GetValue().ToStdString());
    SetColor(colorOutput->getColor(), true, false);
}
void MainFrame::OnInputOutputBlur(wxFocusEvent& event)
{
    SetColor(GetColor());
    event.Skip();
}
void MainFrame::OnInputOutputEnter(wxCommandEvent& event)
{
    SetColor(GetColor());
}
