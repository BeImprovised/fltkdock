//
// $Id: Preview.cxx,v 1.2 2006/06/24 20:02:12 z_hossain Exp $
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

#include <fltk/DockHolder.h>

#include <fltk/draw.h>

using namespace fltk;

Preview::Preview(int x, int y, int w, int h) : Rectangle(x, y, w, h), flags_(NO_FLAGS) {
	redraw_ = new Rectangle(x, y, w, h);
}

Preview::~Preview() {
	delete redraw_;
}

void Preview::move_preview() {
	if(flag(X_BEFORE)) move( -w() >> 1, 0);
	else if(flag(X_AFTER)) move( +w() >> 1, 0);
	else if(flag(Y_BEFORE)) move(0, -h() >> 1);
	else if(flag(Y_AFTER)) move(0, +h() >> 1);
}

void Preview::draw(Widget &to) {	
	Rectangle rf;
	translate(to, rf);
	setcolor(GRAY25);
	line_style(SOLID, 5);
	//since fltk only produce correct behavior for 0-thickness lines
	rf.move_x( +2);
	rf.move_r( -2);
	rf.move_y( +1);
	rf.move_b( -2);
	
	strokerect(rf);
	line_style(0);
}

Rectangle &Preview::translate(Widget &to, Rectangle &rf) {
	Rectangle r;
	to.get_absolute_rect(&r);
	rf.set(x() - r.x(), y() - r.y(), w(), h());
	return rf;
}

//
// End of $Id: Preview.cxx,v 1.2 2006/06/24 20:02:12 z_hossain Exp $
//
