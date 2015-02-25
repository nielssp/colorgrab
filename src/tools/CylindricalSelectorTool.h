// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef CYLINDRICALSELECTORTOOL_H
#define CYLINDRICALSELECTORTOOL_H

#include "ToolWindow.h"

#include "colormodels.h"

class HueSlider;
class CrossSection;

class CylindricalSelectorTool : public ToolWindow
{
private:
    double hue;
    double saturation;
    double lightness;
    HueSlider *hueSlider;
    CrossSection *crossSection;
    HSLModel model;
public:
    CylindricalSelectorTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void UpdateColor(const wxColour& color);
    
    void SetHue(double hue);
    double GetHue();
    
    void SetSaturation(double saturation);
    double GetSaturation();
    
    void SetLightness(double lightness);
    double GetLightness();
    
    void OnPushColor(wxMouseEvent& event);
};

#endif // CYLINDRICALSELECTORTOOL_H
