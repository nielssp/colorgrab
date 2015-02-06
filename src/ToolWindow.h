// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include "wx/frame.h"

#define DEFAULT_TOOL_WINDOW_STYLE (wxCAPTION | wxSYSTEM_MENU | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP | wxCLOSE_BOX | wxFRAME_NO_TASKBAR | wxRESIZE_BORDER)

class MainFrame;
class wxConfigBase;

/// A tool window opened from the tools-menu.
class ToolWindow : public wxFrame
{
protected:
    MainFrame* main;
    
    void RestorePosition(wxConfigBase* config);
public:
    ToolWindow(MainFrame* main, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = DEFAULT_TOOL_WINDOW_STYLE);
    virtual ~ToolWindow();

    virtual bool Show(bool show = true);

    /// Name of tool for use in configuration, should be in CamelCase.
    virtual std::string GetName() = 0;
    
    /// Store tool configuration.
    virtual void Store(wxConfigBase* config);
    
    /// Restore tool configuration.
    virtual void Restore(wxConfigBase* config);
private:
    void OnClose(wxCloseEvent& event);
};

#endif // TOOLWINDOW_H
