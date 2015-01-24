#include "MainFrame.h"

#include "GrabFrame.h"
#include "GrabDropSource.h"

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


MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
    magnification = 4;
    dragPicker = true;
    capturing = false;
    format = "#%02x%02x%02x";
    
    for (int i = 2; i <= 64; i *= 2) {
        wxMenuItem* menuItem= new wxMenuItem(m_zoomMenu, wxID_ANY, wxString::Format("%d times", i), wxT(""), wxITEM_RADIO);
        m_zoomMenu->Append(menuItem);
        this->Connect(menuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnZoomSelect), NULL, this);
    }
}

MainFrame::~MainFrame()
{
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
    info.SetCopyright(_("Copyright 2015 Niels Sonnich Poulsen"));
    info.SetLicence(_("MIT License"));
    info.SetDescription(_("Short description goes here"));
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
    wxPoint mouse = wxGetMousePosition();
    wxSize screenSize = wxGetDisplaySize();
    wxSize zoomArea(128 / magnification, 128 / magnification);
    int x = 0, y = 0;
    if (mouse.x > zoomArea.x / 2) {
        if (mouse.x > screenSize.x - zoomArea.x / 2)
            x = screenSize.x - zoomArea.x;
        else
            x = mouse.x - zoomArea.x / 2;
    }
    if (mouse.y > zoomArea.y / 2) {
        if (mouse.y > screenSize.y - zoomArea.y / 2)
            y = screenSize.y - zoomArea.y;
        else
            y = mouse.y - zoomArea.y / 2;
    }
    int relX = mouse.x - x, relY = mouse.y - y;
    wxBitmap bitmap(128, 128);
    wxScreenDC dc;
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.StretchBlit(0, 0, 128, 128, &dc, x, y, zoomArea.x, zoomArea.y);
    memDC.SelectObject(wxNullBitmap);
    wxGraphicsContext* gc = wxGraphicsContext::Create( bitmap );
    if (gc) {
        gc->SetPen(*wxWHITE_PEN);
        gc->SetBrush(wxNullBrush);
        wxGraphicsPath path = gc->CreatePath();
        path.AddCircle(relX * (double)magnification + magnification / 2, relY * (double)magnification + magnification / 2, 5);
        gc->StrokePath(path);
        path = gc->CreatePath();
        path.AddCircle(relX * (double)magnification + magnification / 2, relY * (double)magnification + magnification / 2, 4);
        gc->SetPen(*wxBLACK_PEN);
        gc->StrokePath(path);
        delete gc;
    }
    m_dumpImage->SetImage(bitmap);
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

void MainFrame::SetColor(const wxColor& color, bool updateInputs)
{
    m_colourPicker->SetColour(color);
    m_colorButton->SetBackgroundColour(color);
    if (updateInputs)
    {
        m_redCtrl->ChangeValue(wxString::Format("%d", color.Red()));
        m_greenCtrl->ChangeValue(wxString::Format("%d", color.Green()));
        m_blueCtrl->ChangeValue(wxString::Format("%d", color.Blue()));
    }
    m_formatText->ChangeValue(wxString::Format(format, color.Red(), color.Green(), color.Blue()));
}


void MainFrame::OnGrabClick(wxCommandEvent& event)
{
    wxBitmap bmp = GetScreenShot();
    m_dumpImage->SetImage(bmp);
    wxFrame* frame = new GrabFrame(this);
    ImagePanel* p = new ImagePanel(frame);
    p->SetImage(bmp);
    frame->Show(true);
    frame->ShowFullScreen(true);
}

long getColorValue(wxTextCtrl* ctrl)
{
    long val = 0;
    ctrl->GetValue().ToLong(&val);
    if (val < 0)
        val = 0;
    else if (val > 255)
        val = 255;
    return val;
}

void MainFrame::OnColorChange(wxCommandEvent& event)
{
    long red = getColorValue(m_redCtrl);
    long green = getColorValue(m_greenCtrl);
    long blue = getColorValue(m_blueCtrl);
    SetColor(wxColour(red, green, blue), false);
}
void MainFrame::OnColorPick(wxColourPickerEvent& event)
{
    SetColor(m_colourPicker->GetColour());
}

void MainFrame::OnLeftDown(wxMouseEvent& event)
{
    wxTextDataObject my_data("test data");
    GrabDropSource dragSource( this );
    dragSource.SetData( my_data );
    wxDragResult result = dragSource.DoDragDrop(true);
    dragSource.SetCursor(result, *wxCROSS_CURSOR);
}
void MainFrame::OnCaptureStart(wxMouseEvent& event)
{
    if (dragPicker)
    {
        SetColorFromMouse();
        UpdateZoomArea();
        CaptureMouse();
        SetCursor(*wxCROSS_CURSOR);
        capturing = true;
    }
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
    else if (!dragPicker)
    {
        SetColorFromMouse();
        UpdateZoomArea();
        CaptureMouse();
        SetCursor(*wxCROSS_CURSOR);
        capturing = true;
    }
}
void MainFrame::OnCaptureMove(wxMouseEvent& event)
{
    if (capturing) {
        SetColorFromMouse();
        UpdateZoomArea();
    }
}
void MainFrame::OnSettingDrag(wxCommandEvent& event)
{
    dragPicker = m_dragMenuItem->IsChecked();
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
void MainFrame::OnDumpGrabEnd(wxMouseEvent& event)
{
    SetColorFromMouse();
    capturing = false;
}
void MainFrame::OnDumpGrabMove(wxMouseEvent& event)
{
    if (capturing)
        SetColorFromMouse();
}
void MainFrame::OnDumpGrabStart(wxMouseEvent& event)
{
    
    SetColorFromMouse();
    capturing = true;
}
void MainFrame::OnCaptureZoom(wxMouseEvent& event)
{
    if (capturing)
    {
        if (event.GetWheelRotation() > 0 && magnification < 64)
            magnification *= 2;
        else if (event.GetWheelRotation() < 0 && magnification > 1)
            magnification /= 2;
        UpdateZoomArea();
    }
}

void MainFrame::OnZoomSelect(wxCommandEvent& event)
{
    magnification = 2;
}
