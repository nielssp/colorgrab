#include "MainFrame.h"

#include "RGBModel.h"
#include "HSLModel.h"
#include "CMYKModel.h"

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
    : MainFrameBaseClass(parent), capturing(false), refreshTimer(this)
{
    Bind(wxEVT_TIMER, &MainFrame::OnRefreshTimerEvent, this, refreshTimer.GetId());
    RestorePosition();
    
    colorOutput = new HtmlHexOutput;
    AddColorOutput(colorOutput);
    
    colorModel = new RGBModel;
    AddColorModel(colorModel);
    AddColorModel(new HSLModel);
    AddColorModel(new CMYKModel);
    UpdateColorModel();
    
    SetColor(wxColour(
        config.ReadLong("Main/Color/R", 0),
        config.ReadLong("Main/Color/G", 0),
        config.ReadLong("Main/Color/B", 0)
    ));
    
    m_zoomPanel->SetPoi(wxPoint(
        config.ReadLong("Main/ZoomPanel/X", 0),
        config.ReadLong("Main/ZoomPanel/Y", 0)
    ));
    m_zoomPanel->SetZoom(config.ReadLong("Main/ZoomPanel/Zoom", 4));
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
    wxPoint poi = m_zoomPanel->GetPoi();
    config.Write("Main/ZoomPanel/X", poi.x);
    config.Write("Main/ZoomPanel/Y", poi.y);
    config.Write("Main/ZoomPanel/Zoom", m_zoomPanel->GetZoom());
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

void MainFrame::AddColorModel(IColorModel* colorModel)
{
    wxWindowID id = wxIdManager::ReserveId();
    colorModels[id] = colorModel;
    wxMenuItem* menuItem = new wxMenuItem(m_colorModelMenu, id, colorModel->getName(), wxT(""), wxITEM_RADIO);
    m_colorModelMenu->Append(menuItem);
    m_colorModelMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorModel, this, menuItem->GetId());
}

void MainFrame::AddColorOutput(IColorOutput* colorOutput)
{
    wxWindowID id = wxIdManager::ReserveId();
    colorOutputs[id] = colorOutput;
    wxMenuItem* menuItem = new wxMenuItem(m_colorOutputMenu, id, colorOutput->getName(), wxT(""), wxITEM_RADIO);
    m_colorOutputMenu->Append(menuItem);
    m_colorOutputMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorOutput, this, menuItem->GetId());
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

void update_label_and_ctrl(int i, IColorModel* colorModel, wxStaticText* label, wxSpinCtrl* ctrl)
{
    if (colorModel->getNumComponents() > i) {
        label->Show(true);
        ctrl->Show(true);
        std::string labelStr = colorModel->getLabel(i);
        label->SetLabel(labelStr.substr(0, 1).append(":"));
        label->SetToolTip(labelStr);
        ctrl->SetToolTip(labelStr);
        ctrl->SetMin(colorModel->getMin(i));
        ctrl->SetMax(colorModel->getMax(i));
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
//    m_colourPicker->SetColour(color);
    m_colorButton->SetBackgroundColour(color);
	m_colorButton->Refresh();
    colorModel->setColor(color);
    if (updateInputs)
    {
        m_firstCtrl->SetValue(colorModel->getValue(0));
        m_secondCtrl->SetValue(colorModel->getValue(1));
        m_thirdCtrl->SetValue(colorModel->getValue(2));
        m_fourthCtrl->SetValue(colorModel->getValue(3));
    }
    colorOutput->setColor(color);
    if (updateOutput)
        m_formatText->ChangeValue(colorOutput->getOutput());
}

void MainFrame::OnColorChange(wxCommandEvent& event)
{
    colorModel->setValue(0, m_firstCtrl->GetValue());
    colorModel->setValue(1, m_secondCtrl->GetValue());
    colorModel->setValue(2, m_thirdCtrl->GetValue());
    colorModel->setValue(3, m_fourthCtrl->GetValue());
    SetColor(colorModel->getColor(), false);
}

void MainFrame::OnCaptureStart(wxMouseEvent& event)
{
    SetColorFromMouse();
    UpdateZoomArea();
    CaptureMouse();
    SetCursor(*wxCROSS_CURSOR);
    capturing = true;
    SetFocus();
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
    m_zoomPanel->SetFocus();
    SetColorFromMouse();
    capturing = true;
}
void MainFrame::OnZoomPanelMove(wxMouseEvent& event)
{
    m_zoomPanel->SetFocus();
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
void MainFrame::OnZoomIn(wxCommandEvent& event)
{
    int zoom = m_zoomPanel->GetZoom();
    if (zoom < 64)
        zoom *= 2;
    m_zoomPanel->SetZoom(zoom);
}
void MainFrame::OnZoomOut(wxCommandEvent& event)
{
    int zoom = m_zoomPanel->GetZoom();
    if (zoom > 1)
        zoom /= 2;
    m_zoomPanel->SetZoom(zoom);
}
void MainFrame::OnRefreshImage(wxCommandEvent& event)
{
    m_zoomPanel->Update();
}
void MainFrame::OnSelectTiming(wxCommandEvent& event)
{
    int interval = timerIntervals[event.GetId()];
    m_timerButton->Disable();
    refreshTimer.Start(interval * 1000);
}
void MainFrame::OnTimerRefreshImage(wxCommandEvent& event)
{
    wxMenu menu(_("Refresh image in..."));
    timerIntervals.clear();
    int intervals [] = {1, 2, 5};
    for (int i = 0; i < 3; i++) {
        wxMenuItem* item;
        if (i == 1)
            item = menu.Append(wxID_ANY, wxString::Format(_("%d second"), intervals[i]));
        else
            item = menu.Append(wxID_ANY, wxString::Format(_("%d seconds"), intervals[i]));
        menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectTiming, this, item->GetId());
        timerIntervals[item->GetId()] = intervals[i];
    }
    m_timerButton->PopupMenu(&menu);
}

void MainFrame::OnRefreshTimerEvent(wxTimerEvent& event)
{
    m_zoomPanel->Update();
    m_timerButton->Enable();
    refreshTimer.Stop();
}