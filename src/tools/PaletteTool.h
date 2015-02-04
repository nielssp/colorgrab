#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h" // Base class: ToolWindow

class wxToolBar;
class wxDataViewListCtrl;
class wxDataViewEvent;
class wxToolBarToolBase;

class PaletteTool : public ToolWindow
{
private:
    wxToolBar* toolBar;
    wxDataViewListCtrl* colorList;
    
    wxToolBarToolBase* t_newPalette;
    wxToolBarToolBase* t_open;
    wxToolBarToolBase* t_save;
    wxToolBarToolBase* t_saveAs;
    wxToolBarToolBase* t_addColor;
    wxToolBarToolBase* t_removeColor;
    
protected:
    virtual void OnColorSelected(wxDataViewEvent& event);
    virtual void OnAddColorClick(wxCommandEvent& event);
    virtual void OnRemoveColorClick(wxCommandEvent& event);

public:
    PaletteTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
    
    void AddColor(const wxColour& color, const std::string& name);
};

#endif // PALETTETOOL_H
