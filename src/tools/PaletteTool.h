#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h" // Base class: ToolWindow

class wxToolBar;
class wxStatusBarBase;
class wxDataViewListCtrl;
class wxDataViewEvent;
class wxToolBarToolBase;

class PaletteTool : public ToolWindow
{
private:
    wxToolBar* toolBar;
    wxStatusBarBase* statusBar;
    wxDataViewListCtrl* colorList;
    
    wxToolBarToolBase* t_new;
    wxToolBarToolBase* t_open;
    wxToolBarToolBase* t_save;
    wxToolBarToolBase* t_saveAs;
    wxToolBarToolBase* t_addColor;
    wxToolBarToolBase* t_removeColor;
    
protected:
    virtual void OnNew(wxCommandEvent& event);
    virtual void OnOpen(wxCommandEvent& event);
    virtual void OnSave(wxCommandEvent& event);
    virtual void OnSaveAs(wxCommandEvent& event);

    virtual void OnAddColor(wxCommandEvent& event);
    virtual void OnRemoveColor(wxCommandEvent& event);
    
    virtual void OnColorSelected(wxDataViewEvent& event);

public:
    PaletteTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
    
    void AddColor(const wxColour& color, const wxString& name);
    
    void OpenFile(const wxString& path);
};

#endif // PALETTETOOL_H
