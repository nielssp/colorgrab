#ifndef ICOLORMODEL_H
#define ICOLORMODEL_H

#include <wx/colour.h>

class IColorModel
{
public:
   virtual ~IColorModel() {};
   
   virtual std::string getName() const = 0;
   
   virtual int getNumComponents() const = 0;
   
   virtual std::string getLabel(int i) const = 0;
   
   virtual int getValue(int i) const = 0;
   virtual void setValue(int i, int value) = 0;
   
   virtual wxColor getColor() const = 0;
   virtual void setColor(const wxColour& color) = 0;
};

#endif // ICOLORMODEL_H
