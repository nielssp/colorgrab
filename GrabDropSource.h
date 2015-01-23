#ifndef GRABDROPSOURCE_H
#define GRABDROPSOURCE_H

#include "MainFrame.h"

#include <wx/dnd.h>

class GrabDropSource : public wxDropSource
{
private:
	 MainFrame* main;
public:
	GrabDropSource(MainFrame* main);
	virtual bool GiveFeedback(wxDragResult effect);

};

#endif // GRABDROPSOURCE_H
