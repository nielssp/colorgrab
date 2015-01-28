#ifndef CSSRGBOUTPUT_H
#define CSSRGBOUTPUT_H

#include "HtmlHexOutput.h"

class CssRgbOutput : public HtmlHexOutput
{
public:
    virtual wxString getFormat() const;
    virtual std::string getName() const;
};

#endif // CSSRGBOUTPUT_H
