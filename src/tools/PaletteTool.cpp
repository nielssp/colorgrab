#include "tools/PaletteTool.h"

#include "MainFrame.h"

#include "coloroutputs.h"

#include <wx/confbase.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/dataview.h>
#include <wx/brush.h>
#include <wx/dc.h>
#include <wx/artprov.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/regex.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>

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

PaletteTool::PaletteTool(MainFrame* main) : ToolWindow(main, wxID_ANY, "Palette Tool", wxDefaultPosition, wxSize(220, 300)), filePath(""), isSaved(true), isNew(true)
{
    toolBar = CreateToolBar();
    
    statusBar = CreateStatusBar();
    
    t_new = toolBar->AddTool(wxID_ANY, _("New Palette"), wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR, wxSize(16, 16)));
    t_open = toolBar->AddTool(wxID_ANY, _("Open Palette"), wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddSeparator();
    t_save = toolBar->AddTool(wxID_ANY, _("Save"), wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(16, 16)));
    t_saveAs = toolBar->AddTool(wxID_ANY, _("Save As..."), wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_TOOLBAR, wxSize(16, 16)));
    toolBar->AddSeparator();
    t_addColor = toolBar->AddTool(wxID_ANY, _("Add Color"), wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR, wxSize(16, 16)));
    t_removeColor = toolBar->AddTool(wxID_ANY, _("Remove Color"), wxArtProvider::GetBitmap(wxART_MINUS, wxART_TOOLBAR, wxSize(16, 16)));
    t_removeColor->Enable(false);
    toolBar->Realize();
    
    colorList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_ROW_LINES | wxDV_MULTIPLE);
    colorList->AppendColumn(new wxDataViewColumn("", new ColorColumnRenderer, 0));
    colorList->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_EDITABLE );
    Bind(wxEVT_DATAVIEW_SELECTION_CHANGED, &PaletteTool::OnColorSelected, this, colorList->GetId());
    Bind(wxEVT_DATAVIEW_ITEM_EDITING_DONE, &PaletteTool::OnNameEdited, this, colorList->GetId());
        
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnNew, this, t_new->GetId());
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnOpen, this, t_open->GetId());
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnSave, this, t_save->GetId());
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnSaveAs, this, t_saveAs->GetId());
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnAddColor, this, t_addColor->GetId());
    Bind(wxEVT_COMMAND_TOOL_CLICKED, &PaletteTool::OnRemoveColor, this, t_removeColor->GetId());
}

std::string PaletteTool::GetName()
{
    return "PaletteTool";
}

void PaletteTool::Store(wxConfigBase* config)
{
    ToolWindow::Store(config);
    
    config->Write("Width0", colorList->GetColumn(0)->GetWidth());
    config->Write("Width1", colorList->GetColumn(1)->GetWidth());
    
    config->Write("Saved", isSaved);
    config->Write("File", filePath);
    if (!isSaved)
    {
        wxString userData = wxStandardPaths::Get().GetUserDataDir();
        if (wxDir::Exists(userData) || wxDir::Make(userData))
        {
            wxDir dir(userData);
            SaveFile(wxString::Format("%s%s", dir.GetNameWithSep(), "unsaved.gpl"));
        }
    }
}

void PaletteTool::Restore(wxConfigBase* config)
{
    ToolWindow::Restore(config);
    
    colorList->GetColumn(0)->SetWidth(config->ReadLong("Width0", 30));
    colorList->GetColumn(1)->SetWidth(config->ReadLong("Width1", 160));
    
    isSaved = config->ReadBool("Saved", false);
    if (!isSaved)
    {
        wxString userData = wxStandardPaths::Get().GetUserDataDir();
        OpenFile(wxString::Format("%s%c%s", userData, wxFILE_SEP_PATH, "unsaved.gpl"));
        filePath = config->Read("File", wxT(""));
        isSaved = false;
        isNew = filePath.IsEmpty();
        SetStatusText(filePath);
    }
    else
    {
        wxString file = config->Read("File", wxT(""));
        if (!file.IsEmpty())
            OpenFile(file);
    }
}

void PaletteTool::AddColor(const wxColour& color, const wxString& name)
{
    wxVector<wxVariant> data;
    data.push_back(wxVariant(color));
    data.push_back(wxVariant(name));
    colorList->AppendItem(data);
    isSaved = false;
}

wxString read_line(wxFileInputStream& input)
{
    wxString line("");
    while (!input.Eof())
    {
        char c = input.GetC();
        if (c == '\r')
            continue;
        if (c == '\n')
            break;
        line << c; 
    }
    return line;
}

void write_line(wxFileOutputStream& output, const wxString& line)
{
    for (size_t i = 0; i < line.size(); i++)
        output.PutC(line[i]);
    output.PutC('\n');
}

void PaletteTool::OpenFile(const wxString& path)
{
    if (!wxFile::Exists(path))
    {
        SetStatusText("Could not open file");
        return;
    }
    wxFileInputStream input(path);
    if (!input.IsOk())
    {
        SetStatusText("Could not open file");
        return;
    }
    wxString t = read_line(input);
    if (t != "GIMP Palette")
    {
        SetStatusText("Invalid file format");
        return;
    }
    colorList->DeleteAllItems();
    t_removeColor->Enable(false);
    toolBar->Realize();
    // Read palette settings
    wxRegEx settingRegex("([^:]+): *(.*)");
    while (!input.Eof())
    {
        wxString line = read_line(input);
        if (line[0] == '#')
            break;
        if (settingRegex.Matches(line))
        {
            wxString key = settingRegex.GetMatch(line, 1);
            wxString value = settingRegex.GetMatch(line, 2);
        }
    }
    // Read palette colors
    wxRegEx colorRegex(" *([0-9]{1,3}) +([0-9]{1,3}) +([0-9]{1,3})[ \t]*(.*)");
    while (!input.Eof())
    {
        wxString line = read_line(input);
        if (colorRegex.Matches(line))
        {
            long red = 0, green = 0, blue = 0;
            colorRegex.GetMatch(line, 1).ToLong(&red);
            colorRegex.GetMatch(line, 2).ToLong(&green);
            colorRegex.GetMatch(line, 3).ToLong(&blue);
            wxString name = colorRegex.GetMatch(line, 4);
            AddColor(wxColour(red, green, blue), name);
        }
    }
    filePath = path;
    isSaved = true;
    isNew = false;
    SetStatusText(filePath);
}

void PaletteTool::SaveFile(const wxString& path)
{
    wxFileName fileName(path);
    wxFileOutputStream output(path);
    if (!output.IsOk())
    {
        SetStatusText("Could not open file");
        return;
    }
    write_line(output, "GIMP Palette");
    write_line(output, wxString::Format("Name: %s", fileName.GetName()));
    write_line(output, "#");
    for (int i = 0; i < colorList->GetItemCount(); i++)
    {
        wxColour color;
        wxVariant variant;
        colorList->GetValue(variant, i, 0);
        color << variant;
        wxString name = colorList->GetTextValue(i, 1);
        write_line(output, wxString::Format("%3d %3d %3d\t%s", color.Red(), color.Green(), color.Blue(), name));
    }
    filePath = path;
    isSaved = true;
    isNew = false;
}

void PaletteTool::OnNew(wxCommandEvent& event)
{
    if (!isSaved) {
         if (wxMessageBox(_("Current palette has not been saved! Proceed?"), _("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
    }
    colorList->DeleteAllItems();
    isSaved = true;
    isNew = true;
    filePath = "";
}

void PaletteTool::OnOpen(wxCommandEvent& event)
{
    if (!isSaved) {
         if (wxMessageBox(_("Current palette has not been saved! Proceed?"), _("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
    }
    wxFileDialog openFileDialog(main, _("Open palette file"), "", "", "GIMP palette (*.gpl)|*.gpl", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    OpenFile(openFileDialog.GetPath());
}

void PaletteTool::OnSave(wxCommandEvent& event)
{
    if (isNew)
    {
        OnSaveAs(event);
    }
    else if (!isSaved)
    {
        SaveFile(filePath);
    }
}

void PaletteTool::OnSaveAs(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(main, _("Save palette file"), "", "", "GIMP palette (*.gpl)|*.gpl", wxFD_SAVE);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    SaveFile(saveFileDialog.GetPath());
}


void PaletteTool::OnAddColor(wxCommandEvent& event)
{
    wxColour color = main->GetColor();
    AddColor(color, main->GetColorOutput()->getOutput());
}


void PaletteTool::OnRemoveColor(wxCommandEvent& event)
{
    if (colorList->HasSelection())
    {
        wxDataViewItemArray items;
        int n = colorList->GetSelections(items);
        for (int i = 0; i < n; i++)
        {
            colorList->DeleteItem(colorList->ItemToRow(items[i]));
        }
        t_removeColor->Enable(false);
        toolBar->Realize();
    }
}

void PaletteTool::OnColorSelected(wxDataViewEvent& event)
{
    t_removeColor->Enable(event.GetItem().IsOk());
    toolBar->Realize();
}

void PaletteTool::OnNameEdited(wxDataViewEvent& event)
{
    isSaved = false;
}
