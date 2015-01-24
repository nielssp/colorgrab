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
    dragPicker = true;
    capturing = false;
    format = "#%02x%02x%02x";
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
    int x = 0, y = 0;
    if (mouse.x > 10) {
        if (mouse.x > screenSize.x - 10)
            x = screenSize.x - 20;
        else
            x = mouse.x - 10;
    }
    if (mouse.y > 10) {
        if (mouse.y > screenSize.y - 10)
            y = screenSize.y - 20;
        else
            y = mouse.y - 10;
    }
    int relX = mouse.x - x, relY = mouse.y - y;
    wxBitmap bitmap(100, 100);
    wxScreenDC dc;
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.StretchBlit(0, 0, 100, 100, &dc, x, y, 20, 20);
    memDC.SelectObject(wxNullBitmap);
    wxGraphicsContext* gc = wxGraphicsContext::Create( bitmap );
    if (gc) {
        gc->SetPen(*wxWHITE_PEN);
        gc->SetBrush(wxNullBrush);
        wxGraphicsPath path = gc->CreatePath();
        path.AddCircle(relX * 5.0 + 2.5, relY * 5.0 + 2, 5);
        gc->StrokePath(path);
        path = gc->CreatePath();
        path.AddCircle(relX * 5.0 + 2.5, relY * 5.0 + 2, 4);
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
