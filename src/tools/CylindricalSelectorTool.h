// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef CYLINDRICALSELECTORTOOL_H
#define CYLINDRICALSELECTORTOOL_H

#include "ToolWindow.h"

#include "colormodels.h"

#include <vector>

class HueSlider;
class CrossSection;
class wxChoice;

struct CylColorSpace;

class CylindricalSelectorTool : public ToolWindow
{
private:
    std::vector<CylColorSpace*> colorSpaces;
    double hue;
    double a;
    double b;
    HueSlider* hueSlider;
    CrossSection* crossSection;
    wxChoice* spaceChoice;
    wxColour (*toRgb)(double hue, double a, double b);
    IColorModel* model;
    
    void AddColorSpace(const wxString& name, wxColour (*toRgb)(double hue, double a, double b), IColorModel *model);
    void SetColorSpace(CylColorSpace* colorSpace);
public:
    CylindricalSelectorTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
    
    virtual void UpdateColor(const wxColour& color);
    
    wxColour ToRgb(double hue, double a, double b);
    
    void SetHue(double hue);
    double GetHue();
    
    void SetA(double a);
    double GetA();
    
    void SetB(double b);
    double GetB();
    
    void OnPushColor(wxMouseEvent& event);
    void OnSelectColorSpace(wxCommandEvent& event);
};

#endif // CYLINDRICALSELECTORTOOL_H
