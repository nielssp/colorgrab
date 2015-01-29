#ifndef ICOLOROUTPUT_H
#define ICOLOROUTPUT_H

#include <wx/colour.h>

class IColorOutput
{
public:
    virtual ~IColorOutput() {}

    virtual std::string getName() const = 0;
    
    virtual std::string getOutput() const = 0;
    virtual bool parseColor(std::string colorString) = 0;

    virtual wxColor getColor() const = 0;
    virtual void setColor(const wxColour& color) = 0;
};

#endif // ICOLOROUTPUT_H
