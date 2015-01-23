#include "GrabFrame.h"


GrabFrame::GrabFrame(wxWindow* parent) : wxFrame()
{
	 SetBackgroundStyle(wxBG_STYLE_TRANSPARENT);
	 SetCursor(*wxCROSS_CURSOR);
	 SetTransparent(0);
	 Create(
		  parent, -1, "Color Grab", wxDefaultPosition, wxDefaultSize,
		  wxSTAY_ON_TOP | wxFRAME_NO_TASKBAR
	 );
}


