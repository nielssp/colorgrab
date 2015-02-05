// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef COLORDROPTARGET_H
#define COLORDROPTARGET_H

#include <wx/dnd.h>

class IColorReceiver
{
public:
    virtual ~IColorReceiver() { }
    virtual bool ParseColor(std::string colorString) = 0;
};

class ColorDropTarget : public wxTextDropTarget
{
private:
    IColorReceiver* receiver;
public:
    ColorDropTarget(IColorReceiver* receiver)
    {
        this->receiver = receiver;
    }
    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
    {
        return receiver->ParseColor(data.ToStdString());
    }
};

#endif // COLORDROPTARGET_H
