#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h" // Base class: ToolWindow

class wxToolBar;
class wxDataViewListCtrl;

class PaletteTool : public ToolWindow
{
private:
    wxToolBar* toolBar;
    wxDataViewListCtrl* colorList;
public:
    PaletteTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
    
    void AddColor(const wxColour& color, const std::string& name);
};

#endif // PALETTETOOL_H
