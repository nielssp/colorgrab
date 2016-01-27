// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#include "MainFrame.h"

#include "colormodels.h"
#include "coloroutputs.h"
#include "ToolWindow.h"
#include "tools/PaletteTool.h"
#include "tools/CylindricalSelectorTool.h"

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
#include <wx/clipbrd.h>


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
    : MainFrameBaseClass(parent), capturing(false), refreshTimer(this), config(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxCONFIG_USE_SUBDIR | wxCONFIG_USE_LOCAL_FILE )
{
    ColorDropTarget* dt = new ColorDropTarget(this);
    SetDropTarget(dt);
    
    Bind(wxEVT_TIMER, &MainFrame::OnRefreshTimerEvent, this, refreshTimer.GetId());
    RestorePosition();
    
    bool alwaysOnTop = config.ReadBool("Main/AlwaysOnTop", true);
    if (alwaysOnTop) {
        SetWindowStyle(GetWindowStyle() | wxSTAY_ON_TOP);
        m_alwaysOnTopSetting->Check(true);
    }
    else {
        SetWindowStyle(GetWindowStyle() & ~wxSTAY_ON_TOP);
    }
    
    
    if (config.ReadBool("Main/CommaSpace", true)) {
        m_commaSpaceSetting->Check();
    }
    if (config.ReadBool("Main/AlignSpace", false)) {
        m_alignSetting->Check();
    }
    
    colorOutput = new HtmlHexOutput;
    AddColorOutput(colorOutput);
    AddColorOutput(new CssRgbOutput);
    AddColorOutput(new CssHslOutput);
    AddColorOutput(new HexOutput);
    AddColorOutput(new RgbFloatOutput);
    
    colorModel = new RGBModel;
    AddColorModel(colorModel);
    AddColorModel(new HSLModel);
    AddColorModel(new HSVModel);
    AddColorModel(new CMYKModel);
    UpdateColorModel();
    
    SetColor(config.ReadObject("Main/Color", wxColor(0, 0, 0)));
    
    m_zoomPanel->SetPoi(wxPoint(
        config.ReadLong("Main/ZoomPanel/X", 0),
        config.ReadLong("Main/ZoomPanel/Y", 0)
    ));
    m_zoomPanel->SetZoom(config.ReadLong("Main/ZoomPanel/Zoom", 4));
    
    for (int i = 0; i < 10; i++)
    {
        wxPanel *stackColor = new wxPanel(m_colorStack, wxID_ANY, wxDefaultPosition, wxSize(25,13), wxBORDER_SIMPLE);
        stackColor->SetCursor(wxCursor(wxCURSOR_HAND));
        stackColor->SetBackgroundColour(
            config.ReadObject(wxString::Format("Main/Stack/Color%d", i), wxColor(0, 0, 0))
        );
        m_colorStack->GetSizer()->Add(stackColor, 0, 0, 0);
        stackColors[stackColor->GetId()] = stackColor;
        stackColor->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnStackColorClick, this, stackColor->GetId());
        stackColor->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnPushColor, this, stackColor->GetId());
    }
    
    AddTool(new PaletteTool(this));
    AddTool(new CylindricalSelectorTool(this));
    
    AddCursor(_("&Crosshair"), *wxCROSS_CURSOR);
    AddCursor(_("&Magnifier"), wxCursor(wxCURSOR_MAGNIFIER));
    AddCursor(_("&Bullseye"), wxCursor(wxCURSOR_BULLSEYE));

    #include "../img/eyedropper.xpm"
    wxImage eyedropper(eyedropper_xpm);
    eyedropper.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 0);
    eyedropper.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 17);
    wxCursor eyedropperCursor(eyedropper);
    AddCursor(_("&Eyedropper"), eyedropperCursor);
}

MainFrame::~MainFrame()
{
    wxPoint pos = GetScreenPosition();
    config.Write("Main/Position/X", pos.x);
    config.Write("Main/Position/Y", pos.y);
    config.Write("Main/Color", GetColor());
    wxPoint poi = m_zoomPanel->GetPoi();
    config.Write("Main/ZoomPanel/X", poi.x);
    config.Write("Main/ZoomPanel/Y", poi.y);
    config.Write("Main/ZoomPanel/Zoom", m_zoomPanel->GetZoom());
    config.Write("Main/Model", wxString(colorModel->getName()));
    config.Write("Main/Output", wxString(colorOutput->getName()));
    
    config.Write("Main/AlwaysOnTop", (GetWindowStyle() & wxSTAY_ON_TOP) != 0);
    config.Write("Main/CommaSpace", m_commaSpaceSetting->IsChecked());
    config.Write("Main/AlignSpace", m_alignSetting->IsChecked());

    // Save stack colors
    for(size_t i = 0; i < 10; i++) {
        wxWindow* stackColor = m_colorStack->GetSizer()->GetItem(i)->GetWindow();
        config.Write(wxString::Format("Main/Stack/Color%d", (int)i), stackColor->GetBackgroundColour());
    }

    // Save state of tool windows
    for(std::map<int, ToolWindow*>::const_iterator iter = tools.begin(); iter != tools.end(); ++iter) {
        ToolWindow* tool = iter->second;
        config.SetPath(tool->GetName());
        tool->Store(&config);
        config.SetPath("..");
    }

    // Save cursor
    for(std::map<int, std::pair<wxString, wxCursor> >::const_iterator iter = cursors.begin(); iter != cursors.end(); ++iter) {
        if (m_cursorMenu->IsChecked(iter->first)) {
            config.Write("Main/Cursor", iter->second.first);
            break;
        }
    }
}

wxConfigBase* MainFrame::GetConfig()
{
    return &config;
}

void MainFrame::PushColor(const wxColour& color)
{
    wxColour previous = color;
    for (size_t i = 0; i < 10; i++)
    {
        wxWindow* stackColor = m_colorStack->GetSizer()->GetItem(i)->GetWindow();
        wxColour color = stackColor->GetBackgroundColour();
        stackColor->SetBackgroundColour(previous);
        stackColor->Refresh();
        previous = color;
    }
}

void MainFrame::RestorePosition()
{
    wxPoint current = GetScreenPosition();
    int x = config.ReadLong("Main/Position/X", current.x);
    int y = config.ReadLong("Main/Position/Y", current.y);
    wxSize screenSize = wxGetDisplaySize();
    wxSize windowSize = GetSize();
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > (screenSize.x - windowSize.x)) x = screenSize.x - windowSize.x;
    if (y > (screenSize.y - windowSize.y)) y = screenSize.y - windowSize.y;
    Center();
    Move(wxPoint(x, y));
}

bool MainFrame::Show(bool show)
{
  bool result = wxFrame::Show(show);
  // Under some window managers position can't be restored until after the window
  // has been opened (possibly?)
  wxPoint wantedPosition = GetPosition();
  Center();
  Move(wantedPosition);
  return result;
}

int MainFrame::AddColorModel(IColorModel* colorModel)
{
    wxWindowID id = wxIdManager::ReserveId();
    colorModels[id] = colorModel;
    wxMenuItem* menuItem = new wxMenuItem(m_colorModelMenu, id, wxString::Format("%s\tCtrl+%d", colorModel->getName(), (int) colorModels.size()), wxT(""), wxITEM_RADIO);
    m_colorModelMenu->Append(menuItem);
    m_colorModelMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorModel, this, menuItem->GetId());
    if (colorModel->getName() == config.Read("Main/Model", "RGB"))
    {
        this->colorModel = colorModel;
        menuItem->Check();
    }
    return id;
}

int MainFrame::AddColorOutput(ColorOutput* colorOutput)
{
    wxWindowID id = wxIdManager::ReserveId();
    colorOutputs[id] = colorOutput;
    wxMenuItem* menuItem = new wxMenuItem(m_colorOutputMenu, id, wxString::Format("%s\tAlt+%d", colorOutput->getName(), (int) colorOutputs.size()), wxT(""), wxITEM_RADIO);
    m_colorOutputMenu->Append(menuItem);
    m_colorOutputMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectColorOutput, this, menuItem->GetId());
    if (colorOutput->getName() == config.Read("Main/Output", "Hexadecimal (HTML/CSS)"))
    {
        this->colorOutput = colorOutput;
        menuItem->Check();
    }
    return id;
}

void MainFrame::AddTool(ToolWindow* tool)
{
    wxMenuItem* menuItem = new wxMenuItem(m_toolsMenu, wxID_ANY, wxString::Format("%s\tF%d", tool->GetTitle(), (int) tools.size() + 1));
    m_toolsMenu->Append(menuItem);
    tools[menuItem->GetId()] = tool;
    m_toolsMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectTool, this, menuItem->GetId());
    config.SetPath(tool->GetName());
    tool->Restore(&config);
    tool->UpdateColor(GetColor());
    config.SetPath("..");
}

void MainFrame::AddCursor(const wxString& label, const wxCursor& cursor)
{
    wxMenuItem* menuItem = new wxMenuItem(m_cursorMenu, wxID_ANY, label, wxT(""), wxITEM_RADIO);
    m_cursorMenu->Append(menuItem);
    if (label == config.Read("Main/Cursor", _("Crosshair"))) {
        m_zoomPanel->SetCursor(cursor);
        menuItem->Check();
    }
    cursors[menuItem->GetId()] = std::make_pair(label, cursor);
    m_cursorMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSelectCursor, this, menuItem->GetId());
}

void MainFrame::SetColorModel(IColorModel* colorModel)
{
    this->colorModel = colorModel;
    UpdateColorModel();
}

void MainFrame::SetColorOutput(ColorOutput* colorOutput)
{
    this->colorOutput = colorOutput;
    SetColor(GetColor());
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
        ctrl->SetRange(colorModel->getMin(i), colorModel->getMax(i));
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
    info.SetVersion("0.2");
    info.SetWebSite("http://nielssp.dk");
    info.SetCopyright(_("(C) 2015 Niels Sonnich Poulsen"));
    info.SetLicence(_("Copyright (C) 2015 Niels Sonnich Poulsen (http://nielssp.dk)\n\
\n\
Permission is hereby granted, free of charge, to any person obtaining a copy\n\
of this software and associated documentation files (the \"Software\"), to deal\n\
in the Software without restriction, including without limitation the rights\n\
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n\
copies of the Software, and to permit persons to whom the Software is\n\
furnished to do so, subject to the following conditions:\n\
\n\
The above copyright notice and this permission notice shall be included in all\n\
copies or substantial portions of the Software.\n\
\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n\
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n\
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n\
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n\
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n\
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n\
SOFTWARE."));
    info.SetDescription(_("A free cross-platform color picker tool."));
    wxAboutBox(info);
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
        m_formatText->ChangeValue(colorOutput->getOutput(m_commaSpaceSetting->IsChecked(), m_alignSetting->IsChecked()));
        
    for (std::map<int, ToolWindow*>::const_iterator iter = tools.begin(); iter != tools.end(); ++iter )
    {
        ToolWindow* tool = iter->second;
        tool->UpdateColor(color);
    }
}

bool MainFrame::ParseColor(std::string colorString)
{
    bool result = colorOutput->parseColor(colorString);
    SetColor(colorOutput->getColor());
    return result;
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
    SetCursor(m_zoomPanel->GetCursor());
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

void MainFrame::OnSelectTool(wxCommandEvent& event)
{
    wxFrame* tool = tools[event.GetId()];
    tool->Show(!tool->IsVisible());
    if (tool->IsVisible())
        tool->SetFocus();
}

void MainFrame::OnSelectCursor(wxCommandEvent& event)
{
    wxCursor cursor = cursors[event.GetId()].second;
    m_zoomPanel->SetCursor(cursor);
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
    colorOutput->parseColor(m_formatText->GetValue().ToStdString());
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
        if (intervals[i] == 1)
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

void MainFrame::OnStackColorClick(wxMouseEvent& event)
{
    SetColor(stackColors[event.GetId()]->GetBackgroundColour());
    wxTextDataObject colorData(colorOutput->getOutput());
    wxDropSource dragSource(this);
    dragSource.SetData(colorData);
    dragSource.DoDragDrop(true);
}

void MainFrame::OnPushColor(wxMouseEvent& event)
{
    SetColorFromMouse();
    PushColor(GetColor());
}
void MainFrame::OnCapturePush(wxMouseEvent& event)
{
    if (capturing)
        PushColor(GetColor());
}
void MainFrame::OnCopyColor(wxCommandEvent& event)
{
    if(wxTheClipboard->Open()) {
        SetColor(GetColor());
        wxTheClipboard->SetData(new wxTextDataObject(m_formatText->GetValue()));
        wxTheClipboard->Close();
    }
}
void MainFrame::OnCopyMagnifiedArea(wxCommandEvent& event)
{
    if(wxTheClipboard->Open()) {
        SetColor(GetColor());
        wxTheClipboard->SetData(new wxBitmapDataObject(m_zoomPanel->getImage()));
        wxTheClipboard->Close();
    }
}
void MainFrame::OnPasteColor(wxCommandEvent& event)
{
    if(wxTheClipboard->Open()) {
        if(wxTheClipboard->IsSupported(wxDF_TEXT)) {
            wxTextDataObject data;
            wxTheClipboard->GetData(data);
            m_formatText->SetValue(data.GetText());
        }
        wxTheClipboard->Close();
    }
}
void MainFrame::OnDragColor(wxMouseEvent& event)
{
    wxTextDataObject colorData(colorOutput->getOutput());
    wxDropSource dragSource(this);
    dragSource.SetData(colorData);
    dragSource.DoDragDrop(true);
}

ColorOutput* MainFrame::GetColorOutput() const
{
    return colorOutput;
}

IColorModel* MainFrame::GetColorModel() const
{
    return colorModel;
}

void MainFrame::OnSetAlwaysOnTop(wxCommandEvent& event)
{
    SetWindowStyle(GetWindowStyle() ^ wxSTAY_ON_TOP);
}

void MainFrame::OnSetCommaSpace(wxCommandEvent& event)
{
    SetColor(GetColor());
}

void MainFrame::OnSetAlign(wxCommandEvent& event)
{
    SetColor(GetColor());
}