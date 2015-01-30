#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H

#include "wx/frame.h"

class MainFrame;

class ToolWindow : public wxFrame
{
public:
    ToolWindow(MainFrame* main, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    virtual std::string GetName() = 0;
private:
    void OnClose(wxCloseEvent& event);
};

#endif // TOOLWINDOW_H
