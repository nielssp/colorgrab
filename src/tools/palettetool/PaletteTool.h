#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h" // Base class: ToolWindow

class PaletteTool : public ToolWindow
{
public:
    PaletteTool(MainFrame* main);
    ~PaletteTool();

public:
    virtual
std::string GetName();
};

#endif // PALETTETOOL_H
