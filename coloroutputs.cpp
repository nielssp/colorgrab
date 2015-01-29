#include "coloroutputs.h"

#include <wx/regex.h>

wxColor HtmlHexOutput::getColor() const
{
    return color;
}
wxString HtmlHexOutput::getFormat() const
{
    return "#%02X%02X%02X";
}
std::string HtmlHexOutput::getName() const
{
    return "Hexadecimal (HTML/CSS)";
}
std::string HtmlHexOutput::getOutput() const
{
    return wxString::Format(getFormat(), color.Red(), color.Green(), color.Blue()).ToStdString();
}
void HtmlHexOutput::setColor(const wxColour& color)
{
    this->color = color;
}
bool HtmlHexOutput::parseColor(std::string colorString)
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