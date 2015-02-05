// ColorGrab
// Copyright (c) 2015 Niels Sonnich Poulsen (http://nielssp.dk)
// Licensed under the MIT license.
// See the LICENSE file or http://opensource.org/licenses/MIT for more information.

#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <wx/bitmap.h>
#include <wx/panel.h>

class ImagePanel : public wxPanel
{
private:
	 wxBitmap image;
public:
	ImagePanel(wxWindow* parent);
	
	void paintEvent(wxPaintEvent & evt);
	void paintNow();
	
	void render(wxDC& dc);
	
	void SetImage(wxBitmap image);

	DECLARE_EVENT_TABLE()
};

#endif // IMAGEPANEL_H
