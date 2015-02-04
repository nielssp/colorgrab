#include "coloroutputs.h"

#include <wx/regex.h>

ColorOutput::~ColorOutput()
{
}

wxColor ColorOutput::getColor() const
{
    return color;
}
void ColorOutput::setColor(const wxColour& color)
{
    this->color = color;
}

std::string ColorOutput::getOutput()
{
    return format(color);
}

bool ColorOutput::parseColor(std::string colorString)
{
    return parseColor(colorString, color);
}

wxString HtmlHexOutput::getFormat() const
{
    return "#%02X%02X%02X";
}
std::string HtmlHexOutput::getName() const
{
    return "Hexadecimal (HTML/CSS)";
}
std::string HtmlHexOutput::format(const wxColour& color)
{
    return wxString::Format(getFormat(), color.Red(), color.Green(), color.Blue()).ToStdString();
}
bool HtmlHexOutput::parseColor(std::string colorString, wxColour& color)
{
    wxRegEx regex("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
    if (regex.Matches(colorString))
    {
        long red = 0, green = 0, blue = 0;
        regex.GetMatch(colorString, 1).ToLong(&red, 16);
        regex.GetMatch(colorString, 2).ToLong(&green, 16);
        regex.GetMatch(colorString, 3).ToLong(&blue, 16);
        color.Set(red, green, blue);
        return true;
    }
    return false;
}

wxString CssRgbOutput::getFormat() const
{
    return "rgb(%3d, %3d, %3d)";
}

std::string CssRgbOutput::getName() const
{
    return "Decimal RGB (CSS)";
}
bool CssRgbOutput::parseColor(std::string colorString, wxColour& color)
{
    wxRegEx regex("rgb *\\( *([0-9]{1,3}) *, *([0-9]{1,3}) *, *([0-9]{1,3}) *\\)", wxRE_ICASE);
    if (regex.Matches(colorString))
    {
        long red = 0, green = 0, blue = 0;
        regex.GetMatch(colorString, 1).ToLong(&red);
        regex.GetMatch(colorString, 2).ToLong(&green);
        regex.GetMatch(colorString, 3).ToLong(&blue);
        color.Set(red, green, blue);
        return true;
    }
    return false;
}

wxString CssHslOutput::getFormat() const
{
    return "hsl(%3d, %3d%%, %3d%%)";
}

std::string CssHslOutput::getName() const
{
    return "Decimal HSL (CSS)";
}
std::string CssHslOutput::format(const wxColour& color)
{
    model.setColor(color);
    return wxString::Format(getFormat(), model.getValue(0), model.getValue(1), model.getValue(2)).ToStdString();
}
bool CssHslOutput::parseColor(std::string colorString, wxColour& color)
{
    wxRegEx regex("hsl *\\( *([0-9]{1,3}) *, *([0-9]{1,3}) *%? *, *([0-9]{1,3}) *%? *\\)", wxRE_ICASE);
    if (regex.Matches(colorString))
    {
        long h = 0, s = 0, l = 0;
        regex.GetMatch(colorString, 1).ToLong(&h);
        regex.GetMatch(colorString, 2).ToLong(&s);
        regex.GetMatch(colorString, 3).ToLong(&l);
        model.setValue(0, h);
        model.setValue(1, s);
        model.setValue(2, l);
        color = model.getColor();
        return true;
    }
    return false;
}

wxString HexOutput::getFormat() const
{
    return "%02X%02X%02X";
}

std::string HexOutput::getName() const
{
    return "Hexadecimal RGB";
}


wxString RgbFloatOutput::getFormat() const
{
    return "%.2lf, %.2lf, %.2lf";
}

std::string RgbFloatOutput::format(const wxColour& color)
{
    return wxString::Format(getFormat(), color.Red() / 255.0, color.Green() / 255.0, color.Blue() / 255.0).ToStdString();
}

std::string RgbFloatOutput::getName() const
{
    return "Float RGB";
}
bool RgbFloatOutput::parseColor(std::string colorString, wxColour& color)
{
    wxRegEx regex("([0-9]+(?:\\.[0-9]+)?) *, *([0-9]+(?:\\.[0-9]+)?) *, *([0-9]+(?:\\.[0-9]+)?)", wxRE_ADVANCED);
    if (regex.Matches(colorString))
    {
        double red = 0, green = 0, blue = 0;
        regex.GetMatch(colorString, 1).ToDouble(&red);
        regex.GetMatch(colorString, 2).ToDouble(&green);
        regex.GetMatch(colorString, 3).ToDouble(&blue);
        color.Set((int) (red * 255), (int) (green * 255), (int) (blue * 255));
        return true;
    }
    return false;
}
