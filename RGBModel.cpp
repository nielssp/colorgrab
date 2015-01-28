#include "RGBModel.h"

std::string RGBModel::getName() const
{
    return "RGB";
}

wxColor RGBModel::getColor() const
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
