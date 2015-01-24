#include "MainFrame.h"

#include "GrabFrame.h"
#include "GrabDropSource.h"

#include <wx/aboutdlg.h>
#include <wx/dcscreen.h>
#include <wx/dcmemory.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/dnd.h>
#include <wx/dataobj.h>


MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
	 capturing = false;
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
	 if (mouse.x > 10)
	 {
		  if (mouse.x > screenSize.x - 20)
				x = screenSize.x - 20;
		  else
				x = mouse.x - 10;
	 }
	 if (mouse.y > 10)
	 {
		  if (mouse.y > screenSize.y - 20)
				y = screenSize.y - 20;
		  else
				y = mouse.y - 10;
	 }
	 wxBitmap bitmap(100, 100);
	 wxScreenDC dc;
	 wxMemoryDC memDC;
	 memDC.SelectObject(bitmap);
	 memDC.StretchBlit(0, 0, 100, 100, &dc, x, y, 20, 20);
	 memDC.SelectObject(wxNullBitmap);
	 m_dumpImage->SetImage(bitmap);
}

void MainFrame::SetColorFromPixel(wxCoord x, wxCoord y)
{
	 wxColor color;
	 wxScreenDC dc;
	 dc.GetPixel(x, y, &color);
	 SetColor(color);
}

void MainFrame::SetColor(const wxColor& color)
{
	m_colourPicker->SetColour(color);
	m_colorButton->SetBackgroundColour(color);
	m_redCtrl->ChangeValue(wxString::Format("%d", color.Red()));
	m_greenCtrl->ChangeValue(wxString::Format("%d", color.Green()));
	m_blueCtrl->ChangeValue(wxString::Format("%d", color.Blue()));
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

long getColorValue(wxTextCtrl* ctrl) {
	long val = 0;
	ctrl->GetValue().ToLong(&val);
	if (val < 0)
		val = 0;
	else if (val > 255)
		val = 255;
	ctrl->ChangeValue(wxString::Format("%ld", val));
	return val;
}

void MainFrame::OnColorChange(wxCommandEvent& event)
{
	long red = getColorValue(m_redCtrl);
	long green = getColorValue(m_greenCtrl);
	long blue = getColorValue(m_blueCtrl);
	SetColor(wxColour(red, green, blue));
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
	 CaptureMouse();
	 SetCursor(*wxCROSS_CURSOR);
	 capturing = true;
}
void MainFrame::OnCaptureEnd(wxMouseEvent& event)
{
	 if (capturing)
	 {
		  int x, y;
		  wxGetMousePosition(&x, &y);
		  SetColorFromPixel(x, y);
		  ReleaseMouse();
		  capturing = false;
		  SetCursor(wxNullCursor);
	 }
}
void MainFrame::OnCaptureMove(wxMouseEvent& event)
{
	 if (capturing)
	 {
		  int x, y;
		  wxGetMousePosition(&x, &y);
		  SetColorFromPixel(x, y);
		  UpdateZoomArea();
	 }
}
