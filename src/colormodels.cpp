#include "colormodels.h"

#include <cmath>
#include <algorithm>

std::string RGBModel::getName() const
{
    return "RGB";
}

wxColour RGBModel::getColor() const
{
    return color;
}

std::string RGBModel::getLabel(int i) const
{
    switch (i) {
        case 0: return "Red";
        case 1: return "Green";
        case 2: return "Blue";
        default: return "";
    }
}
int RGBModel::getNumComponents() const
{
    return 3;
}
int RGBModel::getValue(int i) const
{
    switch (i) {
        case 0: return color.Red();
        case 1: return color.Green();
        case 2: return color.Blue();
        default: return 0;
    }
}
int RGBModel::getMin(int i) const
{
    return 0;
}
int RGBModel::getMax(int i) const
{
    return 255;
}

void RGBModel::setColor(const wxColour& color)
{
    this->color = color;
}

void RGBModel::setValue(int i, int value)
{
    if (value < 0)
        value = 0;
    else if (value > 255)
        value = 255;
    switch (i) {
        case 0: color.Set(value, color.Green(), color.Blue()); break;
        case 1: color.Set(color.Red(), value, color.Blue()); break;
        case 2: color.Set(color.Red(), color.Green(), value); break;
    }
}



std::string HSLModel::getName() const
{
    return "HSL";
}

int hueToRgb(double m1, double m2, double hue)
{
    double v;
	if (hue < 0)
		hue += 1;
	else if (hue > 1)
		hue -= 1;
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

wxColour hslToRgb(double h, double s, double l)
{
    int r, g, b;
    if (s == 0)
    {
        r = g = b = l * 255;
    }
    else
    {
        double m2; // m2 = C / 2 + 1
        if (l <= 0.5)
            m2 = l * (s + 1);
        else
            m2 = l + s - l * s;
        double m1 = l * 2 - m2;
        r = hueToRgb(m1, m2, h + 1.0 / 3.0);
        g = hueToRgb(m1, m2, h);
        b = hueToRgb(m1, m2, h - 1.0 / 3.0);
    }
    return wxColour(r, g, b);
}

wxColour HSLModel::getColor() const
{
    return hslToRgb(h / 360.0, s / 100.0, l / 100.0);
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
int HSLModel::getMin(int i) const
{
    return 0;
}
int HSLModel::getMax(int i) const
{
    switch (i) {
        case 0: return 360;
        case 1: return 100;
        case 2: return 100;
        default: return 0;
    }
}

#include <stdlib.h>

void HSLModel::setColor(const wxColour& color)
{
    double r = color.Red() / 255.0;
    double g = color.Green() / 255.0;
    double b = color.Blue() / 255.0;
    double M = std::max(std::max(r, g), b);
    double m = std::min(std::min(r, g), b);
    double C = M - m;
    double L = 0.5 * (M + m);
    double H = 0;
    double S = 0;
    if (C != 0)
    {
        if (M == r)
        {
            H = (g - b) / C;
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
    if (L != 0 && L != 1)
    {
        S = C / (1 - std::abs(2 * L - 1));
    }
    h = round(60 * H);
    if (h < 0) h += 360;
    if (h >= 360) h -= 360;
    s = round(S * 100);
    l = round(L * 100);
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

std::string HSVModel::getName() const
{
    return "HSV";
}

wxColour hsvToRgb(double h, double s, double v)
{
    if (h >= 1) h -= 1;
    if (h < 0) h += 1;
    if (s == 0)
        return wxColour(v * 255, v * 255, v * 255);
    h *= 6;
    int i = (int) h;
    double f = h - i;
    double p = v * ( 1 - s );
	double q = v * ( 1 - s * f );
	double t = v * ( 1 - s * ( 1 - f ) );
    switch (i) {
        case 0:
            return wxColour(v * 255, t * 255, p * 255);
        case 1:
            return wxColour(q * 255, v * 255, p * 255);
        case 2:
            return wxColour(p * 255, v * 255, t * 255);
        case 3:
            return wxColour(p * 255, q * 255, v * 255);
        case 4:
            return wxColour(t * 255, p * 255, v * 255);
        default:
            return wxColour(v * 255, p * 255, q * 255);
    }
}

wxColour HSVModel::getColor() const
{
    return hsvToRgb(h / 360.0, s / 100.0, v / 100.0);
}

std::string HSVModel::getLabel(int i) const
{
    switch (i) {
        case 0: return "Hue";
        case 1: return "Saturation";
        case 2: return "Value";
        default: return "";
    }
}
int HSVModel::getNumComponents() const
{
    return 3;
}
int HSVModel::getValue(int i) const
{
    switch (i) {
        case 0: return h;
        case 1: return s;
        case 2: return v;
        default: return 0;
    }
}
int HSVModel::getMin(int i) const
{
    return 0;
}
int HSVModel::getMax(int i) const
{
    switch (i) {
        case 0: return 360;
        case 1: return 100;
        case 2: return 100;
        default: return 0;
    }
}
void HSVModel::setColor(const wxColour& color)
{
    double r = color.Red() / 255.0;
    double g = color.Green() / 255.0;
    double b = color.Blue() / 255.0;
    double M = std::max(std::max(r, g), b);
    double m = std::min(std::min(r, g), b);
    double C = M - m;
    double V = M;
    double H = 0;
    double S = 0;
    if (C != 0)
    {
        if (M == r)
        {
            H = (g - b) / C;
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
    if (M != 0)
    {
        S = C / M;
    }
    h = round(60 * H);
    if (h < 0) h += 360;
    if (h >= 360) h -= 360;
    s = round(S * 100);
    v = round(V * 100);
}
void HSVModel::setValue(int i, int value)
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
            v = value;
            break;
    }
}


std::string CMYKModel::getName() const
{
    return "CMYK";
}

wxColour CMYKModel::getColor() const
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
