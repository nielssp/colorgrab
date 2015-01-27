#include "HtmlHexOutput.h"

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
void HtmlHexOutput::setOutput(std::string output)
{
    wxRegEx regex("#?([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})");
    if (regex.Matches(output))
    {
        long red = 0, green = 0, blue = 0;
        regex.GetMatch(output, 1).ToLong(&red, 16);
        regex.GetMatch(output, 2).ToLong(&green, 16);
        regex.GetMatch(output, 3).ToLong(&blue, 16);
        color.Set(red, green, blue);
    }
}
