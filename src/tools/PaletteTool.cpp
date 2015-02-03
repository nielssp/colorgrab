#include "tools/PaletteTool.h"

#include <wx/confbase.h>
#include <wx/toolbar.h>
#include <wx/dataview.h>
#include <wx/brush.h>
#include <wx/dc.h>

class ColorColumnRenderer : public wxDataViewCustomRenderer
{
private:
    wxColour color;
public:
    ColorColumnRenderer() : wxDataViewCustomRenderer("wxColour")
    {
    }
    bool GetValue(wxVariant& value) const
    {
        wxVariant variant;
        variant << color;
        return variant;
    }
    bool SetValue(const wxVariant& value)
    {
        color << value;
        return true;
    }
    wxSize GetSize() const
    {
        return wxSize(20, 10);
    }
    bool Render(wxRect cell, wxDC* dc, int state)
    {
        wxBrush brush(color);
        dc->SetBackground(brush);
        dc->SetBrush(brush);
        dc->DrawRectangle(cell);
        return true;
    }
};

PaletteTool::PaletteTool(MainFrame* main) : ToolWindow(main, wxID_ANY, "Palette Tool")
{
    toolBar = CreateToolBar();
    
    colorList = new wxDataViewListCtrl(this, wxID_ANY);
    colorList->AppendColumn(new wxDataViewColumn("", new ColorColumnRenderer, 0));
    colorList->AppendTextColumn("Color");
    colorList->AppendTextColumn("Name");
    
    AddColor(wxColour(102, 0, 0), "Redish");
    AddColor(wxColour(50, 200, 0), "Greenish");
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

void PaletteTool::AddColor(const wxColour& color, const std::string& name)
{
    wxVector<wxVariant> data;
    data.push_back(wxVariant(color));
    data.push_back(wxVariant("rgb"));
    data.push_back(wxVariant(name));
    colorList->AppendItem(data);
//    long index = colorList->InsertItem(1, "");
//    colorList->SetItemBackgroundColour(index, color);
//    colorList->SetItem(index, 2, name);
}
