//
// $Id: GripperBar.h,v 1.2 2006/06/19 16:31:16 z_hossain Exp $
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

#ifndef fltk_GripperBar_h
#define fltk_GripperBar_h

#include <fltk/DockWindow.h>

namespace fltk {

class Gripper : public Rectangle {
	int thickness_;
	
public:
	Gripper(int = 15);
	void draw();

	int thickness() const { return thickness_; }
	void thickness(int t) { thickness_ = t; }
};


class FL_API GripperBar : public DockWindow {
	Gripper *gripper_;

protected:
	Gripper *gripper() const { return gripper_; }
	
public:
	GripperBar(int, int, int, int, const char * = 0);
	void layout();
	void draw();
	Flags orientation() const { return DockWindow::orientation(); }
	void orientation(Flags a, bool = false);
	Flags state() const { return DockWindow::state(); }
	void state(Flags a);
	void type(uchar t);
	uchar type() const { return DockWindow::type(); }
};

}
#endif

//
// End of $Id: GripperBar.h,v 1.2 2006/06/19 16:31:16 z_hossain Exp $.
//
