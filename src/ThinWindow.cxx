//
// $Id: ThinWindow.cxx,v 1.2 2006/06/24 20:00:06 z_hossain Exp $
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

#include <fltk/DockWindow.h>
#include <Utility.h>

#include <fltk/layout.h>
#include <fltk/Symbol.h>
#include <fltk/events.h>
#include <fltk/Cursor.h>
#include <fltk/draw.h>

using namespace fltk;

ThinWindow::ThinWindow(Group *g, bool c) : 
		Window(g->x(), g->y(), g->w(), g->h(), g->label()) {
	
	flags_ = 0;
	box(THIN_UP_BOX);
	ext[0] = box()->dw();
	ext[1] = box()->dh();
	resize(w() + ext[0], h() + ext[1]);
	border(false);
	resizer(false);
	transient(c);
// 	set_override();
}

void ThinWindow::layout() {
	if (!layout_damage()) return;
	
// 	if (!(layout_damage() & (LAYOUT_XYWH|LAYOUT_DAMAGE|LAYOUT_CHILD))) {
// 		return;
// 	}
	if(!children()) return;
	
	DockWindow *dw = (DockWindow *)child(0);
	resize(max(w(), dw->min_w() + ext[0]), max(h(), dw->min_h() + ext[1]));
	
	int damage = layout_damage() & LAYOUT_DAMAGE;
	
	Rectangle r(w(), h());
	box()->inset(r);
	
	if (!dw->visible()) return;
	dw->resize(r.x(), r.y(), r.w(), r.h());
	dw->layout_damage(dw->layout_damage() | damage);
	dw->layout();
	
	Window::layout();
}

void ThinWindow::draw() {
	Window::draw();
	
	if(!children()) return;
	if(resizer() && ((DockWindow *)child(0))->state() == fltk::DOCKED_NONE) {
		setcolor(GRAY15);
		line_style(SOLID, 3);
		drawline(w() - 2, h() - 10, w() - 2, h());
		drawline(w() - 10, h() - 2, w(), h() - 2);
		line_style(0);
	}
}

int ThinWindow::handle(int event) {
	if(!resizer()) return Window::handle(event);
	
	static int oldx = MAX_INT, oldy = MAX_INT;
	Rectangle rt(w(), h());
	get_absolute_rect(&rt);
	
	int cx = event_x_root();
	int cy = event_y_root();
	static Cursor *cr = 0;
	
	switch(event) {
		case PUSH:
			if(cr) { oldx = cx; oldy = cy; return 1; }
			break;
		case DRAG:
			if(oldx == MAX_INT && oldy == MAX_INT) break;
			if(abs(cx - oldx) <= 5 && abs(cy - oldy) <= 5) break;
			resize(w() + cx - oldx, h() + cy - oldy);
			oldx = cx; oldy = cy;
			return 1;
		case RELEASE:
			if(oldx != MAX_INT && oldy != MAX_INT) {
				oldx = oldy = MAX_INT;
			}
			break;
		case MOVE:
			if(cx >= rt.r() - 2 && cx <= rt.r() && cy >= rt.b() - 10) cursor(cr = CURSOR_NWSE);
			else if(cy >= rt.b() - 2 && cy <= rt.b() && cx >= rt.r() - 10) cursor(cr = CURSOR_NWSE);
			else cursor(cr = 0);
			return 1;
		case LEAVE:
			cursor(cr = 0);
	}
	return Window::handle(event);
}

//
// End of $Id: ThinWindow.cxx,v 1.2 2006/06/24 20:00:06 z_hossain Exp $
//
