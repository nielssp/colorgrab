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
    ColorOutput* colorOutput;
    void RestorePosition();

public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
    
    wxConfigBase *GetConfig();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    virtual bool Show(bool show = true);

    int AddColorModel(IColorModel* colorModel);
    void SetColorModel(IColorModel* colorModel);
    void UpdateColorModel();
    IColorModel* GetColorModel() const;

    int AddColorOutput(ColorOutput* colorOutput);
    void SetColorOutput(ColorOutput* colorOutput);
    ColorOutput* GetColorOutput() const;

    void AddTool(ToolWindow* tool);

    void PushColor(const wxColour& color);

    wxColour GetColor() const;
    void SetColor(const wxColor& color, bool updateInputs = true, bool updateOutput = true);
    void SetColorFromMouse();
    void SetColorFromPixel(wxCoord x, wxCoord y);
    bool ParseColor(std::string colorString);
    void UpdateZoomArea();

protected:
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
    virtual void OnSelectTiming(wxCommandEvent& event);
    virtual void OnRefreshTimerEvent(wxTimerEvent& event);
};
#endif // MAINFRAME_H
