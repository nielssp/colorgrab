#include "GrabDropSource.h"

GrabDropSource::GrabDropSource(MainFrame* main) : wxDropSource(main)
{
	 this->main = main;
}

bool GrabDropSource::GiveFeedback(wxDragResult effect)
{
	 int x, y;
	 wxGetMousePosition(&x, &y);
	 wxMessageOutput::Get()->Printf("Hello %d %d", x, y);
	 main->SetColorFromPixel(x, y);
	 return true;
}