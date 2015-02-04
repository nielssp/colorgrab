#include "tools/PaletteTool.h"

#include <wx/confbase.h>
#include <wx/toolbar.h>
#include <wx/dataview.h>
#include <wx/brush.h>
#include <wx/dc.h>
#include <wx/artprov.h>

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
        return wxSize(30, 20);
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
    
    toolBar->AddTool(wxID_ANY, "New Palette", wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddTool(wxID_ANY, "Open Palette", wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddTool(wxID_ANY, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddTool(wxID_ANY, "Save As...", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_ANY, "Add Color", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddTool(wxID_ANY, "Remove Color", wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->Realize();
    
    colorList = new wxDataViewListCtrl(this, wxID_ANY);
    colorList->AppendColumn(new wxDataViewColumn("", new ColorColumnRenderer, 0));
    colorList->AppendTextColumn("Color");
    colorList->AppendTextColumn("Name", wxDATAVIEW_CELL_EDITABLE );
    
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
