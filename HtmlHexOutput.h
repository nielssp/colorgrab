#ifndef HTMLHEXOUTPUT_H
#define HTMLHEXOUTPUT_H

#include "IColorOutput.h" // Base class: IColorOutput


#include <wx/string.h>

class HtmlHexOutput : public IColorOutput
{
private:
    wxColour color;

public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual wxColor getColor() const;
    virtual std::string getOutput() const;
    virtual void setColor(const wxColour& color);
    virtual bool parseColor(std::string colorString);
};

#endif // HTMLHEXOUTPUT_H
