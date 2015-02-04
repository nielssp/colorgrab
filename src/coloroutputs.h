#ifndef COLOROUTPUTS_H
#define COLOROUTPUTS_H

#include "colormodels.h"
#include <wx/colour.h>
#include <wx/string.h>

class ColorOutput
{
protected:
    wxColour color;
   
public:
    virtual ~ColorOutput();
    
    virtual wxColor getColor() const;
    virtual void setColor(const wxColour& color);
    
    virtual std::string getOutput();
    virtual bool parseColor(std::string colorString);

    virtual std::string getName() const = 0;
    virtual bool parseColor(std::string colorString, wxColour& color) = 0;
    virtual std::string format(const wxColour& color) = 0;

};

class HtmlHexOutput : public ColorOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
    virtual std::string format(const wxColour& color);
};

class CssRgbOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
};

class CssHslOutput : public HtmlHexOutput
{
private:
    HSLModel model;
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
    virtual std::string format(const wxColour& color);
};

class HexOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
};

class RgbFloatOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string format(const wxColour& color);
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
};

#endif // COLOROUTPUTS_H
