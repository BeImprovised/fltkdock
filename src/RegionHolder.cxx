//
// $Id: RegionHolder.cxx,v 1.2 2006/06/23 21:39:10 z_hossain Exp $
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

#include <fltk/RegionHolder.h>
#include <PlainHolder.h>
#include <Utility.h>

#include <fltk/layout.h>
#include <fltk/Symbol.h>
#include <fltk/events.h>

#define RANGE_EXT 30
#define NPHOLDER 3
#define NALIGN 5

using namespace fltk;

/*! \class fltk::RegionHolder

	This is the most commonly used composite holder class. Usually it contains 
	several instances of fltk::BarHolder and fltk::WindowHolder.
*/

/*! \fn void RegionHolder::end();
	Perform initialization and arrangement of new widgets after addition to the parent. 
	Appropriate version of this method called must be ensured since it is not virtual.
*/
 
RegionHolder::RegionHolder(int X, int Y, int W, int H)
	: DockHolder(X, Y, W, H, HALIGN_CENTER), holder_(0) {
	match_id(MATCH_REGIONHOLDER);
	type(REGION_HOLDER);
}

RegionHolder::~RegionHolder() {
	delete [] holder_;
}

void RegionHolder::layout() {
	if (!layout_damage()) return;
	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE)) || !children()) {
		DockHolder::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
	
	Rectangle r(w(), h());
	box()->inset(r);

	for(int i = 0; i < children(); i++) {
		if(holder_[i]->align_holder() == HALIGN_TOP) {
			holder_[i]->resize(r.x(), r.y(), r.w(), holder_[i]->h());
			holder_[i]->layout_damage(holder_[i]->layout_damage() | damage);
			holder_[i]->layout();
			r.move_y(holder_[i]->h());
		}
		else if(holder_[i]->align_holder() == HALIGN_BOTTOM) {
			holder_[i]->resize(r.x(), r.b() - holder_[i]->h(), r.w(), holder_[i]->h());
			holder_[i]->layout_damage(holder_[i]->layout_damage() | damage);
			holder_[i]->layout();
			r.move_b( -holder_[i]->h());
		}
		else if(holder_[i]->align_holder() == HALIGN_LEFT) {
			holder_[i]->resize(r.x(), r.y(), holder_[i]->w(), r.h());
			holder_[i]->layout_damage(holder_[i]->layout_damage() | damage);
			holder_[i]->layout();
			r.move_x(holder_[i]->w());
		}
		else if(holder_[i]->align_holder() == HALIGN_RIGHT) {
			holder_[i]->resize(r.r() - holder_[i]->w(), r.y(), holder_[i]->w(), r.h());
			holder_[i]->layout_damage(holder_[i]->layout_damage() | damage);
			holder_[i]->layout();
			r.move_r( -holder_[i]->w());
		}
		else if(holder_[i]->align_holder() == HALIGN_CENTER) {
			holder_[i]->resize(r.x(), r.y(), r.w(), r.h());
			holder_[i]->layout_damage(holder_[i]->layout_damage() | damage);
			holder_[i]->layout();
		}
	}
}

void RegionHolder::draw() {
	DockHolder::draw();
	if(preview()->flag(Preview::VISIBLE)) preview()->draw(*this);
}

void RegionHolder::init_child() {
	if(holder_) delete [] holder_;
	
	int *r, i;
	holder_ = new DockHolder*[children()];
	for(i = 0; i < children(); i++) {
		holder_[i] = dynamic_cast<DockHolder *>(child(i));
		r = holder_[i]->range();
		r[RANGE_TOP] = r[RANGE_LEFT] = -RANGE_EXT;
		r[RANGE_BOTTOM] = r[RANGE_RIGHT] = +RANGE_EXT;
	}
	int align[] = { HALIGN_TOP, HALIGN_BOTTOM, HALIGN_LEFT, HALIGN_RIGHT, HALIGN_CENTER };
	for(int k = 1, index = 0; k < MATCHID_MASK; k <<= 1) {
	if(Global::fill_vertical() && k == MATCH_TABHOLDER) {
		fltk::swap(&align[0], &align[2]);
		fltk::swap(&align[1], &align[3]);
	}
	for(int j = 0; j < NALIGN; j++) {
	for(i = index; i < children(); i++) {
		if(holder_[i]->align_holder() == align[j] && (holder_[i]->match_id() & k)) {
			DockHolder *t = holder_[index];
			holder_[index] = holder_[i];
			holder_[i] = t;
			index++;
		}
	}
	}
	}
	if(status()) status()->read_parent(parent_window());
}

bool RegionHolder::in_range(DockWindow &dw, DockHolder &dh) {
	Rectangle r1;
	dh.get_absolute_rect(&r1);
	
	switch(dh.align_holder()) {
		case HALIGN_LEFT:
		case HALIGN_RIGHT:
			return (	event_x_root() >= r1.x() + dh.range()[RANGE_LEFT] 	&&
								event_x_root() <= r1.r() + dh.range()[RANGE_RIGHT] 	&&
								event_y_root() >= r1.y() && event_y_root() <= r1.b() );
		case HALIGN_TOP:
		case HALIGN_BOTTOM:
			return (	event_x_root() >= r1.x() + dh.range()[RANGE_LEFT] 	&&
								event_x_root() <= r1.r() + dh.range()[RANGE_RIGHT] 	&&
								event_y_root() >= r1.y() + dh.range()[RANGE_TOP]		&&
								event_y_root() <= r1.b() + dh.range()[RANGE_BOTTOM] );
		default:
			return false;
	}
}

int RegionHolder::find_holder(Widget *w) {
	for(int i = 0; i < children(); i++) {
		if(holder_[i]->find(w) < holder_[i]->children()) {
			return i;
		}
	}
	return children();
}

void RegionHolder::try_dock(DockWindow *dw) {
	for(int i = 0; i < children(); i++) {
		if(in_range(*dw, *holder_[i])
					&& dw->dockable() & holder_[i]->align_holder()
				 	&& dw->match_id() & match_id()
				 	&& dw->match_id() & holder_[i]->match_id()) {
			holder_[i]->dock(dw);
			dw->state(holder_[i]->align_holder());
			notify_dock(dw);
			return;
		}
	}
}

void RegionHolder::dock(DockWindow *dw, Flags a) {
	for(int i = 0; i < children(); i++) {
		if(holder_[i]->align_holder() & a & dw->dockable() 
				 && (dw->match_id() & holder_[i]->match_id())) {
			dw->orientation(holder_[i]->orientation());
			holder_[i]->dock(dw);
			dw->state(holder_[i]->align_holder());
			notify_dock(dw);
			return;
		}
	}
}

void RegionHolder::try_undock(DockWindow *dw, int dx, int dy) {
	if(dx == MAX_INT && dy == MAX_INT) {
		dw->dock_in()->try_undock(dw, dx, dy);
		return;
	}
	dw->dock_in()->try_undock(dw, dx, dy);
	if(dw->state() == DOCKED_NONE) {
		dw->dock_in(0);
		dw->state(DOCKED_NONE);
		notify_undock(dw);
	}
}

void RegionHolder::undock(DockWindow *dw) {
	int temp;
	if((temp = find_holder(dw)) < children()) {
		holder_[temp]->undock(dw);
		dw->state(DOCKED_NONE);
		notify_undock(dw);
	}
}

void RegionHolder::preview_dock(DockWindow *dw) {
	static DockHolder *hpreview = 0;
	if(hpreview) {
		redraw(hpreview->preview()->redraw(*this));
		hpreview->preview_dock();
	}
	if(dw == 0) return;
	
	for(int i = 0; i < children(); i++) {
		if(in_range(*dw, *holder_[i])
				 && dw->dockable() & holder_[i]->align_holder()
				 && dw->match_id() & holder_[i]->match_id()) {
			redraw(holder_[i]->preview()->redraw(*this));
			holder_[i]->preview_dock(dw);
			hpreview = holder_[i];
			break;
		}
	}
}

void RegionHolder::initiate() {
	if(status()) {
		for(int i = status()->ndockwin_; i--; ) {
			status()->read_status(status()->dockwin_[i]);
		}
	}
	PlainHolder *ph;
	for(int i = children(); i--; ) {
		ph = dynamic_cast<PlainHolder *>(holder_[i]);
		if(ph) ph->init_child();
	}
}

//
// End of $Id: RegionHolder.cxx,v 1.2 2006/06/23 21:39:10 z_hossain Exp $.
//
