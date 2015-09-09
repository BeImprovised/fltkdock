//
// $Id: GripperBar.cxx,v 1.2 2006/06/19 16:33:20 z_hossain Exp $
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

#include <fltk/GripperBar.h>
#include <fltk/RegionHolder.h>

#include <fltk/draw.h>
#include <fltk/Symbol.h>
#include <fltk/layout.h>

#define GRIPPER_GAP 5
#define GRIPPER_DOT_GAP 6

using namespace fltk;

/*
********************************************************************************
                                     Gripper
********************************************************************************
*/

Gripper::Gripper(int t) : Rectangle(t, t) {
	thickness_ = t;
}

void Gripper::draw() {
	int X = w()/2, mx;
	int Y = h()/2, my;
	
	setcolor(GRAY33);
	fillrect(Rectangle(X, Y, 2, 2));
	setcolor(WHITE);
	fillrect(Rectangle(X + 1, Y + 1, 2, 2));
	
	for(int i = 1; true; i++) {
		if(w() < h()) {
			mx = 0; my = i * GRIPPER_DOT_GAP;
			if(my > Y - h()/6) break;
		} else {
			mx = i * GRIPPER_DOT_GAP; my = 0;
			if(mx > X - w()/6) break;
		}
		setcolor(GRAY33);
		fillrect(Rectangle(X + mx, Y + my, 2, 2));
		setcolor(WHITE);
		fillrect(Rectangle(X + mx + 1, Y + my + 1, 2, 2));
	
		setcolor(GRAY33);
		fillrect(Rectangle(X - mx, Y - my, 2, 2));
		setcolor(WHITE);
		fillrect(Rectangle(X - mx + 1, Y - my + 1, 2, 2));
	}
}
/*
********************************************************************************
                                     GripperBar
********************************************************************************
*/

/*! \class fltk::GripperBar

	The base class for fltk::ToolBar. Generally it is used to contain the 
	fltk::MenuBar
*/

GripperBar::GripperBar(int X, int Y, int W, int H, const char *l) 
	: DockWindow (X, Y, W, H, l) {
	gripper_ = new Gripper();
	type(GRIPPER_BAR);	//type(NORMAL);	//GRIP_MENUBAR, GRIPPER_BAR
	dockable(DOCKABLE_ALL);
	floatable(true);
	fillable(false);
	orientable(ORIENTABLE_BOTH);
	orientation(ORIENTED_HORIZONTAL, true);
	match_id(MATCH_BARHOLDER | MATCH_REGIONHOLDER);
	type(GRIPPER_BAR);
	dragger(gripper_);
	build_menu();
}

void GripperBar::layout() {
	if (!layout_damage()) return;

	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE)) || !children()) {
		DockWindow::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
  
	Rectangle r(w(), h());
	box()->inset(r);
	
	if(orientation()) {
		gripper_->set(r.x(), r.y(), gripper_->thickness(), r.h());
		r.move_x(gripper_->thickness());
	}
	else {
		gripper_->set(r.x(), r.y(), r.w(), gripper_->thickness());
		r.move_y(gripper_->thickness());
	}
	
	for (int i = 0; i < children(); i++) {
		Widget* widget = child(i);
		if (!widget->visible()) continue;
		widget->resize(r.x(), r.y(), r.w(), r.h());
		widget->layout_damage(widget->layout_damage()|damage);
		widget->layout();
	}
}

void GripperBar::draw() {
	DockWindow::draw();
	gripper_->draw();
}

void GripperBar::orientation(Flags a, bool init) {
	DockWindow::orientation(a, init);
	if(type() == GRIP_MENUBAR) {
		int i;
		for(i = 0; i < children() && orientation(); i++) child(i)->set_horizontal();
		for(i = 0; i < children() && !orientation(); i++) child(i)->set_vertical();
	}
}

void GripperBar::state(Flags a) {
	DockWindow::state(a);
	if(state() < DOCKED_NONE) box(ENGRAVED_BOX);
	else box(0);
	relayout();
}

void GripperBar::type(uchar t) {
	if(t == GRIP_MENUBAR) fillable(true);
	else fillable(false);
	DockWindow::type(t);
}
//
// End of $Id: GripperBar.cxx,v 1.2 2006/06/19 16:33:20 z_hossain Exp $.
//
