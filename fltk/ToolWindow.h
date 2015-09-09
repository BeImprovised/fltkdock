//
// $Id: ToolWindow.h,v 1.2 2006/06/24 20:01:59 z_hossain Exp $
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

#ifndef fltk_ToolWindow_h
#define fltk_ToolWindow_h

#include <fltk/DockFlags.h>
#include <fltk/DockWindow.h>


namespace fltk {

class Title : public Rectangle {
	friend class ToolWindow;
	
	const char *label_;
	Color color_;
	int flags_;
	
	Color darker_color(Color c);
	
public:
	enum {
		PUSHED = 0x01,
		HIGHLIGHTED = 0x02,
	};
	Title(const char*, Widget*, int = 18);
	~Title();
	
	void draw(Widget *);
	int handle(int, DockWindow *);
	Flags set_flag(int c) { return flags_ |= c; }
	Flags clear_flag(int c) { return flags_ &= ~c; }
};

class FL_API ToolWindow : public DockWindow {
	Title *title_;
	
public:
	ToolWindow(int, int, int, int, const char*);
	~ToolWindow();
	
	void draw();
	void layout();
	int handle(int);
	void label(const char *a) { title_->label_ = a; }
	const char *label() const { return title_->label_; }
	Color titlecolor() { return title_->color_; }
	void titlecolor(Color c) { title_->color_ = c; }
};

}
#endif
//
// End of $Id: ToolWindow.h,v 1.2 2006/06/24 20:01:59 z_hossain Exp $
//

