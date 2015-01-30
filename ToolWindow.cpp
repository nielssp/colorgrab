#include "ToolWindow.h"

#include "MainFrame.h"

ToolWindow::ToolWindow(MainFrame *main, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(main, id, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP | wxCLOSE_BOX | wxFRAME_NO_TASKBAR)
{
    Bind(wxEVT_CLOSE_WINDOW, &ToolWindow::OnClose, this);
}

void ToolWindow::OnClose(wxCloseEvent& event)
{
    Hide();
}