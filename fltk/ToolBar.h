//
// $Id: ToolBar.h,v 1.2 2006/06/19 16:31:47 z_hossain Exp $
//
// A dockable window framework for Fast Light Tool Kit (FLTK).
//
// Copyright (C) 2006 MD. Z. Hossain.
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

#ifndef fltk_ToolBar_h
#define fltk_ToolBar_h

#include <fltk/GripperBar.h>

#include <fltk/PopupMenu.h>
#include <fltk/RepeatButton.h>


namespace fltk {

class ChildInfo {
	friend class ToolBar;
	
	Widget *widget_;
	uchar flag_;
	int iw_, ih_;
	int fgap_, rgap_;
	
public:	
	ChildInfo(Widget *, uchar = 0/*ToolBar::PLAIN*/, int = 0, int = 0, const char * = 0);
	
	Widget *widget() { return widget_; }
	int iw() const { return iw_; }
	int ih() const { return ih_; }
	
	int front_gap() const { return fgap_; }
	int rear_gap() const { return rgap_; }
	
	uchar flag() const { return flag_; }
};

class FL_API ToolBar : public GripperBar {
	ChildInfo **cinfo_;
	int ncinfo_;
	
protected:
	void add(ChildInfo *o);
	int find_info(Widget *);
	void draw_gripper();
	
public:
	enum {
		PLAIN = 0,
		FIXED_WIDTH = 1,
		FIXED_HEIGHT = 2,
	};
	ToolBar(int, int, int, int, const char *l = 0);
	~ToolBar();
	
	void add(Widget *w) { GripperBar::add(w); init_child(); }
	void add(Widget &w) { GripperBar::add(w); init_child(); }
	void layout();
	void init_child();
	
	void child_info(int, int = 0, int = 0, Widget * = 0);
	void end() { GripperBar::end();  init_child(); }
	ChildInfo *cinfo(int i) { return cinfo_[i]; }
	int cinfo() { return ncinfo_; }
};

}
#endif
//
// End of $Id: ToolBar.h,v 1.2 2006/06/19 16:31:47 z_hossain Exp $.
//
