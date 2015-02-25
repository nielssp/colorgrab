// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef CYLINDRICALSELECTORTOOL_H
#define CYLINDRICALSELECTORTOOL_H

#include "ToolWindow.h"

class CylindricalSelectorTool : public ToolWindow
{
public:
    CylindricalSelectorTool(MainFrame* main);

    virtual std::string GetName();
};

#endif // CYLINDRICALSELECTORTOOL_H
