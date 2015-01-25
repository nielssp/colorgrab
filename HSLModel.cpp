#include "HSLModel.h"

#include <cmath>
#include <algorithm>

std::string HSLModel::getName() const
{
    return "HSL";
}

int hueToRgb(double m1, double m2, double hue)
{
    double v;
	if (6.0 * hue < 1.0)
		v = m1 + (m2 - m1) * hue * 6.0;
	else if (2.0 * hue < 1.0)
		v = m2;
	else if (3.0 * hue < 2.0)
		v = m1 + (m2 - m1) * (2.0 / 3.0 - hue) * 6.0;
	else
		v = m1;
	return 255 * v;
}

wxColor HSLModel::getColor() const
{
    double S = s / 100.0;
    double L = l / 100.0;
    int r, g, b;
    if (s == 0)
    {
        r = g = b = L * 255;
    }
    else
    {
        double m2;
        if (L <= 0.5)
            m2 = L * (S + 1);
        else
            m2 = L + S - L * S;
        double m1 = L * 2 - m2;
        double H = h / 360.0;
        r = hueToRgb(m1, m2, H + 1.0 / 3.0);
        g = hueToRgb(m1, m2, H);
        b = hueToRgb(m1, m2, H - 1.0 / 3.0);
    }
    return wxColour(r, g, b);
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
    double r = color.Red() / 255.0;
    double g = color.Green() / 255.0;
    double b = color.Blue() / 255.0;
    double M = std::max(std::max(r, g), std::max(g, b));
    double m = std::min(std::min(r, g), std::min(g, b));
    double C = M - m;
    double H = 0;
    if (C != 0)
    {
        if (M == r)
        {
            H = std::fmod((g - b) / C, 6);
        }
        if (M == g)
        {
            H = (b - r) / C + 2;
        }
        if (M == b)
        {
            H = (r - g) / C + 4;
        }
    }
    double L = 0.5 * (M + m);
    double S = 0;
    if (L != 0 && L != 1)
    {
        S = C / (1 - std::abs(2 * L - 1));
    }
    h = 60 * H;
    s = S * 100;
    l = L * 100;
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
