#ifndef COLORDROPTARGET_H
#define COLORDROPTARGET_H


#include "MainFrame.h"

#include <wx/dnd.h>

class ColorDropTarget : public wxTextDropTarget
{
private:
    MainFrame* main;
public:
    ColorDropTarget(MainFrame* main)
    {
        this->main = main;
    }
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
    {
        return main->ParseColor(data.ToStdString());
    }
};

#endif // COLORDROPTARGET_H
