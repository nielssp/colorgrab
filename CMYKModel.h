#ifndef CMYKMODEL_H
#define CMYKMODEL_H

#include "IColorModel.h" // Base class: IColorModel

class CMYKModel : public IColorModel
{
private:
    int c;
    int m;
    int y;
    int k;

public:
    virtual std::string getName() const;
    virtual wxColor getColor() const;
    virtual std::string getLabel(int i) const;
    virtual int getNumComponents() const;
    virtual int getValue(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

#endif // CMYKMODEL_H
