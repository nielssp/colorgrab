// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef PALETTETOOL_H
#define PALETTETOOL_H

#include "ToolWindow.h"
#include "colordnd.h"

class wxToolBar;
class wxStatusBarBase;
class wxDataViewListCtrl;
class wxDataViewEvent;
class wxToolBarToolBase;

class PaletteTool : public ToolWindow, public IColorReceiver
{
private:
    wxString filePath;
    bool isSaved;
    bool isNew;

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
    virtual void OnColorPush(wxDataViewEvent& event);
    virtual void OnColorDrag(wxDataViewEvent& event);
    virtual void OnNameEdited(wxDataViewEvent& event);
    
    virtual void OnKeyDown(wxKeyEvent& event);

public:
    PaletteTool(MainFrame* main);

    virtual std::string GetName();
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
    
    void RefreshTitle();
    
    void AddColor(const wxColour& color, const wxString& name);
    virtual bool ParseColor(std::string colorString);
    void RemoveSelectedColors();
    
    void OpenFile(const wxString& path);
    void SaveFile(const wxString& path);
    
    bool ConfirmSave();
};

#endif // PALETTETOOL_H
