#ifndef RGBMODEL_H
#define RGBMODEL_H

#include "IColorModel.h" // Base class: IColorModel

class RGBModel : public IColorModel
{
private:
    wxColour color;
    
public:
    RGBModel();
    
    virtual const wxColor& getColor() const;
    virtual std::string getLabel(int i) const;
    virtual int getNumComponents() const;
    virtual int getValue(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

#endif // RGBMODEL_H
