#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h" // Base class: ToolWindow

class PaletteTool : public ToolWindow
{
public:
    PaletteTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
};

#endif // PALETTETOOL_H
