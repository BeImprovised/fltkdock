//
// $Id: ToolBar.cxx,v 1.2 2006/06/18 10:35:48 z_hossain Exp $
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

#include <fltk/ToolBar.h>
#include <fltk/ToolDivider.h>
#include <Utility.h>

#include <fltk/Symbol.h>
#include <fltk/draw.h>
#include <fltk/layout.h>
#include <string.h>

using namespace fltk;

/*
********************************************************************************
                                   ToolDivider
********************************************************************************
*/

#define DIVIDER_GAP 8

/*! \class fltk::ToolDivider

	It is similar to the fltk::Divider but improve the look for adjust in fltk::ToolBar.
*/
ToolDivider::ToolDivider() : Divider() {
	w(DIVIDER_GAP);
	h(DIVIDER_GAP);
}

/*! Resize the wide to 8p according to the orientation.
*/
void ToolDivider::layout() {
	if(h() > w()) w(DIVIDER_GAP);
	else h(DIVIDER_GAP);
}

void ToolDivider::draw() {
	draw_background();
	setcolor( GRAY33 );
	if ( w() > h() ) {
		int y = ( h() - 1 ) / 2;
		drawline(4, y, w() - 5, y);
		setcolor(WHITE);
		drawline(4, y + 1, w() - 5, y + 1);
	}
	else if ( h() ) {
		int x = (w() - 1) / 2;
		drawline(x, 4, x, h() - 5);
		setcolor(WHITE);
		drawline(x + 1, 4, x + 1, h() - 5);
	}
}

/*
********************************************************************************
                                   ChildInfo
********************************************************************************
*/
ChildInfo::ChildInfo(Widget *o, uchar f, int fg, int rg, const char *ml) 
	: widget_(o), flag_(f), iw_(o->w()), ih_(o->h()), fgap_(fg), rgap_(rg) { }
/*
********************************************************************************
                                     ToolBar
********************************************************************************
*/

/*! \class fltk::ToolBar
		
	This class provide the functionality of toolbar.
*/

ToolBar::ToolBar(int X, int Y, int W, int H, const char *l) 
	: GripperBar(X, Y, W, H, l), cinfo_(0), ncinfo_(0) {
	type(TOOLBAR);
}

void ToolBar::add(ChildInfo *o) {
	if (!ncinfo_) {
		cinfo_ = new ChildInfo*[1];
	} 
	else if (!(ncinfo_ & (ncinfo_-1))) {
		ChildInfo** newarray = new ChildInfo*[ncinfo_<<1];
		memcpy(newarray, cinfo_, ncinfo_*sizeof(ChildInfo*));
		delete[] cinfo_;
		cinfo_ = newarray;
	}
	cinfo_[ncinfo_++] = o;
}

ToolBar::~ToolBar() {
	delete [] cinfo_;
}

void ToolBar::child_info(int f, int fg, int rg, Widget *w) {
	if(!w) w = child(children() - 1);
	int index = find_info(w);
	if(index != ncinfo_) cinfo_[index] = new ChildInfo(w, f, fg, rg);
	else add(new ChildInfo(w, f, fg, rg));
}

int ToolBar::find_info(Widget *w) {
	for(int i = 0; i < ncinfo_; i++) {
		if(cinfo_[i]->widget() == w) return i;
	}
	return ncinfo_;
}

void ToolBar::init_child() {
	ChildInfo **newcinfo_ = new ChildInfo*[children()];
	int index = 0;
		
	for(int i = 0; i < children(); i++) {
		index = find_info(child(i));
		if(index != ncinfo_) newcinfo_[i] = cinfo_[index];
		else newcinfo_[i] = new ChildInfo(child(i));
	}
	if(cinfo_) delete [] cinfo_;
	ncinfo_ = children();
	cinfo_ = newcinfo_;
}

void ToolBar::layout() {
	if (!layout_damage()) return;
	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE)) || !children()) {
		GripperBar::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
		
	Rectangle r(w(), h());
	box()->inset(r);
	int maxw = w(), maxh = h();
	int sdmh = h() - r.h(), sdmw = w() - r.w();
	
	if(orientation()) {
		gripper()->set(r.x(), r.y(), gripper()->thickness(), r.h());
		r.move_x(gripper()->thickness());
		maxh = h();
	}
	else {
		gripper()->set(r.x(), r.y(), r.w(), gripper()->thickness());
		r.move_y(gripper()->thickness());
		maxw = w();
	}
	for (int i = 0; i < children(); i++) {
		Widget* o = child(i);	
		if(!o->visible()) continue;
		
		int X, Y, W, H;
		if (orientation()) {
			X = r.x() + cinfo_[i]->front_gap();
			W = cinfo_[i]->iw();
			if(cinfo_[i]->flag_ & ToolBar::FIXED_HEIGHT) {
				maxh = (maxh < cinfo_[i]->ih())? cinfo_[i]->ih() + sdmh : maxh;
				Y = r.y()+ (r.h() - cinfo_[i]->ih()) / 2;
				H = cinfo_[i]->ih();
			}
			else {
				Y = r.y();
				H = r.h();
			}
			r.move_x(cinfo_[i]->front_gap() + W + cinfo_[i]->rear_gap());
		}
		else {
			Y = r.y() + cinfo_[i]->front_gap();
			H = cinfo_[i]->ih();
			if(cinfo_[i]->flag_ & ToolBar::FIXED_WIDTH) {
				maxw = (maxw < cinfo_[i]->iw())? cinfo_[i]->iw() + sdmw : maxw;
				X = r.x() + (r.w() - cinfo_[i]->iw()) / 2;
				W = cinfo_[i]->iw();
			}
			else {
				X = r.x();
				W = r.w();
			}
			r.move_y(cinfo_[i]->front_gap() + H + cinfo_[i]->rear_gap());
		}
		if (X != o->x() || Y != o->y() || W != o->w() || H != o->h()) {
			o->resize(X, Y, W, H);
			o->layout_damage(o->layout_damage() | damage);
			o->layout();
		}
	}
	if(orientation()) min_sizes(
				max(r.x() + sdmw/2, min_w()),
				max(maxh, min_h()), ORIENTED_HORIZONTAL);
	else min_sizes(
				max(maxw, min_w()), 
				max(r.y() + sdmh/2, min_h()), ORIENTED_VERTICAL);
	
	Widget::resize(min_w(), min_h());
// 	window()->resize(min_w(), min_h());
}

//
// End of $Id: ToolBar.cxx,v 1.2 2006/06/18 10:35:48 z_hossain Exp $.
//
