// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef COLOROUTPUTS_H
#define COLOROUTPUTS_H

#include "colormodels.h"
#include <wx/colour.h>
#include <wx/string.h>

/// Conversion between color and string.
class ColorOutput
{
protected:
    wxColour color;
   
public:
    virtual ~ColorOutput();
    
    virtual wxColor getColor() const;
    virtual void setColor(const wxColour& color);
    
    virtual std::string getOutput(bool commaSpace = true, bool align = false);
    virtual bool parseColor(std::string colorString);

    virtual std::string getName() const = 0;
    virtual bool parseColor(std::string colorString, wxColour& color) = 0;
    virtual std::string format(const wxColour& color, bool commaSpace = true, bool align = false) = 0;

};

/// Hexadecimal RGB color: #FFFFFF
class HtmlHexOutput : public ColorOutput
{
public:
    virtual wxString getFormat(bool commaSpace = true, bool align = false) const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
    virtual std::string format(const wxColour& color, bool commaSpace = true, bool align = false);
};

/// Decimal RGB color: rgb(255, 255, 255)
class CssRgbOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat(bool commaSpace = true, bool align = false) const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
};

/// Decimal HSL color: hsl(359, 100, 100)
class CssHslOutput : public HtmlHexOutput
{
private:
    HSLModel model;
public:
    virtual wxString getFormat(bool commaSpace = true, bool align = false) const;
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
    virtual std::string format(const wxColour& color, bool commaSpace = true, bool align = false);
};

/// Hexadecimal RGB color: FFFFFF
class HexOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat(bool commaSpace = true, bool align = false) const;
    virtual std::string getName() const;
};

/// Float RGB color: 1.0, 1.0, 1.0
class RgbFloatOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat(bool commaSpace = true, bool align = false) const;
    virtual std::string format(const wxColour& color, bool commaSpace = true, bool align = false);
    virtual std::string getName() const;
    virtual bool parseColor(std::string colorString, wxColour& color);
};

#endif // COLOROUTPUTS_H
