#ifndef COLOROUTPUTS_H
#define COLOROUTPUTS_H

#include <wx/colour.h>
#include <wx/string.h>

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

class CssRgbOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
};

#endif // COLOROUTPUTS_H
