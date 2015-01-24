#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
{
private:
	 bool capturing;
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	 
	 void SetColor(const wxColor& color);
	 void SetColorFromPixel(wxCoord x, wxCoord y);
	 void UpdateZoomArea();
protected:
    virtual void OnCaptureMove(wxMouseEvent& event);
    virtual void OnCaptureEnd(wxMouseEvent& event);
    virtual void OnCaptureStart(wxMouseEvent& event);
    virtual void OnLeftDown(wxMouseEvent& event);
    virtual void OnColorPick(wxColourPickerEvent& event);
    virtual void OnColorChange(wxCommandEvent& event);
    virtual void OnGrabClick(wxCommandEvent& event);
};
#endif // MAINFRAME_H
