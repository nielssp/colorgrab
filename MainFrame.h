#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "IColorModel.h"
#include "IColorOutput.h"
#include "wxcrafter.h"

#include <wx/config.h>
#include <map>

class MainFrame : public MainFrameBaseClass
{
private:
    wxConfig config;
    bool capturing;
	std::map<int, IColorModel*> colorModels;
    IColorModel* colorModel;
    std::map<int, IColorOutput*> colorOutputs;
    IColorOutput* colorOutput;
    void RestorePosition();
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    void AddColorModel(IColorModel* colorModel);
    void SetColorModel(IColorModel* colorModel);
    void UpdateColorModel();
    
    void AddColorOutput(IColorOutput* colorOutput);
    void SetColorOutput(IColorOutput* colorOutput);
	 
    wxColour GetColor() const;
	 void SetColor(const wxColor& color, bool updateInputs = true, bool updateOutput = true);
	 void SetColorFromMouse();
	 void SetColorFromPixel(wxCoord x, wxCoord y);
	 void UpdateZoomArea();
protected:
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
    virtual void OnFormatChoose(wxMenuEvent& event);
    virtual void OnFormatClick(wxCommandEvent& event);
    virtual void OnSystemColorPicker(wxCommandEvent& event);
    virtual void OnCaptureMove(wxMouseEvent& event);
    virtual void OnCaptureEnd(wxMouseEvent& event);
    virtual void OnCaptureStart(wxMouseEvent& event);
    virtual void OnColorPick(wxColourPickerEvent& event);
    virtual void OnColorChange(wxCommandEvent& event);
    virtual void OnZoomSelect(wxCommandEvent& event);
    virtual void OnSelectColorModel(wxCommandEvent& event);
    virtual void OnSelectColorOutput(wxCommandEvent& event);
};
#endif // MAINFRAME_H
