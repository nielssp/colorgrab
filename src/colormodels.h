#ifndef COLORMODELS_H
#define COLORMODELS_H

#include <wx/colour.h>

class IColorModel
{
public:
    virtual ~IColorModel(){};

    virtual std::string getName() const = 0;
    virtual wxColor getColor() const = 0;
    virtual std::string getLabel(int i) const = 0;
    virtual int getNumComponents() const = 0;
    virtual int getValue(int i) const = 0;
    virtual int getMin(int i) const = 0;
    virtual int getMax(int i) const = 0;
    virtual void setColor(const wxColour& color) = 0;
    virtual void setValue(int i, int value) = 0;
};

class RGBModel : public IColorModel
{
private:
    wxColour color;
    
public:
    virtual std::string getName() const;
    virtual wxColor getColor() const;
    virtual std::string getLabel(int i) const;
    virtual int getNumComponents() const;
    virtual int getValue(int i) const;
    virtual int getMin(int i) const;
    virtual int getMax(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

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
    virtual int getMin(int i) const;
    virtual int getMax(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

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
    virtual int getMin(int i) const;
    virtual int getMax(int i) const;
    virtual void setColor(const wxColour& color);
    virtual void setValue(int i, int value);
};

#endif // colormodels_H