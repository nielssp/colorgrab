#include "ToolWindow.h"

#include "MainFrame.h"

ToolWindow::ToolWindow(MainFrame *main, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(main, id, title, pos, size, style)
{
    Bind(wxEVT_CLOSE_WINDOW, &ToolWindow::OnClose, this);
}

void ToolWindow::OnClose(wxCloseEvent& event)
{
    Hide();
}