// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#include "ToolWindow.h"

#include "MainFrame.h"

#include <wx/confbase.h>

ToolWindow::ToolWindow(MainFrame *main, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(main, id, title, pos, size, style)
{
    this->main = main;
    Bind(wxEVT_CLOSE_WINDOW, &ToolWindow::OnClose, this);
}

ToolWindow::~ToolWindow()
{
}

/// Hides window instead of destroying.
void ToolWindow::OnClose(wxCloseEvent& event)
{
    Hide();
}

/// Restores previous window position.
void ToolWindow::RestorePosition(wxConfigBase* config)
{
    wxPoint current = GetScreenPosition();
    int x = config->ReadLong("Position/X", current.x);
    int y = config->ReadLong("Position/Y", current.y);
    wxSize screenSize = wxGetDisplaySize();
    wxSize windowSize = GetSize();
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > (screenSize.x - windowSize.x)) x = screenSize.x - windowSize.x;
    if (y > (screenSize.y - windowSize.y)) y = screenSize.y - windowSize.y;
    SetPosition(wxPoint(x, y));
}

bool ToolWindow::Show(bool show)
{
  bool result = wxFrame::Show(show);
  // Under some window managers position can't be restored until after the window
  // has been opened (possibly?)
  wxPoint wantedPosition = GetPosition();
  Center();
  Move(wantedPosition);
  return result;
}

void ToolWindow::Store(wxConfigBase* config)
{
    wxPoint pos = GetPosition();
    config->Write("Position/X", pos.x);
    config->Write("Position/Y", pos.y);
    config->Write("IsOpen", IsVisible());
    
    wxSize size = GetSize();
    config->Write("Size/X", size.x);
    config->Write("Size/Y", size.y);
}

void ToolWindow::Restore(wxConfigBase* config)
{
    RestorePosition(config);
    
    SetSize(
        config->ReadLong("Size/X", GetSize().x),
        config->ReadLong("Size/Y", GetSize().y)
    );
    
    Show(config->ReadBool("IsOpen", false));
}

void ToolWindow::UpdateColor(const wxColour& color)
{
}