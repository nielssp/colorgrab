#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include "wx/frame.h"

#define DEFAULT_TOOL_WINDOW_STYLE (wxCAPTION | wxSYSTEM_MENU | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP | wxCLOSE_BOX | wxFRAME_NO_TASKBAR | wxRESIZE_BORDER)

class MainFrame;
class wxConfigBase;

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

    virtual std::string GetName() = 0;
    
    virtual void Store(wxConfigBase* config);
    virtual void Restore(wxConfigBase* config);
private:
    void OnClose(wxCloseEvent& event);
};

#endif // TOOLWINDOW_H
