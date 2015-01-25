#ifndef HSLMODEL_H
#define HSLMODEL_H

#include "IColorModel.h" // Base class: IColorModel

class HSLModel : public IColorModel
{
private:
    int h;
    int s;
    int l;

public:
    virtual std::string getName() const;
    virtual wxColor getColor() const;
    virtual std::string getLabel(int i) const;
    virtual int getNumComponents() const;
    virtual int getValue(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

#endif // HSLMODEL_H
