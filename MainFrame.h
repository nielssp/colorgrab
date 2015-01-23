#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
protected:
    virtual void OnColorPick(wxColourPickerEvent& event);
    virtual void OnColorChange(wxCommandEvent& event);
    virtual void OnGrabClick(wxCommandEvent& event);
};
#endif // MAINFRAME_H
