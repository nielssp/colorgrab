#include "CssRgbOutput.h"

wxString CssRgbOutput::getFormat() const
{
    return "rgb(%3d, %3d, %3d)";
}

std::string CssRgbOutput::getName() const
{
    return "Decimal RGB (CSS)";
}