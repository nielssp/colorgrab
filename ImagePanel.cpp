#include "ImagePanel.h"
#include <wx/dc.h>
#include <wx/dcclient.h>

ImagePanel::ImagePanel(wxWindow* parent) : wxPanel(parent)
{
}


BEGIN_EVENT_TABLE(ImagePanel, wxPanel)

    EVT_PAINT(ImagePanel::paintEvent)

END_EVENT_TABLE()

void ImagePanel::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void ImagePanel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void ImagePanel::render(wxDC &dc)
{
	 if (image.IsOk())
		  dc.DrawBitmap(image, 0, 0, false);
}

void ImagePanel::SetImage(wxBitmap newImage)
{
	 image = newImage;
	 this->Refresh();
}