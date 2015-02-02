#include "PaletteTool.h"

PaletteTool::PaletteTool(MainFrame* main) : ToolWindow(main, wxID_ANY, "Palette Tool")
{
}

PaletteTool::~PaletteTool()
{
}

std::string PaletteTool::GetName()
{
    return "PaletteTool";
}
