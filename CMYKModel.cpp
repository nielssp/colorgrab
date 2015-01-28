#include "CMYKModel.h"

#include <cmath>
#include <algorithm>

std::string CMYKModel::getName() const
{
    return "CMYK";
}

wxColor CMYKModel::getColor() const
{
    int r = round(255.0 * (1 - c / 100.0) * (1 - k / 100.0));
    int g = round(255.0 * (1 - m / 100.0) * (1 - k / 100.0));
    int b = round(255.0 * (1 - y / 100.0) * (1 - k / 100.0));
    return wxColour(r, g, b);
}

std::string CMYKModel::getLabel(int i) const
{
    switch (i) {
        case 0: return "Cyan";
        case 1: return "Magenta";
        case 2: return "Yellow";
        case 3: return "Key";
        default: return "";
    }
}
int CMYKModel::getNumComponents() const
{
    return 4;
}
int CMYKModel::getValue(int i) const
{
    switch (i) {
        case 0: return c;
        case 1: return m;
        case 2: return y;
        case 3: return k;
        default: return 0;
    }
}
int CMYKModel::getMin(int i) const
{
    return 0;
}
int CMYKModel::getMax(int i) const
{
    return 100;
}
void CMYKModel::setColor(const wxColour& color)
{
    double r = color.Red() / 255.0;
    double g = color.Green() / 255.0;
    double b = color.Blue() / 255.0;
    double K = 1.0 - std::max(std::max(r, g), b);
    if (K != 1)
    {
        double C = (1.0 - r - K) / (1 - K);
        double M = (1.0 - g - K) / (1 - K);
        double Y = (1.0 - b - K) / (1 - K);
        c = round(C * 100);
        m = round(M * 100);
        y = round(Y * 100);        
    }
    k = round(K * 100);
}
void CMYKModel::setValue(int i, int value)
{
    if (value < 0) value = 0;
    if (value > 100) value = 100;
    switch (i) {
        case 0: c = value; break;
        case 1: m = value; break;
        case 2: y = value; break;
        case 3: k = value; break;
    }
}
