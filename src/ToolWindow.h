#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include "wx/frame.h"

class MainFrame;

class ToolWindow : public wxFrame
{
public:
    ToolWindow(MainFrame* main, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxSYSTEM_MENU | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP | wxCLOSE_BOX | wxFRAME_NO_TASKBAR);

    virtual std::string GetName() = 0;
private:
    void OnClose(wxCloseEvent& event);
};

#endif // TOOLWINDOW_H
