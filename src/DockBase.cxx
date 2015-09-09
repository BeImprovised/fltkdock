//
// $Id: DockBase.cxx,v 1.2 2006/06/23 21:35:22 z_hossain Exp $
//
// A dockable window framework for Fast Light Tool Kit (FLTK).
//
// Copyright (C) 2006 by MD. Z. Hossain
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to: 
//
//             z_hossain@users.sf.net
//

#include <fltk/DockFlags.h>
#include <DockBase.h>

#include <fltk/Window.h>

using namespace fltk;

/*
********************************************************************************
                                   Global
********************************************************************************
*/

/*! \class fltk::Global
	
	This class contains global static data and methods.
*/

/*! \fn bool Global::save_status(bool b)
	Enable Perspective saving if \a b is true and write user settings when 
	void DockHolder::write_status() is called. \n
	Default value is true.
*/

/*! \fn void Global::highlight_title(bool b)
	Highlight the title of a fltk::ToolWindow when any child of it, has 
	keyboard focus and \a b is true.\n
	Default value is true.
*/

/*! \fn void Global::fill_vertical(bool b)
	The common corner spaces are taken by the vertically docked window 
	instead of horizontally docked window if \a b is true.
	Default value is false.
*/

/*! \fn void Global::app_name(const char *c)
	\a c should be a unique identification of application name and must be a 
	valid directory name.\n
	An acceptable format can be \e application_name.vendor_domain_name \n
	Default value is "fltkdock.sourceforge.net"
*/

/*! \fn void Global::preview_delay(float d)
	Sets the delay \a d for dock preview. \n
	Default value is 0.3f.
*/

uchar Global::option_ = Global::SAVE_STATUS | Global::HIGHLIGHT_TITLE;
float Global::delay_ = 0.3f;
const char *Global::app_ = "fltkdock.sourceforge.net";

/*
********************************************************************************
                                   DockBase
********************************************************************************
*/

/*! \class fltk::DockBase

	The base class for dockable windows and dock holders and other classes 
	in FLTK Dock. It implements various mechanism for identifying objects 
	and its type. It also implements some other common functions.
*/

int DockBase::id_count_ = 0;

DockBase::DockBase(int X, int Y, int W, int H, const char *l) 
	: Group(X, Y, W, H, l) {
	when(WHEN_DOCK | WHEN_UNDOCK);
	type(DOCK_BASE);
	unique_id(++id_count_);
}

DockBase::~DockBase() {
	if(sizes_) delete [] sizes_;
}

void DockBase::push_sizes() {  //stack with capacity one
	sizes_ = new int[4 * (children() + 1)];
	sizes_[0] = x();
	sizes_[1] = y();
	sizes_[2] = w();
	sizes_[3] = h();
	int *p = sizes_ + 4;
	for(int i = 0; i < children(); i++) {
		*p++ = child(i)->x();
		*p++ = child(i)->y();
		*p++ = child(i)->w();
		*p++ = child(i)->h();
	}
}

void DockBase::pop_sizes() {
	resize(sizes_[0], sizes_[1], sizes_[2], sizes_[3]);	//set(sizes_[0], sizes_[1], sizes_[2], sizes_[3]);
	int *p = sizes_ + 4;
	for(int i = 0; i < children(); i++) {
		child(i)->resize(p[0], p[1], p[2], p[3]);	//child(i)->set(p[0], p[1], p[2], p[3]);
		p += 4;
	}
	delete [] sizes_;
}

/*! The top most parent window that contains this widget.
*/
Window *DockBase::parent_window() {
	Widget *w; Window *win;
	for(w = this; w; w = w->parent()) {
		win = dynamic_cast<Window *>(w);
		if(win) return win;
	}
	return 0;
}
//
// End of $Id: DockBase.cxx,v 1.2 2006/06/23 21:35:22 z_hossain Exp $
//
