#include "tools/PaletteTool.h"

#include <wx/confbase.h>

PaletteTool::PaletteTool(MainFrame* main) : ToolWindow(main, wxID_ANY, "Palette Tool")
{
}


std::string PaletteTool::GetName()
{
    return "PaletteTool";
}

void PaletteTool::Store(wxConfigBase* config)
{
    ToolWindow::Store(config);
}

void PaletteTool::Restore(wxConfigBase* config)
{
    ToolWindow::Restore(config);
}
