#ifndef COLOROUTPUTS_H
#define COLOROUTPUTS_H

#include "colormodels.h"
#include <wx/colour.h>
#include <wx/string.h>

class IColorOutput
{
public:
    virtual ~IColorOutput() {}

    virtual std::string getName() const = 0;
    
    virtual std::string getOutput() = 0;
    virtual bool parseColor(std::string colorString) = 0;

    virtual wxColor getColor() const = 0;
    virtual void setColor(const wxColour& color) = 0;
};

class HtmlHexOutput : public IColorOutput
{
protected:
    wxColour color;

public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual wxColor getColor() const;
    virtual std::string getOutput();
    virtual void setColor(const wxColour& color);
    virtual bool parseColor(std::string colorString);
};

class CssRgbOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString);
};

class CssHslOutput : public HtmlHexOutput
{
private:
    HSLModel model;
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
    virtual std::string getOutput();
    virtual bool parseColor(std::string colorString);
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
    virtual std::string getOutput();
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString);
};

#endif // COLOROUTPUTS_H
