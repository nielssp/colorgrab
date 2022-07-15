// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include "colordnd.h"

#include <wx/config.h>
#include <wx/timer.h>
#include <map>

class ToolWindow;
class ColorOutput;
class IColorModel;

/// ColorGrab main window.
class MainFrame : public MainFrameBaseClass, public IColorReceiver
{
private:
    bool capturing;
    wxTimer refreshTimer;
    wxConfig config;
    std::map<int, int> timerIntervals;
    std::map<int, IColorModel*> colorModels;
    IColorModel* colorModel;
    std::map<int, ColorOutput*> colorOutputs;
    std::map<int, wxPanel*> stackColors;
    std::map<int, ToolWindow*> tools;
    std::map<int, std::pair<wxString, wxCursor> > cursors;
    ColorOutput* colorOutput;
    void RestorePosition();
    void update_label_and_ctrl(int i, IColorModel* colorModel, wxStaticText* label, wxSpinCtrl* ctrl);

public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
    
    wxConfigBase *GetConfig();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    virtual bool Show(bool show = true);

    /// Add a color model to the bottom of the color model menu.
    int AddColorModel(IColorModel* colorModel);
    
    /// Set the current color model.
    void SetColorModel(IColorModel* colorModel);
    
    /// Update values of current color model.
    void UpdateColorModel();
    
    /// Get current color model.
    IColorModel* GetColorModel() const;

    /// Add an output format to the bottom of the output menu.
    int AddColorOutput(ColorOutput* colorOutput);
    
    /// Set the current output format.
    void SetColorOutput(ColorOutput* colorOutput);
    
    /// Get current output format.
    ColorOutput* GetColorOutput() const;

    /// Add a tool to the bottom of the tools-menu.
    void AddTool(ToolWindow* tool);
    
    /// Add a cursor to the bottom of the cursors-menu
    void AddCursor(const wxString& label, const wxCursor& cursor);

    /// Push a color to the top of the color stack.
    void PushColor(const wxColour& color);

    /// Get currently selected color.
    wxColour GetColor() const;
    
    /// Set the current color.
    void SetColor(const wxColor& color, bool updateInputs = true, bool updateOutput = true);
    
    /// Pick color from current mouse position.
    void SetColorFromMouse();
    
    /// Pick color from a pixel on the screen.
    void SetColorFromPixel(wxCoord x, wxCoord y);
    
    /// Parse a color using the current output format.
    bool ParseColor(std::string colorString);
    
    /// Refresh the magnified image.
    void UpdateZoomArea();

protected:
    virtual void OnSetAlwaysOnTop(wxCommandEvent& event);
    virtual void OnDragColor(wxMouseEvent& event);
    virtual void OnCopyColor(wxCommandEvent& event);
    virtual void OnCopyMagnifiedArea(wxCommandEvent& event);
    virtual void OnPasteColor(wxCommandEvent& event);
    virtual void OnCapturePush(wxMouseEvent& event);
    virtual void OnPushColor(wxMouseEvent& event);
    virtual void OnRefreshImage(wxCommandEvent& event);
    virtual void OnTimerRefreshImage(wxCommandEvent& event);
    virtual void OnZoomIn(wxCommandEvent& event);
    virtual void OnZoomOut(wxCommandEvent& event);
    virtual void OnInputOutputEnter(wxCommandEvent& event);
    virtual void OnInputOutputBlur(wxFocusEvent& event);
    virtual void OnColorOutputChange(wxCommandEvent& event);
    virtual void OnZoomPanelDown(wxMouseEvent& event);
    virtual void OnZoomPanelMove(wxMouseEvent& event);
    virtual void OnZoomPanelUp(wxMouseEvent& event);
    virtual void OnZoomPanelZoom(wxMouseEvent& event);
    virtual void OnCaptureZoom(wxMouseEvent& event);
    virtual void OnSystemColorPicker(wxCommandEvent& event);
    virtual void OnCaptureMove(wxMouseEvent& event);
    virtual void OnCaptureEnd(wxMouseEvent& event);
    virtual void OnCaptureStart(wxMouseEvent& event);
    virtual void OnColorChange(wxCommandEvent& event);
    virtual void OnStackColorClick(wxMouseEvent& event);
    virtual void OnZoomSelect(wxCommandEvent& event);
    virtual void OnSelectColorModel(wxCommandEvent& event);
    virtual void OnSelectColorOutput(wxCommandEvent& event);
    virtual void OnSelectTool(wxCommandEvent& event);
    virtual void OnSelectCursor(wxCommandEvent& event);
    virtual void OnSelectTiming(wxCommandEvent& event);
    virtual void OnRefreshTimerEvent(wxTimerEvent& event);
    virtual void OnSetAlign(wxCommandEvent& event);
    virtual void OnSetCommaSpace(wxCommandEvent& event);
};
#endif // MAINFRAME_H
