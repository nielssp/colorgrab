#include "HSLModel.h"

#include <cmath>

std::string HSLModel::getName() const
{
    return "HSL";
}

wxColor HSLModel::getColor() const
{
    return wxColour();
}
std::string HSLModel::getLabel(int i) const
{
    switch (i) {
        case 0: return "Hue";
        case 1: return "Saturation";
        case 2: return "Lightness";
        default: return "";
    }
}
int HSLModel::getNumComponents() const
{
    return 3;
}
int HSLModel::getValue(int i) const
{
    switch (i) {
        case 0: return h;
        case 1: return s;
        case 2: return l;
        default: return 0;
    }
}
void HSLModel::setColor(const wxColour& color)
{
    double r = color.Red();
    double g = color.Green();
    double b = color.Blue();
    double M = g > b ? r > g ? r : g : b;
    double m = g < b ? r < g ? r : g : b;
    double C = M - m;
    double H1 = 0;
    if (C != 0)
    {
        if (M == r)
        {
            H1 = std::fmod((g - b) / C, 6);
        }
        if (M == g)
        {
            H1 = (b - r) / C + 2;
        }
        if (M == b)
        {
            H1 = (r - g) / C + 4;
        }
    }
    h = 60 * H1;
    l = 0.5 * (M + m);
    if (l == 0 || l == 100)
    {
        s = 0;
    }
    else
    {
        s = C / (1 - std::abs(2 * (l / 100.0) - 1));
    }
}
void HSLModel::setValue(int i, int value)
{
    switch (i) {
        case 0:
            if (value < 0) value = 0;
            if (value > 360) value = 360;
            h = value;
            break;
        case 1:
            if (value < 0) value = 0;
            if (value > 100) value = 100;
            s = value;
            break;
        case 2:
            if (value < 0) value = 0;
            if (value > 100) value = 100;
            l = value;
            break;
    }
}
